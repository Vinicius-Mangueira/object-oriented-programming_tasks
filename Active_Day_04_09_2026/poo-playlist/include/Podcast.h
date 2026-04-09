#pragma once
#include "Midia.h"

class Podcast : public Midia {
private:
    int episodio_;

public:
    Podcast(const std::string& titulo, const std::string& artista,
            int duracao_seg, const std::string& genero, int episodio)
        : Midia(titulo, artista, duracao_seg, genero), episodio_(episodio) {}

    std::string tipo() const override { return "Podcast"; }

    std::string descricao() const override {
        return titulo_ + " - Ep. " + std::to_string(episodio_);
    }

    void info() const override {
        Midia::info();
        std::cout << "   Episodio: " << episodio_ << "\n";
    }

    int get_episodio() const { return episodio_; }
};
