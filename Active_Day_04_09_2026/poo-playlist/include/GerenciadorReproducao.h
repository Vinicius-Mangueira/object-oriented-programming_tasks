#pragma once
#include "Midia.h"
#include <queue>
#include <stack>
#include <iostream>

class GerenciadorReproducao {
private:
    std::queue<Midia*> fila_;
    std::stack<Midia*> historico_;
    static int total_reproduzidas_;

    GerenciadorReproducao() = default;
    GerenciadorReproducao(const GerenciadorReproducao&) = delete;
    GerenciadorReproducao& operator=(const GerenciadorReproducao&) = delete;

public:
    static GerenciadorReproducao& instancia() {
        static GerenciadorReproducao inst;
        return inst;
    }

    void adicionar_na_fila(Midia* m) {
        fila_.push(m);
        std::cout << "[Fila] Adicionado: " << m->get_titulo() << "\n";
    }

    void reproduzir_proxima() {
        if (fila_.empty()) {
            std::cout << "[Player] Fila vazia.\n";
            return;
        }
        Midia* m = fila_.front();
        fila_.pop();
        historico_.push(m);
        ++total_reproduzidas_;
        std::cout << "[Tocando agora] ";
        m->info();
    }

    void desfazer() {
        if (historico_.empty()) {
            std::cout << "[Desfazer] Historico vazio.\n";
            return;
        }
        Midia* m = historico_.top();
        historico_.pop();
        fila_.push(m);  // re-enfileira
        std::cout << "[Desfazer] Re-enfileirado: " << m->get_titulo() << "\n";
    }

    static int total_reproduzidas() { return total_reproduzidas_; }

    bool fila_vazia()      const { return fila_.empty(); }
    bool historico_vazio() const { return historico_.empty(); }
    std::size_t tamanho_fila() const { return fila_.size(); }
};
