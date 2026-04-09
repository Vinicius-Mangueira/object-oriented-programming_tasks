#pragma once
#include <string>
#include <iostream>

class Midia {
protected:
    std::string titulo_;
    std::string artista_;
    int duracao_seg_;
    std::string genero_;

public:
    Midia(const std::string& titulo, const std::string& artista,
          int duracao_seg, const std::string& genero)
        : titulo_(titulo), artista_(artista),
          duracao_seg_(duracao_seg), genero_(genero) {}

    virtual ~Midia() = default;

    // Virtuais puras
    virtual std::string tipo() const = 0;
    virtual std::string descricao() const = 0;

    // Virtual não pura
    virtual void info() const {
        int min = duracao_seg_ / 60;
        int seg = duracao_seg_ % 60;
        std::cout << "[" << tipo() << "] " << titulo_
                  << " - " << artista_
                  << " (" << min << ":" << (seg < 10 ? "0" : "") << seg << ")"
                  << " | Gênero: " << genero_ << "\n";
    }

    // Getters
    std::string get_titulo()   const { return titulo_; }
    std::string get_artista()  const { return artista_; }
    int         get_duracao()  const { return duracao_seg_; }
    std::string get_genero()   const { return genero_; }
};
