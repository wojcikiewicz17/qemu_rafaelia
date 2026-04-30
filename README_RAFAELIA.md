# RAFAELIA no fork QEMU

Este fork mantém QEMU majoritariamente upstream e adiciona uma camada de integração RAFAELIA/Vectras principalmente fora do núcleo TCG.

## Estado atual

- Núcleo de emulação: majoritariamente upstream.
- Integração RAFAELIA: concentrada em camadas de integração/ABI/telemetria e documentação.
- Objetivo: evolução incremental sem quebrar comportamento upstream.

## Princípios

- Não declarar otimizações de TCG sem patch real.
- Evitar alterações invasivas prematuras em `tcg/` e `accel/tcg/`.
- Priorizar observabilidade (QMP/tracing), reprodutibilidade de build e contrato estável de integração.

## Documentação associada

- `docs/RAFAELIA_QEMU_AUDIT.md`
- `docs/RAFAELIA_QEMU_INTEGRATION.md`
- `docs/RAFAELIA_TCG_ROADMAP.md`
