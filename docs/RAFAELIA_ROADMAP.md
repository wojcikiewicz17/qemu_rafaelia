# RAFAELIA Roadmap & Mapa de Documentação (Atualizado)

## Objetivo

Este documento consolida o status real da documentação e define um roadmap detalhado de execução para o ecossistema RAFAELIA, com foco em coerência técnica, rastreabilidade e entrega contínua.

---

## 1) Diretórios auditados (checked)

Auditoria realizada na árvore do repositório para validar presença de código, documentação e infraestrutura operacional:

- `.` (raiz do projeto)
- `.github/workflows`
- `analysis_reports/qemu_rafaelia`
- `docs`
- `hw/core`
- `include`
- `linux-user`
- `migration`
- `scripts`
- `target`
- `tests`
- `util`

> Inventário detalhado em: [docs/RAFAELIA_DIRECTORY_AUDIT.md](RAFAELIA_DIRECTORY_AUDIT.md)

---

## 2) Mapa do que já está documentado (baseline atual)

| Área | Documento | Status |
|------|-----------|--------|
| Visão geral e onboarding | [README_RAFAELIA.md](../README_RAFAELIA.md) | ✅ Publicado |
| Arquitetura de integração | [INTEGRATION_ARCHITECTURE.md](../INTEGRATION_ARCHITECTURE.md) | ✅ Publicado |
| Guia de uso e API | [INTEGRATION_GUIDE.md](../INTEGRATION_GUIDE.md) | ✅ Publicado |
| Resumo executivo | [INTEGRATION_SUMMARY.md](../INTEGRATION_SUMMARY.md) | ✅ Publicado |
| Implementação core | [RAFAELIA_IMPLEMENTATION.md](../RAFAELIA_IMPLEMENTATION.md) | ✅ Publicado |
| Hook de runtime | [docs/RAFAELIA_RUNTIME_HOOK.md](RAFAELIA_RUNTIME_HOOK.md) | ✅ Publicado |
| Otimizações de runtime | [QEMU_IMPROVEMENTS_README.md](../QEMU_IMPROVEMENTS_README.md) | ✅ Publicado |
| Simbiose visual→código | [docs/RAFAELIA_SYMBIOSIS_MAPPING.md](RAFAELIA_SYMBIOSIS_MAPPING.md) | ✅ Publicado |
| UI/UX e padrões ISO | [docs/UI_UX_INTEGRATION_GUIDELINES.md](UI_UX_INTEGRATION_GUIDELINES.md) | ✅ Publicado |
| Erros e recuperação | [docs/ERROR_HANDLING_PATTERNS.md](ERROR_HANDLING_PATTERNS.md) | ✅ Publicado |
| Limites de autoria | [docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md](RAFAELIA_AUTHORSHIP_BOUNDARIES.md) | ✅ Publicado |
| Registry de módulos | [docs/RAFAELIA_MODULOMR.md](RAFAELIA_MODULOMR.md) | ✅ Publicado |
| Runtime Memory Reduction | [docs/RAFAELIA_RMR.md](RAFAELIA_RMR.md) | ✅ Publicado |
| Documento técnico amplo | [docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md](RAFAELIA_TECHNICAL_DOCUMENTATION.md) | ✅ Publicado |
| Dossiê profissional | [docs/RAFAELIA_PROFESSIONAL_DOSSIER.md](RAFAELIA_PROFESSIONAL_DOSSIER.md) | ✅ Publicado |

---

## 3) Lacunas atuais (priorizadas)

### Lacuna A — Release engineering integrado
**Necessidade:** formalizar versão-alvo, política de corte, changelog consolidado e critérios de rollback.

### Lacuna B — Matriz de compatibilidade operacional
**Necessidade:** mapear relação entre targets de CPU/arquitetura, host suportado, toolchain e cobertura de teste mínima.

### Lacuna C — Evidência automatizada de qualidade
**Necessidade:** consolidar relatório único para build/test/lint/perf e anexar em evidências reproduzíveis.

### Lacuna D — Runbook operacional de incidentes
**Necessidade:** padronizar troubleshooting, sinais de degradação, MTTR alvo e ações de contenção.

### Lacuna E — Governança técnica de manutenção
**Necessidade:** definir ownership por módulo, SLA de revisão e processo de aprovação para mudanças de alto impacto.

---

## 4) Roadmap detalhado por fase

## Fase 0 — Reconhecimento e Inventário (concluída)
**Objetivo:** identificar estado atual de diretórios críticos, documentação e pontos de integração.

**Entregas:**
- Mapa de diretórios auditados.
- Baseline documental consolidado.
- Lista priorizada de lacunas.

**Critério de conclusão:** inventário publicado e validado no repositório.

## Fase 1 — Consolidação de Base de Entrega (curto prazo)
**Objetivo:** tornar release previsível e verificável.

**Backlog executável:**
1. Criar `docs/RAFAELIA_RELEASE_POLICY.md` (versionamento + critérios de corte).
2. Criar `docs/RAFAELIA_CHANGELOG_STRATEGY.md` (padrão de changelog por componente).
3. Criar `docs/RAFAELIA_COMPATIBILITY_MATRIX.md` (host/target/toolchain/testes mínimos).

**KPIs da fase:**
- 100% das releases com critérios de aprovação explícitos.
- 100% das arquiteturas críticas com status de compatibilidade documentado.

## Fase 2 — Evidência de Qualidade e Operação (curto/médio prazo)
**Objetivo:** reduzir risco operacional e aumentar auditabilidade.

**Backlog executável:**
1. Criar `docs/RAFAELIA_TEST_EVIDENCE_PLAYBOOK.md`.
2. Criar `docs/RAFAELIA_OPERATIONAL_RUNBOOK.md`.
3. Integrar checklist de validação em pipeline (`.github/workflows` / scripts existentes).

**KPIs da fase:**
- Redução de incidentes sem diagnóstico formal.
- Cobertura de evidências por build principal >= 95% dos jobs críticos.

## Fase 3 — Governança e Sustentação (médio prazo)
**Objetivo:** manter evolução contínua com previsibilidade.

**Backlog executável:**
1. Criar `docs/RAFAELIA_GOVERNANCE_MODEL.md`.
2. Criar `docs/RAFAELIA_RISK_REGISTER.md`.
3. Definir matriz RACI por diretório estratégico (`hw/core`, `docs`, `scripts`, `tests`).

**KPIs da fase:**
- Tempo médio de revisão (PR) com tendência de queda.
- 100% de módulos críticos com responsável técnico definido.

## Fase 4 — Otimização contínua e benchmark orientado (médio/longo prazo)
**Objetivo:** institucionalizar melhoria contínua com base em métrica objetiva.

**Backlog executável:**
1. Publicar baseline de desempenho por arquitetura prioritária.
2. Definir regressão máxima tolerável por release.
3. Integrar relatórios em `analysis_reports/qemu_rafaelia`.

**KPIs da fase:**
- Regressão de performance por release dentro do limite alvo.
- Tendência de ganho em throughput/latência nos cenários críticos.

---

## 5) Sequência prática recomendada (ordem de execução)

1. Concluir documentos de Fase 1.
2. Estruturar evidência automatizada (Fase 2).
3. Formalizar governança (Fase 3).
4. Fechar ciclo com benchmark contínuo (Fase 4).

---

## 6) Próximos passos imediatos

1. Aprovar este roadmap como baseline atualizado.
2. Abrir issues por item de backlog com responsável e prazo.
3. Executar Fase 1 como prioridade para padronizar releases e compatibilidade.
