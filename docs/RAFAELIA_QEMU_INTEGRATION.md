# RAFAELIA QEMU Integration

## Objetivo

Definir integração segura do ecossistema RAFAELIA/Vectras com QEMU sem regressão funcional do upstream.

## Integrações já seguras

1. Documentação de presets CLI por target/uso.
2. Telemetria via QMP (proposta de expansão read-only).
3. Hooks de tracing não invasivos fora do tradutor TCG.
4. Perfis de build para Android/Termux preservando defaults upstream.

## Fronteira de responsabilidade

- QEMU upstream: emulação, tradução, modelos de dispositivo.
- RAFAELIA: orquestração externa, coleta de métricas, runtime de integração e automação de build/release.

## Regras de evolução

- Mudanças em TCG só com benchmark + teste de regressão + toggle de feature.
- Toda integração nova deve ter caminho de rollback.
- Nunca substituir defaults upstream por comportamento RAFAELIA de forma silenciosa.
