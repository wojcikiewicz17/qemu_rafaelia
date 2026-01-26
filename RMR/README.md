# RMR Authorial Notes (RAFAELIA)

Este diretório concentra **anotações autorais** relacionadas às melhorias do
módulo **RAFAELIA RMR (Runtime Memory Reduction)**. O objetivo é manter um
registro claro de decisões de performance e de integração com o core.

## Melhorias recentes

- **Alocação sem zero-initialize**: novo caminho de alocação no pool RMR para
  estruturas hot que são totalmente inicializadas pelo chamador.
- **Inicialização explícita de blocos**: o core RAFAELIA inicializa todos os
  campos necessários ao usar o caminho rápido, reduzindo o overhead do memset
  completo.

## Referências

- Documentação técnica do RMR: `docs/RAFAELIA_RMR.md`
- Registro de autoria: `docs/RAFAELIA_MODULOMR.md`
- API do core: `include/hw/core/rafaelia-core.h`
