# RAFAELIA ModulomR (Módulo MR) - Autoria e Licenciamento

> **Objetivo**: consolidar as alterações autorais do RAFAELIA em um módulo
> específico, mantendo rastreabilidade de autoria e aderência às licenças do
> repositório.

---

## O que é o ModulomR

O **ModulomR** ("Módulo MR") é o registro formal de autoria e licenciamento das
alterações específicas da RAFAELIA. Ele centraliza:

- **Autoria**: quais arquivos ou subsistemas foram introduzidos/alterados;
- **Escopo**: onde as mudanças estão concentradas;
- **Licença**: referência direta às licenças vigentes do repositório;
- **Origem**: descrição curta de autoria para manter rastreabilidade.

O módulo foi desenhado para facilitar auditorias e evitar que alterações
específicas se percam no histórico geral do QEMU.

---

## Localização do Módulo

- **Header**: `include/hw/core/rafaelia-modulomr.h`
- **Implementação**: `hw/core/rafaelia-modulomr.c`

A função `rafaelia_modulomr_entries()` expõe o inventário de autoria. Essa lista
serve como referência para documentação, revisão de licenças e histórico de
contribuições.

Entradas autorais recentes incluem o **RAFAELIA_RMR**, focado em reduzir
overhead de alocação e expor perfil de hardware para otimizações low-level. A
documentação técnica do módulo está em `docs/RAFAELIA_RMR.md`.

---

## Como atualizar o registro

1. **Quando criar ou alterar algo de autoria RAFAELIA**, inclua uma nova entrada
   em `rafaelia_modulomr_table`.
2. **Defina o escopo** (arquivos ou diretórios específicos).
3. **Adicione um resumo claro** do que foi feito.
4. **Referencie a licença** aplicável (usando os headers do arquivo e
   `COPYING`/`COPYING.LIB`).

> **Nota**: Não substitua licenças já existentes; respeite o licenciamento
> original e documente apenas o que for RAFAELIA.

---

## Diretrizes de autoria

- **Sempre documentar** alterações significativas no ModulomR.
- **Evitar duplicidade**: apenas o essencial para rastrear autoria.
- **Seguir o licenciamento do QEMU** conforme indicado nos arquivos e nas
  licenças raiz (`COPYING`, `COPYING.LIB`).

## Política de licenciamento

- **Não adicionar licenças pessoais**: o ModulomR não substitui o licenciamento
  do repositório e não admite licenças proprietárias individuais.
- **Manter compatibilidade**: toda alteração RAFAELIA deve respeitar o conjunto
  de licenças existentes (`COPYING`, `COPYING.LIB`).

---

## Exemplo de entrada

```c
{
    .identifier = "RAFAELIA_BOOTBLOCK_v1",
    .scope = "hw/core/rafaelia-core.*",
    .summary = "Low-level RAFAELIA core formulas and bootblock runtime.",
    .license = "Follow the file header and COPYING/COPYING.LIB.",
    .origin = "Rafaelia authorship (see docs/RAFAELIA_MODULOMR.md).",
},
```

---

## Conclusão

O ModulomR garante que as alterações de autoria RAFAELIA fiquem bem separadas e
claramente documentadas, mantendo coerência com as licenças do projeto e
facilitando auditorias futuras.
