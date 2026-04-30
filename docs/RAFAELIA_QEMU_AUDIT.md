# RAFAELIA QEMU Audit

Data da auditoria: 2026-04-30 (UTC)

## 1) Base upstream aproximada

A árvore atual contém commits de integração de staging do QEMU com histórico recente de 2026.
Como não há remoto `origin` configurado neste clone, a base upstream foi inferida pelo histórico local.
Commits de referência vistos no histórico:

- `3b5fe75e` - Merge tag `pull-monitor-2026-01-07` de `repo.or.cz/qemu/armbru` para `staging`
- `942b0d37` - Merge tag `for-upstream` de `gitlab.com/bonzini/qemu` para `staging`

Conclusão: o fork está ancorado em uma base QEMU upstream moderna (ciclo 10.x/11.x em progresso), com merge contínuo de branches de maintainers.

## 2) Commits próprios (não-upstream) identificados

Amostra representativa dos commits RAFAELIA/Vectras no topo do histórico:

- `4867a02b` rafaelia: restore integration suite and isolate IPC cycle test
- `9db5296b` rafaelia: enforce IPC cycle gate and digest chain
- `fff4dc05` rafaelia: implement threaded IPC backends and priority routing
- `52116897` Align Android native build pipeline and CI artifacts
- `91a61ee0` docs: refactor rafaelia documentation to auditable baseline
- `d4e2f6e0` Fix RAFAELIA integration standalone build ABI linkage
- `5f599747` hw/core/rafaelia: harden ABI integration and route robustness
- `884d6966` Add RAFAELIA bridge API and wire VMService JNI metrics
- `eea98c1c` rafaelia-rmr: add deterministic route selection and core integration
- `7bb8da40` rafaelia: introduz kernel ABI estável e isola shell QEMU

## 3) Arquivos/áreas modificadas relevantes

Pelo histórico de commits RAFAELIA:

- `hw/core/` (integração RAFAELIA/RMR, ABI, roteamento)
- `tests/unit/` (cobertura de integração RMR)
- integração Android/JNI/CI em camadas de build
- documentação RAFAELIA em `docs/`

## 4) Auditoria por áreas críticas solicitadas

Status inferido por inspeção de histórico local recente:

- `accel/tcg/`: sem evidência de patch RAFAELIA direto recente
- `tcg/`: sem evidência de patch RAFAELIA direto recente
- `target/arm/`: sem evidência de patch RAFAELIA direto recente
- `target/i386/`: sem evidência de patch RAFAELIA direto recente
- `hw/arm/`: histórico upstream ativo; sem patch RAFAELIA recente identificado
- `softmmu/`: sem evidência de patch RAFAELIA direto recente
- `qapi/`: alterações vistas são upstream
- `monitor/qmp*`: alterações vistas são upstream
- `meson.build/configure`: sem patch RAFAELIA recente evidente nos commits listados

**TCG ainda está majoritariamente upstream; integração RAFAELIA atual é externa/planejada.**

## 5) Integração segura recomendada (não invasiva)

1. Telemetria QMP
   - Expor métricas RAFAELIA via comando QMP dedicado (read-only) sem alterar pipeline de tradução TCG.
2. Presets CLI documentados
   - Perfis de execução RAFAELIA/Vectras com flags estáveis por target.
3. Build profile Android/Termux
   - Perfil de configuração específico para cross/mobile mantendo defaults upstream intactos.
4. Hook de tracing não invasivo
   - Instrumentação em pontos de tracing/monitor e bordas de device/core antes de qualquer patch no TCG.

## 6) Riscos e limites

- Sem remoto upstream configurado neste clone, a determinação de “base exata” é aproximada.
- A validação final de divergência deve ser feita com remoto QEMU oficial e `git range-diff` quando o remoto estiver disponível.
