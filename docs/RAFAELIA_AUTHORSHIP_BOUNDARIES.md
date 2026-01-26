# RAFAELIA Authorship Boundaries and Framework Layers

> **Objetivo**: separar claramente o que é autoria RAFAELIA do que permanece
> upstream QEMU, organizando o framework em camadas e módulos com rastreio de
> licenças e normas técnicas.

---

## 1. Princípios de separação de autoria

1. **QEMU Upstream permanece soberano**
   - Arquivos e diretórios não listados explicitamente como RAFAELIA seguem o
     histórico, licenças e convenções do QEMU original.
2. **Autoria RAFAELIA é delimitada**
   - Apenas arquivos declarados no ModulomR são tratados como autoria RAFAELIA.
3. **Rastreabilidade formal**
   - Qualquer alteração RAFAELIA deve atualizar o ModulomR.
4. **Compatibilidade de licenças**
   - Nenhuma licença é substituída: toda mudança segue os headers e os arquivos
     `COPYING` e `COPYING.LIB`.

---

## 2. Mapa de camadas do framework

| Camada | Nome | Objetivo | Escopo típico |
|--------|------|----------|---------------|
| L0 | **QEMU Core** | Base upstream de emulação | TODO o restante do repositório |
| L1 | **Integration Hub** | Integração multi-repo e roteamento ético | `include/hw/core/rafaelia-integration.h` |
| L2 | **RAFAELIA Core** | Fórmulas e bootblock | `include/hw/core/rafaelia-core.h`, `hw/core/rafaelia-core.c` |
| L3 | **Runtime Optimization** | RMR e helpers low-level | `include/hw/core/rafaelia-rmr*.h`, `hw/core/rafaelia-rmr*.c` |
| L4 | **MVP Modules** | Módulos e agregações | `include/hw/core/rafaelia-mvp-modules.h` |
| L5 | **Governança & Documentação** | Normas técnicas, autoria, compliance | `docs/RAFAELIA_*.md`, `README_RAFAELIA.md` |

> **Nota**: L0 é intocável em termos de autoria RAFAELIA. Qualquer extensão
> deve ocorrer nas camadas L1–L5, com registro no ModulomR.

---

## 3. Mapa de módulos RAFAELIA (autoria delimitada)

Os módulos abaixo são considerados **RAFAELIA authored** e devem estar no
**ModulomR**:

- **RAFAELIA_BOOTBLOCK_v1** → `hw/core/rafaelia-core.*`
- **RAFAELIA_INTEGRATION** → `include/hw/core/rafaelia-integration.h`
- **RAFAELIA_RMR** → `include/hw/core/rafaelia-rmr*.h`, `hw/core/rafaelia-rmr*.c`
- **RAFAELIA_MVP_MODULES** → `include/hw/core/rafaelia-mvp-modules.h`
- **RAFAELIA_MODULOMR** → `include/hw/core/rafaelia-modulomr.h`, `hw/core/rafaelia-modulomr.c`
- **Governança & Docs** → `docs/RAFAELIA_*.md`, `README_RAFAELIA.md`

---

## 4. Normas técnicas e compliance

- **ISO 9001/8000**: diretrizes de qualidade documentadas em
  `docs/UI_UX_INTEGRATION_GUIDELINES.md`.
- **Rastreabilidade**: referência cruzada entre documentação técnica e código.
- **Controle de mudanças**: toda mudança de autoria RAFAELIA atualiza o
  ModulomR, com resumo e escopo claro.

---

## 5. Checklist de reorganização (para cada mudança)

1. **Identificar camada** (L1–L5) e módulo afetado.
2. **Garantir separação** com escopo listado no ModulomR.
3. **Atualizar documentação** (link de referência no README_RAFAELIA).
4. **Verificar licenças** (headers e arquivos raiz).

---

## 6. Referências obrigatórias

- ModulomR: `docs/RAFAELIA_MODULOMR.md`
- README principal: `README_RAFAELIA.md`
- Documentação técnica: `docs/RAFAELIA_TECHNICAL_DOCUMENTATION.md`

