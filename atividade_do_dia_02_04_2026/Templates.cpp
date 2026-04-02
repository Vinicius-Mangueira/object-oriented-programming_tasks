#include <iostream>
#include <stdexcept>
#include <string>
#include <array>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ostream;

// =========================================================
// CODIGO DA AULA 1 (FORNECIDO — NAO MODIFIQUE)
// =========================================================
enum class Estado { PENDENTE, EM_ANDAMENTO, CONCLUIDA };

string estadoToString(Estado e) {
    switch (e) {
        case Estado::PENDENTE:     return "PENDENTE";
        case Estado::EM_ANDAMENTO: return "EM_ANDAMENTO";
        case Estado::CONCLUIDA:    return "CONCLUIDA";
        default:                   return "DESCONHECIDO";
    }
}

class TaskException : public std::exception {
    string mensagem;
public:
    explicit TaskException(const string& msg) : mensagem(msg) {}
    const char* what() const noexcept override { return mensagem.c_str(); }
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
        : TaskException("Transicao invalida: "
            + estadoToString(a) + " -> " + estadoToString(d)),
          atual(a), destino(d) {}
    Estado getEstadoAtual()  const noexcept { return atual; }
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
    int    id;
    string titulo;
    Estado estado;
public:
    Tarefa(int id = 0, const string& titulo = "")
        : id(id), titulo(titulo), estado(Estado::PENDENTE) {}

    int    getId()     const { return id; }
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
        os << "[" << t.id << "] " << t.titulo
           << " (" << estadoToString(t.estado) << ")";
        return os;
    }
};

// =========================================================
// ITEM (a): Repositorio<T, N>
// =========================================================
template <typename T, int N>
class Repositorio {
    int quantidade;
    std::array<T, N> elementos;
public:
    Repositorio() : quantidade(0), elementos{} {}

    void adicionar(const T& item) {
        // Verifica duplicata
        for (int i = 0; i < quantidade; ++i) {
            if (elementos[i].getId() == item.getId())
                throw DuplicateTaskException(item.getId());
        }
        // Verifica capacidade
        if (quantidade >= N)
            throw std::overflow_error("Repositorio cheio");

        elementos[quantidade++] = item;
    }

    T& buscar(int id) {
        for (int i = 0; i < quantidade; ++i) {
            if (elementos[i].getId() == id)
                return elementos[i];
        }
        throw TaskNotFoundException(id);
    }

    bool remover(int id) {
        for (int i = 0; i < quantidade; ++i) {
            if (elementos[i].getId() == id) {
                // Compacta: desloca elementos subsequentes
                for (int j = i; j < quantidade - 1; ++j)
                    elementos[j] = elementos[j + 1];
                --quantidade;
                return true;
            }
        }
        throw TaskNotFoundException(id);
    }

    int getQuantidade() const { return quantidade; }

    friend ostream& operator<<(ostream& os, const Repositorio<T, N>& r) {
        for (int i = 0; i < r.quantidade; ++i)
            os << r.elementos[i] << "\n";
        return os;
    }
};

// =========================================================
// ITEM (b): Funcoes template e especializacao
// =========================================================

// Template generico
template <typename T>
string descricao(const T& item) {
    return "[" + std::to_string(item.getId()) + "] "
           + item.getTitulo()
           + " -- Estado: "
           + estadoToString(item.getEstado());
}

// Especializacao completa para int
template <>
string descricao<int>(const int& id) {
    return "ID: " + std::to_string(id);
}

// Funcao variadica com fold expression (C++17)
template <typename T, int N, typename... Args>
void adicionarVarios(Repositorio<T, N>& repo, Args&&... itens) {
    (repo.adicionar(std::forward<Args>(itens)), ...);
}

// =========================================================
// ITEM (c): Par<K, V>
// =========================================================
template <typename K, typename V>
class Par {
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

// =========================================================
// ITEM (c.3): Resposta conceitual em comentario
//
// Instanciacao IMPLICITA: ocorre automaticamente quando o
// compilador encontra o uso do template com um tipo concreto,
// sem que o programador precise escrever nada extra.
// Exemplo neste codigo:
//   Repositorio<Tarefa, 5> repo;
// O compilador gera a classe Repositorio<Tarefa,5> sozinho.
//
// Instanciacao EXPLICITA: o programador instrui o compilador
// a gerar o codigo do template para um tipo especifico,
// mesmo que nao haja uso direto naquele ponto de traducao.
// Exemplo usando o codigo deste exercicio:
//   template class Repositorio<Tarefa, 5>;
// Isso forca a geracao antecipada, util para reduzir tempo
// de compilacao em projetos grandes com multiplas translation
// units.
// =========================================================

int main() {
    // --------------------------------------------------
    // 1. Cria repositorio e adiciona 3 tarefas individualmente
    // --------------------------------------------------
    Repositorio<Tarefa, 5> repo;
    repo.adicionar(Tarefa(1, "Implementar login"));
    repo.adicionar(Tarefa(2, "Corrigir bug #42"));
    repo.adicionar(Tarefa(3, "Escrever testes"));

    cout << "=== Repositorio com 3 tarefas ===" << endl;
    cout << repo;

    // --------------------------------------------------
    // 2. adicionarVarios — adiciona mais 2 tarefas
    // --------------------------------------------------
    adicionarVarios(repo,
        Tarefa(4, "Revisar PR"),
        Tarefa(5, "Deploy staging"));

    cout << "=== Apos adicionarVarios (5 tarefas) ===" << endl;
    cout << repo;

    // --------------------------------------------------
    // 3. Busca por ID
    // --------------------------------------------------
    cout << "=== Busca por ID 3 ===" << endl;
    Tarefa& t = repo.buscar(3);
    cout << t << endl;

    // --------------------------------------------------
    // 4. Remocao por ID
    // --------------------------------------------------
    cout << "=== Remocao do ID 2 ===" << endl;
    repo.remover(2);
    cout << "Quantidade apos remocao: " << repo.getQuantidade() << endl;

    // --------------------------------------------------
    // 5. Funcao descricao — generica e especializada
    // --------------------------------------------------
    cout << "=== Funcao descricao ===" << endl;
    Tarefa t1(1, "Implementar login");
    cout << "descricao(tarefa): " << descricao(t1)  << endl;
    cout << "descricao(int): "    << descricao(42)   << endl;

    // --------------------------------------------------
    // 6. Par<K, V>
    // --------------------------------------------------
    cout << "=== Par<K, V> ===" << endl;
    Par<int, string> p1(1, "Alta");
    Par<int, string> p2(3, "Baixa");
    cout << p1 << endl;
    cout << p2 << endl;

    // --------------------------------------------------
    // 7. Tratamento de excecoes
    // --------------------------------------------------
    cout << "=== Tratamento de excecoes ===" << endl;
    try {
        repo.adicionar(Tarefa(1, "Duplicata"));
    } catch (const TaskException& e) {
        cout << "Erro: " << e.what() << endl;
    }
    try {
        repo.buscar(99);
    } catch (const TaskException& e) {
        cout << "Erro: " << e.what() << endl;
    }

    return 0;
}