# C-SystemSuite

C-SystemSuite é uma suíte de ferramentas em C focada em linha de comando, com vários módulos independentes rodando sobre um único binário: shell simples, servidor HTTP, mini-banco de dados em arquivo, monitor de sistema e ferramentas de análise de arquivos.

Projeto pensado para estudo de C, sistemas e portfólio técnico.

---

## Funcionalidades

### Menu principal

Ao executar `systemsuite`, você acessa o menu raiz:

- Shell
- Servidor HTTP
- Monitor de Sistema
- Banco de Dados (mini-DB)
- Ferramentas (análise de arquivos)
- Sair

O fluxo é sempre pelo teclado e usar `0` volta para o menu anterior.

### Shell

Módulo de shell simplificado, dentro do próprio programa:

- Execução de comandos do sistema
- Histórico básico durante a sessão
- Comportamento controlado em C (sem depender de bash, zsh, etc.)

Arquivos principais:
- `shell/shell.c`
- `shell/parser.c`
- `shell/executor.c`

### Servidor HTTP

Mini servidor HTTP embutido:

- Responde em `localhost` em uma porta configurada no código
- Página HTML com card de status do servidor
- Link direto para o repositório no GitHub
- Endpoint de status em JSON

Arquivos:
- `http/server.c`
- `http/router.c`
- `http/http.h`

### Banco de Dados (mini-DB)

Um pequeno banco baseado em arquivo binário:

- Registros com: `id`, `name`, `value`, `active`
- CRUD completo:
  - Listar registros
  - Criar novo registro
  - Buscar por ID
  - Editar registro
  - Remover (marca como inativo)
- Suporte a múltiplas “tabelas”:
  - Cada tabela salva em um arquivo `csystemsuite_db_<nome>.bin`

Arquivos:
- `db/engine.c`
- `db/storage.c`
- `db/db.h`

### Monitor de Sistema

Monitor simples focado em Windows:

- Informações de sistema:
  - Número de processadores
  - Memória total e livre
  - Uptime do sistema
- Listagem de processos:
  - PID + nome do executável
  - Paginação a cada 25 linhas

Arquivos:
- `monitor/system.c`
- `monitor/process.c`
- `monitor/monitor.h`

### Ferramentas (Tools)

Coleção de utilitários de arquivo:

- Hash:
  - Cálculo de hash (FNV-1a 32-bit) de arquivos
  - Comparação de hash entre arquivos
- Compressão:
  - Compressão e descompressão simples em arquivo
- Inspeção:
  - `fileinfo`: tamanho e propriedades básicas
  - `hexdump`: dump hexadecimal de arquivos
  - `strings`: extração de strings imprimíveis com filtro e mínimo de tamanho
  - `decode`: utilitário para decodificação simples
  - `array_scan`:
    - Varredura de blocos repetidos em binários
    - Hash de blocos com FNV-1a
    - Exibição em hex e “string aproximada”
    - Filtro por texto (ex.: procurar padrões como `Player` dentro de dumps ou DLLs)

Arquivos:
- `tools/tools_main.c`
- `tools/hash.c`
- `tools/compress.c`
- `tools/decompress.c`
- `tools/fileinfo.c`
- `tools/hexdump.c`
- `tools/strings.c`
- `tools/decode.c`
- `tools/array_scan.c`
- `tools/tools.h`

---

## Arquitetura Interna

A suíte é dividida em módulos, com núcleo comum:

- `core/memory/allocator.c`  
  - Funções `mem_alloc`, `mem_realloc`, `mem_free` para centralizar alocação de memória.

- `core/utils/logger.c`  
  - Logger com timestamps e níveis (`info`, `warn`, `error`)  
  - Saída em arquivo de log (`csystemsuite.log`) ou stderr.

- `core/utils/ui.c`  
  - Funções utilitárias de interface de texto:
    - `ui_clear`, `ui_header`, `ui_wait_enter`, `ui_line_is_back`

- `core/menu/menu.c`  
  - Menu principal que orquestra Shell, HTTP, DB, Monitor e Tools.

---

## Build

Projeto pensado inicialmente para Windows com `gcc` (por exemplo, [w64devkit] ou MinGW) e `make`.

Pré-requisitos:

- `gcc`
- `make`

Clone o repositório e execute:

```bash
make
