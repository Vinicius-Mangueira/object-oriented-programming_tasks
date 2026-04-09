#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

template<typename T>
class Catalogo {
private:
    std::vector<T*> itens_;

public:
    void adicionar(T* item) {
        itens_.push_back(item);
    }

    T* buscar(const std::string& titulo) const {
        auto it = std::find_if(itens_.begin(), itens_.end(),
            [&titulo](T* item) { return item->get_titulo() == titulo; });
        return (it != itens_.end()) ? *it : nullptr;
    }

    void listar() const {
        if (itens_.empty()) {
            std::cout << "  (catalogo vazio)\n";
            return;
        }
        for (const auto* item : itens_) {
            item->info();
        }
    }

    void ordenar_por_duracao() {
        std::sort(itens_.begin(), itens_.end(),
            [](T* a, T* b) { return a->get_duracao() < b->get_duracao(); });
    }

    std::size_t tamanho() const { return itens_.size(); }

    const std::vector<T*>& get_itens() const { return itens_; }
};
