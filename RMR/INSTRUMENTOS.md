# RMR Instrumentos de Sistema (QEMU)

Este documento separa os **instrumentos de telemetria low-level** adicionados ao
módulo RMR, com foco em coleta determinística de parâmetros do host para guiar
rotas de execução com menor atrito de hardware.

## Snapshot coletado

API: `rafaelia_rmr_collect_instruments()`

Campos coletados:
- arquitetura (`arch`)
- sistema operacional (`os`)
- release de kernel (`kernel_release`)
- máquina/ISA reportada (`machine`)
- largura de ponteiro (`pointer_bits`)
- CPUs online (`cpu_online`)
- tamanho de página (`page_bytes`)
- uptime (`uptime_seconds`)
- RAM total/livre em KiB (`total_ram_kib`, `free_ram_kib`)
- disponibilidade de aceleração KVM (`has_kvm_accel`)

## Princípios

- caminho único, sem alocação dinâmica
- inicialização explícita de toda a estrutura
- compatível com uso em inicialização precoce
- sem dependências externas adicionais

## Arquivos

- Header público: `include/hw/core/rafaelia-rmr.h`
- Implementação: `hw/core/rafaelia-rmr.c`
- Testes: `tests/unit/test-rafaelia-rmr-instruments.c`
