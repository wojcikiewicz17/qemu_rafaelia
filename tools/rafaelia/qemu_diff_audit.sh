#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
REPORT_DIR="$ROOT_DIR/reports"
REPORT_FILE="$REPORT_DIR/rafaelia_qemu_audit.txt"

mkdir -p "$REPORT_DIR"

cd "$ROOT_DIR"

current_commit="$(git rev-parse --short HEAD)"

modified_files="$(git diff-tree --no-commit-id --name-only -r HEAD || true)"

has_tcg_changes="NO"
has_target_arm_changes="NO"

if [[ -n "$modified_files" ]] && echo "$modified_files" | rg -q '^(accel/tcg/|tcg/)'; then
  has_tcg_changes="YES"
fi

if [[ -n "$modified_files" ]] && echo "$modified_files" | rg -q '^target/arm/'; then
  has_target_arm_changes="YES"
fi

{
  echo "RAFAELIA QEMU DIFF AUDIT"
  echo "========================"
  echo "commit: $current_commit"
  echo
  echo "modified_files_in_HEAD:"
  if [[ -n "$modified_files" ]]; then
    echo "$modified_files"
  else
    echo "(none)"
  fi
  echo
  echo "tcg_changes_in_HEAD: $has_tcg_changes"
  echo "target_arm_changes_in_HEAD: $has_target_arm_changes"
} > "$REPORT_FILE"

echo "Report generated: $REPORT_FILE"
