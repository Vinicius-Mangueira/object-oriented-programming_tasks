# POO Playlist — Sistema de Gestão de Playlist

Exercício Prático de Programação Orientada a Objetos (POO) — UFPB 2025.2

## Descrição

Sistema de streaming de música que gerencia mídias (músicas e podcasts), organiza playlists por gênero, mantém fila de reprodução e histórico, e gera estatísticas. Implementado em 5 módulos progressivos usando containers e algoritmos da STL do C++17.

## Estrutura do Projeto

```
poo-playlist/
├── include/
│   ├── Midia.h                  # Classe base abstrata
│   ├── Musica.h                 # Herda de Midia
│   ├── Podcast.h                # Herda de Midia
│   ├── GerenciadorReproducao.h  # Singleton com queue e stack
│   └── Catalogo.h               # Template Catalogo<T>
├── src/
│   ├── main.cpp                 # Demonstração dos 5 módulos
│   └── GerenciadorReproducao.cpp
├── docs/
│   └── llm-log.md
├── Makefile
└── README.md
```

## Compilação e Execução

```bash
make        # compila
make run    # compila e executa
make clean  # remove binário
```

Requer: g++ com suporte a C++17.

## Módulos Implementados

### Módulo 1 — Hierarquia + vector + sort + find_if
- `Midia`: classe base abstrata com virtuais puras `tipo()` e `descricao()`, virtual não pura `info()`, destrutor virtual.
- `Musica` e `Podcast`: herdam publicamente, sobrescrevem `info()` chamando a base.
- `vector<Midia*>` com `std::sort` por lambda (duração crescente) e `std::find_if` por título.

### Módulo 2 — map + set + iteradores
- `map<string, vector<Midia*>>` indexando mídias por gênero.
- `set<string>` acumulando gêneros únicos.
- Iteração com structured binding `auto& [chave, valor]` e `map::find()`.

### Módulo 3 — queue + stack + Singleton
- `GerenciadorReproducao`: Singleton com construtor privado, cópia deletada e lazy init.
- `queue<Midia*>` para fila FIFO de reprodução.
- `stack<Midia*>` para histórico LIFO.
- `static int total_reproduzidas_` como contador de reproduções.

### Módulo 4 — Algoritmos STL
- `std::accumulate`: duração total da playlist.
- `std::count_if`: contagem de mídias com mais de 3 minutos.
- `std::transform`: geração de títulos formatados `"Titulo - Artista"`.
- Erase-remove idiom: remoção de mídias com menos de 2 minutos.

### Módulo 5 — Template Catalogo<T>
- `Catalogo<T>` encapsula `vector<T*>` com `adicionar`, `buscar`, `listar`, `ordenar_por_duracao` e `tamanho`.
- Template inteiramente no header (requisito C++).
- Instanciado como `Catalogo<Musica>` e `Catalogo<Podcast>`.
- Integrado com `GerenciadorReproducao` para popular e reproduzir a fila.

## Decisões de Design

- Getters retornam por valor (`string`, `int`) para simplicidade e segurança.
- `GerenciadorReproducao::desfazer()` re-enfileira a mídia no final (comportamento de "voltar"), sem modificar o ponteiro do topo da stack antes de remover.
- O template `Catalogo<T>` usa `T*` para compatibilidade com polimorfismo e consistência com o restante do sistema.
- Nenhum `new`/`delete` manual: todos os objetos vivem na stack do `main`, eliminando risco de vazamento.
