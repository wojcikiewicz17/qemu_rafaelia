# RAFAELIA RMR (Runtime Memory Reduction)

> **Status**: MVP disponível (pool de blocos + detecção de hardware).
> **Escopo**: reduzir overhead de alocação e manter interoperabilidade low-level.

## Navegação

- [Resumo executivo](#resumo-executivo)
- [Objetivos e não-objetivos](#objetivos-e-não-objetivos)
- [MVP (escopo mínimo viável)](#mvp-escopo-mínimo-viável)
- [Arquitetura](#arquitetura)
- [API pública](#api-pública)
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
- Função de prefetch compatível com GCC/Clang.
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
```

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
- [ ] Revisar capacidade do pool conforme o perfil de uso real.
