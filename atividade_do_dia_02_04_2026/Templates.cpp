#include <array>
#include <forward_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;

// =========================================================
// CODIGO DA AULA 1 (FORNECIDO — NAO MODIFIQUE)
// =========================================================
enum class Estado { PENDENTE, EM_ANDAMENTO, CONCLUIDA };

string estadoToString(Estado e) {
    switch (e) {
        case Estado::PENDENTE: return "PENDENTE";
        case Estado::EM_ANDAMENTO: return "EM_ANDAMENTO";
        case Estado::CONCLUIDA: return "CONCLUIDA";
        default: return "DESCONHECIDO";
    }
}

class TaskException : public std::exception {
    string mensagem;

public:
    explicit TaskException(const string& msg) : mensagem(msg) {}

    const char* what() const noexcept override {
        return mensagem.c_str();
    }

    virtual ~TaskException() noexcept = default;
};

class TaskNotFoundException : public TaskException {
    int id;

public:
    explicit TaskNotFoundException(int id)
        : TaskException("Tarefa nao encontrada: ID " + std::to_string(id)), id(id) {}

    int getId() const noexcept { return id; }
};

class DuplicateTaskException : public TaskException {
    int id;

public:
    explicit DuplicateTaskException(int id)
        : TaskException("Tarefa duplicada: ID " + std::to_string(id)), id(id) {}

    int getId() const noexcept { return id; }
};

class InvalidStateTransitionException : public TaskException {
    Estado atual, destino;

public:
    InvalidStateTransitionException(Estado a, Estado d)
        : TaskException("Transicao invalida: " + estadoToString(a) + " -> " + estadoToString(d)),
          atual(a),
          destino(d) {}

    Estado getEstadoAtual() const noexcept { return atual; }
    Estado getEstadoDestino() const noexcept { return destino; }
};

class LogOperacao {
    string operacao;

public:
    explicit LogOperacao(const string& op) : operacao(op) {
        cout << "[LOG] Iniciando: " << operacao << endl;
    }

    ~LogOperacao() {
        cout << "[LOG] Finalizando: " << operacao << endl;
    }
};

class Tarefa {
    int id;
    string titulo;
    Estado estado;

public:
    Tarefa(int id = 0, const string& titulo = "")
        : id(id), titulo(titulo), estado(Estado::PENDENTE) {}

    int getId() const { return id; }
    string getTitulo() const { return titulo; }
    Estado getEstado() const { return estado; }

    void iniciar() {
        if (estado != Estado::PENDENTE)
            throw InvalidStateTransitionException(estado, Estado::EM_ANDAMENTO);
        estado = Estado::EM_ANDAMENTO;
    }

    void concluir() {
        if (estado != Estado::EM_ANDAMENTO)
            throw InvalidStateTransitionException(estado, Estado::CONCLUIDA);
        estado = Estado::CONCLUIDA;
    }

    friend ostream& operator<<(ostream& os, const Tarefa& t) {
        os << "[" << t.id << "] " << t.titulo << " (" << estadoToString(t.estado) << ")";
        return os;
    }
};

// =========================================================
// ITEM (a): IMPLEMENTE Repositorio<T, N>
// =========================================================
template <typename T, int N>
class Repositorio {
private:
    std::array<T, N> elementos;
    int quantidade;

public:
    Repositorio() : quantidade(0) {}

    void adicionar(const T& item) {
        if (quantidade >= N) {
            throw std::overflow_error("Repositorio cheio");
        }

        for (int i = 0; i < quantidade; i++) {
            if (elementos[i].getId() == item.getId()) {
                throw DuplicateTaskException(item.getId());
            }
        }

        elementos[quantidade] = item;
        quantidade++;
    }

    T& buscar(int id) {
        for (int i = 0; i < quantidade; i++) {
            if (elementos[i].getId() == id) {
                return elementos[i];
            }
        }
        throw TaskNotFoundException(id);
    }

    bool remover(int id) {
        int posicao = -1;

        for (int i = 0; i < quantidade; i++) {
            if (elementos[i].getId() == id) {
                posicao = i;
                break;
            }
        }

        if (posicao == -1) {
            throw TaskNotFoundException(id);
        }

        for (int i = posicao; i < quantidade - 1; i++) {
            elementos[i] = elementos[i + 1];
        }

        quantidade--;
        return true;
    }

