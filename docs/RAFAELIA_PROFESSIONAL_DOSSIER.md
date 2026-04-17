# RAFAELIA Professional Dossier (Refatorado)

**Versão:** 3.0.0  
**Data:** 2026-04-17

## 1) Resumo executivo

QEMU Rafaelia é tratado como um sistema único com dois eixos de entrega:

1. **Confiabilidade de build/CI** baseada em workflow reproduzível.
2. **Integração nativa RAFAELIA** validada por build e teste locais do módulo.

A proposta profissional desta refatoração foi converter documentação descritiva em documentação auditável.

## 2) O que foi profissionalizado

- Priorização de **fonte de verdade operacional** (workflow e Makefile reais).
- Remoção de dependência de claims não auditáveis como critério de qualidade.
- Estrutura orientada a execução: comando → evidência → artefato.
- Redução de ambiguidade para onboarding técnico (engenharia, CI, release).

## 3) Governança técnica

### 3.1 Princípios

- Reprodutibilidade antes de narrativa.
- Evidência antes de marketing técnico.
- Mudança mínima e verificável.
- Alinhamento entre código, automação e documentação.

### 3.2 Controle de risco

- Evitar divergência entre texto e pipeline.
- Evitar “falso verde” (claim sem execução).
- Preservar trilha oficial de build sem atalhos inseguros.

## 4) Indicadores recomendados (somente com prova)

Indicadores aceitos no dossiê:
- taxa de sucesso de jobs CI por período,
- tempo médio de build,
- sucesso/falha de testes do módulo de integração,
- presença de artefatos obrigatórios.

Todos devem ser publicados com data e comando/fonte de extração.

## 5) Mapa de documentos

- Baseline técnico: `docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md`
- Readme operacional: `README_RAFAELIA.md`
- Cobertura de árvore: `docs/RAFAELIA_DIRECTORY_AUDIT.md`

## 6) Próximos passos recomendados

1. Adicionar job dedicado para `make -f hw/core/Makefile.integration test` no CI.
2. Publicar relatório de tempos de build por commit (sem inflar escopo de release).
3. Introduzir checklist de evidência para qualquer claim novo em documentação.
