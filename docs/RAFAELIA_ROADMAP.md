# RAFAELIA Roadmap & Mapa de Documentação

## Objetivo

Este documento organiza o **roadmap** do projeto RAFAELIA, mapeando o que **já está documentado ("já impresso")** e o que **ainda falta** para completar o pacote de integração, validação e entrega. Ele também aponta os documentos necessários e onde encontrá-los.

---

## 1) Mapa do que já está documentado ("já impresso")

| Área | Documento | O que cobre |
|------|-----------|-------------|
| Visão geral e onboarding | [README_RAFAELIA.md](../README_RAFAELIA.md) | Visão macro, integração multi-repositório, links e status geral |
| Arquitetura de integração | [INTEGRATION_ARCHITECTURE.md](../INTEGRATION_ARCHITECTURE.md) | Design estratégico, camadas e decisões |
| Guia de uso & API | [INTEGRATION_GUIDE.md](../INTEGRATION_GUIDE.md) | Guia de uso, API e fluxos de integração |
| Resumo executivo | [INTEGRATION_SUMMARY.md](../INTEGRATION_SUMMARY.md) | Métricas, impactos e resultados |
| Implementação core | [RAFAELIA_IMPLEMENTATION.md](../RAFAELIA_IMPLEMENTATION.md) | Núcleo RAFAELIA e fórmulas |
| Hook de runtime | [docs/RAFAELIA_RUNTIME_HOOK.md](RAFAELIA_RUNTIME_HOOK.md) | Hook vivo, métricas e flags |
| Otimizações QEMU | [QEMU_IMPROVEMENTS_README.md](../QEMU_IMPROVEMENTS_README.md) | Melhorias de performance e estabilidade |
| UI/UX | [docs/UI_UX_INTEGRATION_GUIDELINES.md](UI_UX_INTEGRATION_GUIDELINES.md) | Padrões Android/ISO/WCAG |
| Erros & recuperação | [docs/ERROR_HANDLING_PATTERNS.md](ERROR_HANDLING_PATTERNS.md) | Taxonomia de erros e estratégias |
| Simbiose (visual→código) | [docs/RAFAELIA_SYMBIOSIS_MAPPING.md](RAFAELIA_SYMBIOSIS_MAPPING.md) | Mapeamento visual para módulos |
| Autoria e licenças | [docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md](RAFAELIA_AUTHORSHIP_BOUNDARIES.md) | Limites de autoria e módulos |
| Registry de módulos | [docs/RAFAELIA_MODULOMR.md](RAFAELIA_MODULOMR.md) | Registro ModulomR |
| RMR (memória) | [docs/RAFAELIA_RMR.md](RAFAELIA_RMR.md) | Runtime Memory Reduction |
| Dossiê técnico | [docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md](RAFAELIA_TECHNICAL_DOCUMENTATION.md) | Documento técnico completo |
| Dossiê profissional | [docs/RAFAELIA_PROFESSIONAL_DOSSIER.md](RAFAELIA_PROFESSIONAL_DOSSIER.md) | Posição acadêmica/mercado |

---

## 2) O que ainda falta (lacunas identificadas)

Itens abaixo **não foram localizados na árvore atual** e devem ser produzidos para fechar o ciclo de entrega:

1. **Plano de release e versionamento** (cronograma por versão, critérios de corte e changelog consolidado).
2. **Matriz de compatibilidade** (QEMU upstream, Android alvo, kernels suportados, targets/sistemas hospedados).
3. **Plano de testes automatizados** com evidências rastreáveis (ex.: CI, relatórios e cobertura).
4. **Guia operacional/runbook** (monitoramento, recuperação, troubleshooting passo a passo).
5. **Modelo de governança e riscos** (papéis, manutenção e SLAs internos do framework).

---

## 3) Roadmap recomendado (macro-fases)

### Fase 1 — Consolidação documental (Curto prazo)
- Publicar plano de release e versionamento.
- Adicionar matriz de compatibilidade.
- Publicar runbook operacional.

### Fase 2 — Validação e evidências (Médio prazo)
- Implementar pipeline de testes automatizados e relatórios.
- Criar página de evidências de desempenho (benchmarks com metodologia replicável).

### Fase 3 — Governança e sustentabilidade (Médio/Longo prazo)
- Definir governança do framework (papéis, responsabilidades, processo de revisão).
- Formalizar modelo de risco e continuidade.

---

## 4) Documentos necessários (anexos)

Os documentos listados abaixo compõem o pacote essencial de entrega. Todos estão presentes no repositório e podem ser anexados em apresentações ou dossiês:

- README principal: [README_RAFAELIA.md](../README_RAFAELIA.md)
- Arquitetura: [INTEGRATION_ARCHITECTURE.md](../INTEGRATION_ARCHITECTURE.md)
- Guia/API: [INTEGRATION_GUIDE.md](../INTEGRATION_GUIDE.md)
- Resumo executivo: [INTEGRATION_SUMMARY.md](../INTEGRATION_SUMMARY.md)
- Implementação core: [RAFAELIA_IMPLEMENTATION.md](../RAFAELIA_IMPLEMENTATION.md)
- Hook de runtime: [docs/RAFAELIA_RUNTIME_HOOK.md](RAFAELIA_RUNTIME_HOOK.md)
- Otimizações: [QEMU_IMPROVEMENTS_README.md](../QEMU_IMPROVEMENTS_README.md)
- Simbiose: [docs/RAFAELIA_SYMBIOSIS_MAPPING.md](RAFAELIA_SYMBIOSIS_MAPPING.md)
- UI/UX: [docs/UI_UX_INTEGRATION_GUIDELINES.md](UI_UX_INTEGRATION_GUIDELINES.md)
- Erros: [docs/ERROR_HANDLING_PATTERNS.md](ERROR_HANDLING_PATTERNS.md)
- Autoria & licenças: [docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md](RAFAELIA_AUTHORSHIP_BOUNDARIES.md)
- Registry: [docs/RAFAELIA_MODULOMR.md](RAFAELIA_MODULOMR.md)
- RMR: [docs/RAFAELIA_RMR.md](RAFAELIA_RMR.md)
- Dossiê técnico: [docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md](RAFAELIA_TECHNICAL_DOCUMENTATION.md)
- Dossiê profissional: [docs/RAFAELIA_PROFESSIONAL_DOSSIER.md](RAFAELIA_PROFESSIONAL_DOSSIER.md)

---

## 5) Próximos passos imediatos

1. Validar a lista de lacunas com stakeholders.
2. Definir responsáveis e prazos por item.
3. Integrar os novos documentos na hub de documentação principal.
