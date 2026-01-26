# RAFAELIA Runtime Hook — Mapa de Execução

## Objetivo

Fechar o ciclo **compilar ≠ rodar** com um hook vivo no runtime do QEMU,
expondo um contrato mínimo de execução e provas de atividade (Ω/E↔C).

---

## Contrato do hook (API mínima)

O runtime RAFAELIA é acionado com três pontos principais:

- `rafaelia_runtime_init()`
- `rafaelia_runtime_tick(dt_ms)` *(implementado via main-loop notifier)*
- `rafaelia_runtime_shutdown()`

Essas chamadas vivem em `hw/core/rafaelia-runtime.c` e são configuradas via
`-rafaelia` no CLI.

---

## Ponto de entrada (ψ)

O hook é conectado ao **main loop** do QEMU via `main_loop_poll_add_notifier`,
executado após cada ciclo de `main_loop_wait()`. O bootstrap ocorre em
`system/vl.c` logo após `qemu_init_main_loop()`.

---

## Observabilidade (χ)

Métricas mínimas registradas:

- **Ω_ticks_total**: contador de ciclos
- **E_entropy_last**: medida de entropia calculada por ciclo
- **C_coherence_last**: coerência atual do núcleo

Saídas:

- `trace_rafaelia_runtime_tick()` (modo `trace`)
- `error_report()` (modo `log` ou `debug=on`)
- `trace_rafaelia_runtime_shutdown()` no fechamento

---

## Ruído (ρ)

- O tick é **rate-limited** por `tick_ms`.
- Quando o VM está pausado, o intervalo é multiplicado por 5 para reduzir
  impacto em idle.
- Existe **cap** de segurança (`RAFAELIA_RUNTIME_TICK_CAP`) para evitar
  burst excessivo.

---

## Transmutação (Δ)

A cada tick, o runtime executa `rafaelia_loop_step()` e atualiza os
indicadores E↔C. Esses valores alimentam a preparação para conectores
(do Hub) em evolução futura.

---

## Memória (Σ)

O estado é guardado em `rafaelia_runtime_state_t`, que mantém:

- `rafaelia_core_t` (core vivo)
- `rafaelia_integration_hub_t` (hub inicializado)
- `ticks_total`, `entropy_last`, `coherence_last`
- `last_tick_ms`, `last_runstate`

---

## Fecho (Ω)

No shutdown do QEMU, o runtime emite o digest final via tracepoints e
finaliza o hub/core com limpeza segura.

---

## Flags / Feature Gate

CLI (QEMU):

```
-rafaelia on|off[,tick_ms=ms][,mode=MODE][,debug=on|off]
```

Modos suportados: `silent`, `log`, `trace`, `symbiosis`, `audit`, `bench`.

---

## Teste E2E mínimo

Script recomendado:

```
QEMU_BIN=./build/qemu-system-x86_64 \
  tests/rafaelia-runtime-smoke.sh
```

Esse teste valida que **Ω_ticks_total > 0** ao encerrar.

---

## Arquivos-chave

- `include/hw/core/rafaelia-runtime.h`
- `hw/core/rafaelia-runtime.c`
- `system/vl.c`
- `hw/core/trace-events`
- `docs/RAFAELIA_RUNTIME_HOOK.md`
