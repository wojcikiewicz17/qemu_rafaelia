# RAFAELIA Technical Documentation (Refatorado)

**Versão:** 3.0.0  
**Data:** 2026-04-17  
**Status:** baseline técnico verificável

## 1. Escopo

Este documento consolida o contrato técnico do repositório para:

- build local reprodutível,
- execução coerente em CI,
- geração e upload de artefatos,
- rastreabilidade entre módulos de integração RAFAELIA e base QEMU.

## 2. Causa-raiz tratada nesta refatoração

A documentação anterior misturava narrativa institucional, métricas não reproduzíveis e claims sem vínculo direto com comandos de validação. Isso criava quatro riscos estruturais:

1. **Divergência doc × CI**: comandos descritos não refletiam exatamente o pipeline.
2. **Métricas sem trilha de auditoria**: ausência de procedimento para reproduzir números.
3. **Ambiguidade de fonte de verdade**: não ficava explícito se valia workflow, script local ou texto.
4. **Onboarding lento**: excesso de conteúdo conceitual e pouca sequência operacional.

## 3. Arquitetura operacional (fonte de verdade)

### 3.1 Fluxo A — QEMU smoke build (pipeline oficial)

Fonte: `.github/workflows/ci-build.yml`.

Ordem:
1. Validar sintaxe dos workflows YAML.
2. Instalar dependências de compilação.
3. Configurar QEMU para `x86_64-softmmu`.
4. Compilar alvo smoke `subprojects/libvhost-user/libvhost-user.a`.
5. Coletar artefatos.
6. Fazer upload para GitHub Actions.

### 3.2 Fluxo B — Integração RAFAELIA (módulo local)

Fonte: `hw/core/Makefile.integration` + arquivos em `hw/core/` e `include/hw/core/`.

Objetivo:
- construir componentes do hub de integração,
- executar testes de integração do módulo,
- validar regressão local sem alterar trilha de release oficial.

## 4. Contratos técnicos

### 4.1 Contrato de build

- `./configure` deve gerar `build/config-host.h` e `build/config.log`.
- `ninja -C build subprojects/libvhost-user/libvhost-user.a` deve concluir sem erro.
- `make -f hw/core/Makefile.integration all` deve concluir sem erro.

### 4.2 Contrato de artefatos

- Artefatos mínimos esperados: biblioteca smoke + logs de configuração.
- Upload obrigatório no CI via `actions/upload-artifact@v4`.
- `if-no-files-found: error` deve permanecer para falha explícita.

### 4.3 Contrato de documentação

Toda métrica publicada em docs deve indicar:
- comando de geração,
- origem do dado,
- contexto (local/CI),
- data de execução.

## 5. Matriz de execução rápida

| Objetivo | Comando | Resultado esperado |
|---|---|---|
| Validar YAML de workflow | `ruby -e "require 'yaml'; Dir['.github/workflows/*.yml'].each { |f| YAML.load_file(f) }"` | Sem exceção |
| Configurar build QEMU | `./configure --target-list=x86_64-softmmu --disable-werror --disable-docs --disable-gtk --disable-sdl --disable-curses --disable-vnc-jpeg` | Geração de `build/` |
| Compilar smoke target | `ninja -C build subprojects/libvhost-user/libvhost-user.a` | Biblioteca gerada |
| Build integração | `make -f hw/core/Makefile.integration all` | Binários/objetos gerados |
| Teste integração | `make -f hw/core/Makefile.integration test` | Suite concluída |

## 6. Limites e não-objetivos

- Este repositório **não** substitui política oficial de contribuição do mirror GitHub.
- Este documento **não** altera regras de assinatura/release externo.
- Claims de mercado/investidor só devem permanecer quando amarrados a evidência técnica rastreável.

## 7. Referências internas

- `README_RAFAELIA.md`
- `docs/RAFAELIA_PROFESSIONAL_DOSSIER.md`
- `docs/RAFAELIA_DIRECTORY_AUDIT.md`
- `.github/workflows/ci-build.yml`
- `.github/workflows/lockdown.yml`
