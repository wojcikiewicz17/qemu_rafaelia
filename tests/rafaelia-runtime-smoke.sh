#!/usr/bin/env bash
set -euo pipefail

if [[ -z "${QEMU_BIN:-}" ]]; then
  if [[ -x "./build/qemu-system-x86_64" ]]; then
    QEMU_BIN="./build/qemu-system-x86_64"
  elif command -v qemu-system-x86_64 >/dev/null 2>&1; then
    QEMU_BIN="$(command -v qemu-system-x86_64)"
  fi
fi

if [[ -z "${QEMU_BIN:-}" || ! -x "$QEMU_BIN" ]]; then
  echo "QEMU_BIN is not set or executable; set QEMU_BIN to your qemu-system-x86_64 binary."
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
