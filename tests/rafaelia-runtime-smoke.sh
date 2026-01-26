#!/usr/bin/env bash
set -euo pipefail

if [[ -z "${QEMU_BIN:-}" ]]; then
  echo "QEMU_BIN is not set; skipping RAFAELIA runtime smoke test."
  exit 0
fi

if [[ ! -x "$QEMU_BIN" ]]; then
  echo "QEMU_BIN is not executable: $QEMU_BIN"
  exit 1
fi

output=$( {
  sleep 0.2
  printf 'quit\n'
} | timeout 5 "$QEMU_BIN" \
    -machine none \
    -accel tcg \
    -display none \
    -nographic \
    -nodefaults \
    -monitor stdio \
    -no-reboot \
    -no-shutdown \
    -rafaelia on,mode=log,tick_ms=10 \
    2>&1 )

if ! grep -q "RAFAELIA runtime tick=" <<<"$output"; then
  echo "RAFAELIA runtime did not emit tick output."
  echo "$output"
  exit 1
fi

echo "RAFAELIA runtime smoke test passed."