    int getQuantidade() const {
        return quantidade;
    }

    friend ostream& operator<<(ostream& os, const Repositorio<T, N>& repo) {
        for (int i = 0; i < repo.quantidade; i++) {
            os << repo.elementos[i] << '\n';
        }
        return os;
    }
};

// =========================================================
// ITEM (b): IMPLEMENTE funcoes template
// =========================================================
template <typename T>
string descricao(const T& item) {
    return "[" + std::to_string(item.getId()) + "] " +
           item.getTitulo() + " -- Estado: " +
           estadoToString(item.getEstado());
}

template <>
string descricao<int>(const int& id) {
    return "ID: " + std::to_string(id);
}

template <typename T, int N, typename... Args>
void adicionarVarios(Repositorio<T, N>& repo, Args&&... itens) {
    (repo.adicionar(std::forward<Args>(itens)), ...);
}

// =========================================================
// ITEM (c): IMPLEMENTE Par<K, V>
// =========================================================
template <typename K, typename V>
class Par {
private:
    K chave;
    V valor;

public:
    Par(K c, V v) : chave(c), valor(v) {}

    K getChave() const { return chave; }
    V getValor() const { return valor; }

    friend ostream& operator<<(ostream& os, const Par<K, V>& p) {
        os << p.chave << ": " << p.valor;
        return os;
    }
};

/*
Qual a diferenca entre instanciacao implicita e explicita de templates?

Instanciacao implicita:
acontece quando o compilador deduz automaticamente o tipo
a partir do uso. Exemplo neste codigo:
    Repositorio<Tarefa, 5> repo;
    descricao(repo.buscar(1));
Aqui o compilador instancia os templates porque eles foram usados.

Instanciacao explicita:
acontece quando o programador informa manualmente qual versao
do template deve ser gerada. Exemplo:
    descricao<int>(42);
Nesse caso eu disse explicitamente que quero a versao de descricao
para int.

Outro exemplo de escrita explicita seria:
    template class Repositorio<Tarefa, 5>;
que forca a geracao dessa instancia do template.
*/

// =========================================================
// MAIN
// =========================================================
int main() {
    try {
        Repositorio<Tarefa, 5> repo;

        repo.adicionar(Tarefa(1, "Implementar login"));
        repo.adicionar(Tarefa(2, "Corrigir bug #42"));
        repo.adicionar(Tarefa(3, "Escrever testes"));

        cout << "=== Repositorio com 3 tarefas ===" << endl;
        cout << repo;

        adicionarVarios(
            repo,
            Tarefa(4, "Revisar PR"),
            Tarefa(5, "Deploy staging")
        );

        cout << "=== Apos adicionarVarios (5 tarefas) ===" << endl;
        cout << repo;

        cout << "=== Busca por ID 3 ===" << endl;
        Tarefa& encontrada = repo.buscar(3);
        cout << encontrada << endl;

        cout << "=== Remocao do ID 2 ===" << endl;
        repo.remover(2);
        cout << "Quantidade apos remocao: " << repo.getQuantidade() << endl;

        cout << "=== Funcao descricao ===" << endl;
        cout << "descricao(tarefa): " << descricao(repo.buscar(1)) << endl;
        cout << "descricao(int): " << descricao<int>(42) << endl;

        cout << "=== Par<K, V> ===" << endl;
        Par<int, string> p1(1, "Alta");
        Par<int, string> p2(3, "Baixa");
        cout << p1 << endl;
        cout << p2 << endl;

        cout << "=== Tratamento de excecoes ===" << endl;

        try {
            repo.adicionar(Tarefa(1, "Duplicada"));
        } catch (const TaskException& e) {
            cerr << "Erro: " << e.what() << endl;
        }

        try {
            repo.buscar(99);
        } catch (const TaskException& e) {
            cerr << "Erro: " << e.what() << endl;
        }

    } catch (const std::exception& e) {
        cerr << "Erro geral: " << e.what() << endl;
    }

    return 0;
}