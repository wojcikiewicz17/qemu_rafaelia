# RAFAELIA TCG Roadmap

## Estado atual

**TCG ainda está majoritariamente upstream; integração RAFAELIA atual é externa/planejada.**

## Fases

1. Fase 0 — Observabilidade
   - Tracing + QMP telemetry sem alterar tradutor.
2. Fase 1 — Ensaios controlados
   - Provas de conceito behind-flag em branch experimental.
3. Fase 2 — Validação
   - Benchmarks reproduzíveis por arquitetura (arm/i386/x86_64).
4. Fase 3 — Proposta formal
   - Patchset pequeno, revisável, com fallback para comportamento upstream.

## Não-objetivos (no estado atual)

- Não há implementação comprovada de NEON batching no TCG.
- Não há implementação comprovada de prefetch/cache-aware execution no TCG.
- Não há alteração de pipeline de tradução declarada como produção.
