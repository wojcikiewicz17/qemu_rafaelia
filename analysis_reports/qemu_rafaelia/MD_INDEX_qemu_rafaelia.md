# MD Index — QEMU Rafaelia

## Navegação
- [1. Lista ordenada de arquivos .md](#1-lista-ordenada-de-arquivos-md)
- [2. Resumo por clusters de documentação](#2-resumo-por-clusters-de-documentação)
- [3. Árvore resumida (até 3 níveis)](#3-árvore-resumida-até-3-níveis)
- [4. Top 10 arquivos mais importantes](#4-top-10-arquivos-mais-importantes)
- [5. Checklist de build/exec](#5-checklist-de-buildexec)

## 1. Lista ordenada de arquivos .md
```text
./.gitlab/issue_templates/bug.md
./.gitlab/issue_templates/feature_request.md
./INTEGRATION_ARCHITECTURE.md
./INTEGRATION_GUIDE.md
./INTEGRATION_SUMMARY.md
./QEMU_IMPROVEMENTS_README.md
./RAFAELIA_IMPLEMENTATION.md
./README_RAFAELIA.md
./android/vectras-vm-android/README.md
./docs/ERROR_HANDLING_PATTERNS.md
./docs/QEMU_PROCESS_IMPROVEMENTS.md
./docs/RAFAELIA_MODULOMR.md
./docs/RAFAELIA_RMR.md
./docs/RAFAELIA_SYMBIOSIS_MAPPING.md
./docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md
./docs/UI_UX_INTEGRATION_GUIDELINES.md
./hw/core/IMPLEMENTATION_SUMMARY.md
./hw/core/MATRIX_REFACTORING.md
./hw/core/MIGRATION_GUIDE.md
./hw/core/RAFAELIA_README.md
./hw/core/README_MATRIX.md
./hw/uefi/LIMITATIONS.md
./scripts/coverity-scan/COMPONENTS.md
./target/i386/hvf/README.md
```

## 2. Resumo por clusters de documentação
- **Documentação de integração e visão geral (raiz)**
  - `INTEGRATION_*`, `QEMU_IMPROVEMENTS_README.md`, `RAFAELIA_IMPLEMENTATION.md`, `README_RAFAELIA.md`.
- **Documentação Rafaelia (docs/)**
  - `docs/RAFAELIA_*` e `docs/UI_UX_INTEGRATION_GUIDELINES.md`.
- **Processos e padrões (docs/)**
  - `docs/ERROR_HANDLING_PATTERNS.md`, `docs/QEMU_PROCESS_IMPROVEMENTS.md`.
- **Hardware/core (hw/core/)**
  - `IMPLEMENTATION_SUMMARY.md`, `MATRIX_REFACTORING.md`, `MIGRATION_GUIDE.md`, `RAFAELIA_README.md`, `README_MATRIX.md`.
- **Plataformas e targets específicos**
  - `android/vectras-vm-android/README.md`, `hw/uefi/LIMITATIONS.md`, `target/i386/hvf/README.md`.
- **Ferramentas/segurança**
  - `scripts/coverity-scan/COMPONENTS.md`.
- **Templates de issues**
  - `.gitlab/issue_templates/*.md`.

## 3. Árvore resumida (até 3 níveis)
```text
./
  accel/
  android/
    vectras-vm-android/
      app/
      gradle/
  backends/
  block/
  docs/
    about/
    devel/
    interop/
    specs/
    system/
    tools/
    user/
  hw/
    core/
    uefi/
  include/
  migration/
  net/
  rust/
    system/
    qom/
    migration/
  target/
    i386/
    arm/
    riscv/
  tests/
  tools/
  ui/
```

## 4. Top 10 arquivos mais importantes
1. **`README.rst`** — Instruções de build e visão geral do projeto.
2. **`configure`** — Principal entry-point de configuração do build.
3. **`Makefile`** — Orquestração do build clássico.
4. **`meson.build`** — Build alternativo moderno com Meson.
5. **`docs/index.rst`** — Índice principal da documentação.
6. **`INTEGRATION_GUIDE.md`** — Guia de integração (Rafaelia).
7. **`INTEGRATION_ARCHITECTURE.md`** — Arquitetura de integração.
8. **`RAFAELIA_IMPLEMENTATION.md`** — Documenta implementação específica.
9. **`docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md`** — Doc técnica especializada.
10. **`hw/core/IMPLEMENTATION_SUMMARY.md`** — Resumo de implementação do núcleo.

## 5. Checklist de build/exec
**Baseado no README.rst**
- [ ] `mkdir build`
- [ ] `cd build`
- [ ] `../configure`
- [ ] `make`

**Notas**
- Verifique dependências específicas por target/host em `docs/` e `README.rst`.
- Para Android, usar o projeto em `android/vectras-vm-android/` via Gradle.
