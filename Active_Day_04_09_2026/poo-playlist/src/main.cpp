#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <algorithm>
#include <string>

#include "Midia.h"
#include "Musica.h"
#include "Podcast.h"
#include "GerenciadorReproducao.h"
#include "Catalogo.h"

static void separador(const std::string& titulo) {
    std::cout << "\n==============================\n";
    std::cout << "  " << titulo << "\n";
    std::cout << "==============================\n";
}

int main() {
    // ========================================================
    // MÓDULO 1 — Hierarquia + vector + sort + find_if
    // ========================================================
    separador("MODULO 1: vector + sort + find_if");

    // 1. Criar mídias
    Musica  m1("Bohemian Rhapsody", "Queen",        354, "Rock",      "A Night at the Opera");
    Musica  m2("Blinding Lights",   "The Weeknd",   200, "Pop",       "After Hours");
    Musica  m3("Back in Black",     "AC/DC",        255, "Rock",      "Back in Black");
    Musica  m4("Lose Yourself",     "Eminem",       326, "Hip-Hop",   "8 Mile");
    Musica  m5("Shape of You",      "Ed Sheeran",   114, "Pop",       "Divide");
    Podcast p1("Lex Fridman #400",  "Lex Fridman",  7200,"Tecnologia",400);
    Podcast p2("Cortes Flow #88",   "Flow Podcast", 110, "Entretenimento", 88);

    std::vector<Midia*> playlist = { &m1, &m2, &m3, &m4, &m5, &p1, &p2 };

    // 2. Ordenar por duração crescente
    std::sort(playlist.begin(), playlist.end(),
        [](Midia* a, Midia* b) { return a->get_duracao() < b->get_duracao(); });

    std::cout << "\n--- Playlist ordenada por duracao ---\n";
    for (Midia* m : playlist) {
        m->info();
    }

    // 3. Buscar por título
    std::string busca = "Lose Yourself";
    auto it = std::find_if(playlist.begin(), playlist.end(),
        [&busca](Midia* m) { return m->get_titulo() == busca; });

    std::cout << "\n--- Busca por \"" << busca << "\" ---\n";
    if (it != playlist.end())
        std::cout << "Encontrado: " << (*it)->descricao() << "\n";
    else
        std::cout << "Nao encontrado.\n";

    // ========================================================
    // MÓDULO 2 — map + set + iteradores
    // ========================================================
    separador("MODULO 2: map + set");

    // 5. Indexar por gênero
    std::map<std::string, std::vector<Midia*>> por_genero;
    for (Midia* m : playlist)
        por_genero[m->get_genero()].push_back(m);

    // 6. Gêneros únicos com set
    std::set<std::string> generos_unicos;
    for (Midia* m : playlist)
        generos_unicos.insert(m->get_genero());

    std::cout << "\n--- Generos unicos no acervo ---\n";
    for (const auto& g : generos_unicos)
        std::cout << "  * " << g << "\n";

    // 7. Iterar pelo map com structured binding
    std::cout << "\n--- Midias por genero ---\n";
    for (auto& [genero, midias] : por_genero) {
        std::cout << "[" << genero << "]\n";
        for (Midia* m : midias)
            std::cout << "  - " << m->get_titulo() << " (" << m->tipo() << ")\n";
    }

    // 8. Buscar gênero específico
    std::string genero_busca = "Rock";
    std::cout << "\n--- Busca pelo genero \"" << genero_busca << "\" ---\n";
    auto git = por_genero.find(genero_busca);
    if (git != por_genero.end()) {
        std::cout << "Genero encontrado com " << git->second.size() << " midia(s):\n";
        for (Midia* m : git->second)
            std::cout << "  - " << m->get_titulo() << "\n";
    } else {
        std::cout << "Genero nao encontrado.\n";
    }

    // ========================================================
    // MÓDULO 3 — queue + stack + Singleton
    // ========================================================
    separador("MODULO 3: GerenciadorReproducao (Singleton)");

    GerenciadorReproducao& player = GerenciadorReproducao::instancia();

    player.adicionar_na_fila(&m1);
    player.adicionar_na_fila(&m2);
    player.adicionar_na_fila(&p1);

    std::cout << "\n--- Reproduzindo ---\n";
    player.reproduzir_proxima();
    player.reproduzir_proxima();

    std::cout << "\n--- Desfazer ultima reproducao ---\n";
    player.desfazer();

    std::cout << "\n--- Continuar reproducao ---\n";
    player.reproduzir_proxima();
    player.reproduzir_proxima();

    std::cout << "\nTotal reproduzidas: " << GerenciadorReproducao::total_reproduzidas() << "\n";

    // ========================================================
    // MÓDULO 4 — Algoritmos STL
    // ========================================================
    separador("MODULO 4: Algoritmos STL");

    // Recriar vetor original (sort anterior mudou a ordem)
    std::vector<Midia*> acervo = { &m1, &m2, &m3, &m4, &m5, &p1, &p2 };

    // 9. accumulate — duração total
    int total_seg = std::accumulate(acervo.begin(), acervo.end(), 0,
        [](int soma, Midia* m) { return soma + m->get_duracao(); });
    std::cout << "\nDuracao total: "
              << total_seg / 60 << "min " << total_seg % 60 << "s\n";

    // 10. count_if — mais de 3 minutos
    int longas = std::count_if(acervo.begin(), acervo.end(),
        [](Midia* m) { return m->get_duracao() > 180; });
    std::cout << "Midias com mais de 3 minutos: " << longas << "\n";

    // 11. transform — títulos formatados
    std::vector<std::string> titulos_fmt(acervo.size());
    std::transform(acervo.begin(), acervo.end(), titulos_fmt.begin(),
        [](Midia* m) { return m->get_titulo() + " - " + m->get_artista(); });

    std::cout << "\n--- Titulos formatados ---\n";
    for (const auto& t : titulos_fmt)
        std::cout << "  " << t << "\n";

    // 12. erase-remove — remover mídias < 2 minutos (120s)
    acervo.erase(
        std::remove_if(acervo.begin(), acervo.end(),
            [](Midia* m) { return m->get_duracao() < 120; }),
        acervo.end());

    std::cout << "\n--- Playlist apos remover midias < 2 min ---\n";
    for (Midia* m : acervo)
        std::cout << "  " << m->get_titulo()
                  << " (" << m->get_duracao() << "s)\n";

    // ========================================================
    // MÓDULO 5 — Template Catalogo<T>
    // ========================================================
    separador("MODULO 5: Catalogo<T> template");

    Catalogo<Musica>  cat_musicas;
    Catalogo<Podcast> cat_podcasts;

    // 13. Popular catálogos
    cat_musicas.adicionar(&m1);
    cat_musicas.adicionar(&m2);
    cat_musicas.adicionar(&m3);
    cat_musicas.adicionar(&m4);
    cat_musicas.adicionar(&m5);

    cat_podcasts.adicionar(&p1);
    cat_podcasts.adicionar(&p2);

    // 14. Ordenar e listar
    std::cout << "\n--- Catalogo de Musicas (ordenado por duracao) ---\n";
    cat_musicas.ordenar_por_duracao();
    cat_musicas.listar();

    std::cout << "\n--- Catalogo de Podcasts ---\n";
    cat_podcasts.listar();

    // Busca no catálogo
    std::string titulo_busca = "Back in Black";
    Musica* encontrada = cat_musicas.buscar(titulo_busca);
    std::cout << "\n--- Busca no catalogo: \"" << titulo_busca << "\" ---\n";
    if (encontrada)
        std::cout << "Encontrada: " << encontrada->descricao() << "\n";
    else
        std::cout << "Nao encontrada.\n";

    // 15. Integração: popular fila a partir do catálogo
    std::cout << "\n--- Integracao: fila de reproducao do catalogo ---\n";
    for (Musica* mus : cat_musicas.get_itens())
        player.adicionar_na_fila(mus);

    std::cout << "\n--- Reproduzindo toda a fila ---\n";
    while (!player.fila_vazia())
        player.reproduzir_proxima();

    std::cout << "\nEstatisticas finais:\n";
    std::cout << "  Total reproduzidas na sessao: "
              << GerenciadorReproducao::total_reproduzidas() << "\n";
    std::cout << "  Musicas no catalogo: " << cat_musicas.tamanho() << "\n";
    std::cout << "  Podcasts no catalogo: " << cat_podcasts.tamanho() << "\n";

    return 0;
}
