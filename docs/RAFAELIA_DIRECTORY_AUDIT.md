# RAFAELIA Directory Audit (Checked Paths)

## Escopo

Este registro lista os diretórios auditados para atualização de roadmap e controle de cobertura documental/técnica.

## Diretórios verificados

| Diretório | Categoria | Evidência de conteúdo | Resultado |
|-----------|-----------|------------------------|-----------|
| `.` | Raiz do projeto | Presença de `Makefile`, `README_RAFAELIA.md`, docs e código-fonte principal | ✅ OK |
| `.github/workflows` | CI/CD | Pipelines e automações de validação | ✅ OK |
| `analysis_reports/qemu_rafaelia` | Relatórios | Área para resultados analíticos e consolidado técnico | ✅ OK |
| `docs` | Documentação | Hub de documentação RAFAELIA + docs upstream | ✅ OK |
| `hw/core` | Núcleo de integração | Módulos centrais RAFAELIA e integração com QEMU | ✅ OK |
| `include` | Headers públicos/internos | Contratos de API e interfaces internas | ✅ OK |
| `linux-user` | User-mode emulação | Estruturas por arquitetura e syscalls | ✅ OK |
| `migration` | Migração/estado | Módulos de migração e transporte de estado | ✅ OK |
| `scripts` | Ferramental operacional | Scripts de build/test/performance/infra | ✅ OK |
| `target` | Arquiteturas alvo | Implementações por ISA suportada | ✅ OK |
| `tests` | Qualidade e validação | Test suites (unit/functional/tcg etc.) | ✅ OK |
| `util` | Utilitários de base | Implementações utilitárias de suporte | ✅ OK |

## Observações de consistência

- A árvore auditada cobre documentação, núcleo de integração, runtime, testes e infraestrutura.
- O estado atual permite evoluir o roadmap com foco em release policy, compatibilidade e evidências automatizadas.
