#pragma once
#include "Midia.h"

class Musica : public Midia {
private:
    std::string album_;

public:
    Musica(const std::string& titulo, const std::string& artista,
           int duracao_seg, const std::string& genero, const std::string& album)
        : Midia(titulo, artista, duracao_seg, genero), album_(album) {}

    std::string tipo() const override { return "Musica"; }

    std::string descricao() const override {
        return titulo_ + " (álbum: " + album_ + ")";
    }

    void info() const override {
        Midia::info();
        std::cout << "   Album: " << album_ << "\n";
    }

    std::string get_album() const { return album_; }
};
