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

log_file="$(mktemp)"

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
    -d trace \
    -D "$log_file" \
    -rafaelia enable=on,mode=1,tick_ms=10 \
    2>&1 )

if ! grep -q "RAFAELIA runtime init" "$log_file"; then
  echo "RAFAELIA runtime did not emit init output."
  cat "$log_file"
  echo "$output"
  exit 1
fi

if ! grep -q "RAFAELIA runtime tick=" "$log_file"; then
  echo "RAFAELIA runtime did not emit tick output."
  cat "$log_file"
  echo "$output"
  exit 1
fi

if ! grep -q "RAFAELIA runtime shutdown" "$log_file"; then
  echo "RAFAELIA runtime did not emit shutdown output."
  cat "$log_file"
  echo "$output"
  exit 1
fi

rm -f "$log_file"

echo "RAFAELIA runtime smoke test passed."
