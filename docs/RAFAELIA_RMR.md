# RAFAELIA RMR (Runtime Memory Reduction)

> **Status**: MVP disponível (pool de blocos + detecção de hardware).
> **Escopo**: reduzir overhead de alocação e manter interoperabilidade low-level.

## Navegação

- [Resumo executivo](#resumo-executivo)
- [Objetivos e não-objetivos](#objetivos-e-não-objetivos)
- [MVP (escopo mínimo viável)](#mvp-escopo-mínimo-viável)
- [Arquitetura](#arquitetura)
- [API pública](#api-pública)
- [Fronteira Kernel ABI](#fronteira-kernel-abi)
- [Versionamento da ABI](#versionamento-da-abi)
- [Contrato de estabilidade de roteamento](#contrato-de-estabilidade-de-roteamento)
- [Integração com o core](#integração-com-o-core)
- [Considerações de desempenho](#considerações-de-desempenho)
- [Licenciamento e autoria](#licenciamento-e-autoria)
- [Checklist de manutenção](#checklist-de-manutenção)

## Resumo executivo

O **RMR (Runtime Memory Reduction)** é um módulo low-level para reduzir overhead
de alocação em objetos de alta frequência (ex.: `rafaelia_bloco_t`) e expor
informações mínimas de hardware (cache line, página, arquitetura e prefetch) sem
introduzir camadas de abstração adicionais.

## Objetivos e não-objetivos

**Objetivos**
- Reduzir a pressão de alocação dinâmica por meio de um pool fixo de blocos.
- Fornecer perfil simples de hardware para otimizações locais.
- Manter interoperabilidade com código C/low-level.

**Não-objetivos**
- Substituir alocadores globais do QEMU.
- Introduzir dependências externas de alocação.
- Implementar políticas de GC ou heurísticas complexas.

## MVP (escopo mínimo viável)

O MVP do RMR inclui:
- Pool fixo com `capacity` definida, alinhamento por cache line e fallback para
  `calloc` quando necessário.
- Detecção simples de arquitetura, cache line e tamanho de página.
- Snapshot de instrumentos de sistema (kernel/máquina/CPU/RAM/KVM) para
  roteamento determinístico orientado ao host.
- Função de prefetch compatível com GCC/Clang.
- Primitivas low-level com fast-path em ASM (x86_64 `rep stosb/movsb`) e
  fallback determinístico por palavras para outros alvos.
- Opção de alocação sem zero-initialize para caminhos hot que inicializam
  manualmente os campos.

Esse conjunto permite ganhos imediatos em cenários com alto churn de blocos,
sem alterar a API pública do core.

## Arquitetura

```
rafaelia_rmr_pool_create()
   └─ aloca buffer alinhado (qemu_memalign)
      └─ encadeia free list (LIFO)

rafaelia_rmr_pool_alloc()
   └─ pop da free list → zero-initialize → retorno

rafaelia_rmr_pool_free()
   └─ push na free list
```

O módulo também expõe `rafaelia_rmr_detect()` para preencher um perfil de
hardware mínimo, usado como dica de alinhamento e prefetch.
Além disso, `rafaelia_rmr_route_select()` calcula seleção determinística de rota
(`fallback`, `portable`, `host_fast`, `kvm_accel`) para integração direta com
o core em baixo overhead.

## API pública

```c
rafaelia_rmr_pool_t *rafaelia_rmr_pool_create(size_t element_size,
                                              uint32_t capacity,
                                              uint32_t alignment);
void rafaelia_rmr_pool_destroy(rafaelia_rmr_pool_t *pool);
void *rafaelia_rmr_pool_alloc(rafaelia_rmr_pool_t *pool);
void *rafaelia_rmr_pool_alloc_uninitialized(rafaelia_rmr_pool_t *pool);
void rafaelia_rmr_pool_free(rafaelia_rmr_pool_t *pool, void *ptr);
bool rafaelia_rmr_pool_owns(const rafaelia_rmr_pool_t *pool, const void *ptr);
void rafaelia_rmr_detect(rafaelia_rmr_hw_profile_t *profile);
bool rafaelia_rmr_collect_instruments(rafaelia_rmr_instrument_snapshot_t *snapshot);
bool rafaelia_rmr_route_select(const rafaelia_rmr_instrument_snapshot_t *snapshot,
                              rafaelia_rmr_route_decision_t *decision);
```

## Fronteira Kernel ABI

A execução crítica do core RAFAELIA passa por uma fronteira estável definida em
`include/hw/core/rafaelia-kernel-abi.h`. Essa ABI contém somente tipos POD e
funções mínimas de entrada/saída para:

- memória (`memzero`, `memcmp_bytes`, `strlcpy`, `strlen_bytes`);
- alocação (`alloc_zero`, `free_mem`);
- pool (`pool_create`, `pool_destroy`, `pool_alloc_uninitialized`, `pool_owns`, `pool_free`);
- instrumentos e roteamento (`collect_instruments`, `route_select`);
- ruído determinístico (`rng_seed`, `rng_next`).

O adapter de infraestrutura QEMU fica isolado em
`hw/core/rafaelia-qemu-shell.c` e exporta `rafaelia_qemu_shell_abi()`.
Com isso, `hw/core/rafaelia-core.c` não precisa conhecer objetos de alto nível
do QEMU nem APIs diretas de RMR fora da ABI.

## Versionamento da ABI

Regras de evolução da fronteira:

1. **Compatibilidade binária first**: não alterar ordem/campos existentes dos
   structs públicos da ABI em releases estáveis.
2. **Mudança aditiva**: novos campos devem ser adicionados ao final de structs
   e novas funções no final de `rafaelia_kernel_abi_t`.
3. **Remoção só em major**: remoções/renomeações exigem versão major e janela
   de migração documentada.
4. **Fallback determinístico**: funções novas devem manter fallback seguro
   quando o provider não suportar recurso avançado.
5. **Core isolado**: qualquer novo ponto de infraestrutura deve entrar primeiro
   na ABI antes de uso no core.

## Contrato de estabilidade de roteamento

O roteamento do core usa tabela estática (`hw/core/rafaelia-route-table.c`) com
chaves mínimas por regra:

- `arch`
- `has_kvm_accel`
- `cpu_online` (limiar mínimo)
- `page_bytes` (limiar mínimo da regra)

Contrato de estabilidade:

1. A função `rafaelia_route_select()` é **determinística**: com o mesmo
   `rafaelia_rmr_instrument_snapshot_t`, o resultado (`id` e `name`) é sempre o
   mesmo.
2. A decisão é **sem alocação dinâmica** e somente leitura de tabela estática.
3. A política de desempate é fixa por ordem de declaração da tabela.
4. A comparação de arquitetura aceita aliases estáveis (ex.: regra `x86_64` pode casar snapshot `x86`/`i386`; `riscv` aceita variantes `riscv*`).
5. Quando nenhuma rota casa, o fallback estável `portable-fallback` é usado.

Essas regras evitam variabilidade runtime e facilitam auditoria de performance
por arquitetura/host.

## Integração com o core

- `rafaelia_core_init()` chama o acquire do pool de blocos.
- `rafaelia_core_cleanup()` libera o pool quando não houver usuários.
- `rafaelia_bloco_create()` usa o pool como primeira opção e cai para `calloc`.
- `rafaelia_bloco_free()` devolve ao pool quando aplicável.

Essa integração é transparente ao usuário do core.

## Considerações de desempenho

- O pool usa free list LIFO para baixo overhead.
- O alinhamento padrão segue `qemu_dcache_linesize` quando disponível.
- Prefetch é opcional (GCC/Clang), sem custo em outros compiladores.
- `rafaelia_rmr_pool_alloc_uninitialized()` evita o zero-initialize completo do
  bloco; use apenas quando todos os campos necessários forem inicializados
  explicitamente pelo chamador.

## Licenciamento e autoria

O RMR **não introduz nova licença**. Ele segue o licenciamento do repositório
(`COPYING`, `COPYING.LIB`) e as diretrizes do ModulomR. **Não é permitido**
aplicar licenças pessoais ou proprietárias ao módulo.

Notas autorais específicas do RMR ficam em `RMR/README.md`, mantendo o histórico
de decisões de performance e integração.

## Checklist de manutenção

- [ ] Atualizar `docs/RAFAELIA_MODULOMR.md` ao alterar escopo ou autoria.
- [ ] Manter a documentação de API em `hw/core/RAFAELIA_README.md`.
- [ ] Executar testes `test-rafaelia-rmr-pool`, `test-rafaelia-rmr-instruments` e
      `test-rafaelia-rmr-lowlevel` em alterações do módulo.
- [ ] Revisar capacidade do pool conforme o perfil de uso real.


## Changelog técnico (curto)

- Ajuste de robustez na coleta de instrumentos para builds não-POSIX,
  garantindo `page_bytes` default estável.
- Remoção de duplicação de includes em `hw/core/rafaelia-rmr.c`.
- Endurecimento da comparação de arquitetura e seleção de rota na tabela
  determinística.
- Alinhamento da integração core ↔ kernel ABI para evitar dependências
  implícitas de tipos internos do RMR.
