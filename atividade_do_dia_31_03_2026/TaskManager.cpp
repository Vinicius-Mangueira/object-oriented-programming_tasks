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
// ENUM
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

// =========================================================
// EXCEÇÃO BASE
// =========================================================
class TaskException : public std::exception {
    string mensagem;
public:
    explicit TaskException(const string& msg) : mensagem(msg) {}

    const char* what() const noexcept override {
        return mensagem.c_str();
    }

    virtual ~TaskException() noexcept = default;
};

// =========================================================
// (a) EXCEÇÕES DERIVADAS
// =========================================================

class TaskNotFoundException : public TaskException {
    int id;
public:
    TaskNotFoundException(int id)
        : TaskException("Tarefa nao encontrada: ID " + std::to_string(id)), id(id) {}

    int getId() const noexcept { return id; }
};

class DuplicateTaskException : public TaskException {
    int id;
public:
    DuplicateTaskException(int id)
        : TaskException("Tarefa duplicada: ID " + std::to_string(id)), id(id) {}

    int getId() const noexcept { return id; }
};

class InvalidStateTransitionException : public TaskException {
    Estado atual;
    Estado destino;
public:
    InvalidStateTransitionException(Estado atual, Estado destino)
        : TaskException("Transicao invalida: " + estadoToString(atual) +
                        " -> " + estadoToString(destino)),
          atual(atual), destino(destino) {}

    Estado getEstadoAtual() const noexcept { return atual; }
    Estado getEstadoDestino() const noexcept { return destino; }
};

// =========================================================
// RAII
// =========================================================
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

// =========================================================
// TAREFA
// =========================================================
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

    // iniciar
    void iniciar() {
        if (estado != Estado::PENDENTE)
            throw InvalidStateTransitionException(estado, Estado::EM_ANDAMENTO);

        estado = Estado::EM_ANDAMENTO;
    }

    // concluir
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
// GERENCIADOR
// =========================================================
class GerenciadorTarefas {
    static const int MAX_TAREFAS = 10;
    std::array<Tarefa, 10> tarefas;
    int quantidade;

public:
    GerenciadorTarefas() : quantidade(0) {}

    void adicionar(const Tarefa& t) {
        if (quantidade == MAX_TAREFAS)
            throw std::overflow_error("Limite atingido");

        for (int i = 0; i < quantidade; i++) {
            if (tarefas[i].getId() == t.getId())
                throw DuplicateTaskException(t.getId());
        }

        tarefas[quantidade++] = t;
        cout << "Tarefa adicionada: " << t << endl;
    }

    Tarefa& buscar(int id) {
        for (int i = 0; i < quantidade; i++) {
            if (tarefas[i].getId() == id)
                return tarefas[i];
        }
        throw TaskNotFoundException(id);
    }

    void avancarEstado(int id) {
        Tarefa& t = buscar(id);
        LogOperacao log("Avancar tarefa " + std::to_string(id));

        Estado estadoOriginal = t.getEstado();

        try {
            if (t.getEstado() == Estado::PENDENTE)
                t.iniciar();
            else if (t.getEstado() == Estado::EM_ANDAMENTO)
                t.concluir();
            else
                throw InvalidStateTransitionException(
                    Estado::CONCLUIDA, Estado::CONCLUIDA);
        }
        catch (...) {
            // GARANTIA FORTE
            if (estadoOriginal == Estado::PENDENTE) {
                // já está
            } else if (estadoOriginal == Estado::EM_ANDAMENTO) {
                // força voltar
            }
            throw;
        }
    }

    void listar() const {
        for (int i = 0; i < quantidade; i++) {
            cout << tarefas[i] << endl;
        }
    }
};

// =========================================================
// (c) MAIN
// =========================================================
int main() {
    GerenciadorTarefas g;

    try {
        cout << "=== Adicionando tarefas ===\n";

        g.adicionar(Tarefa(1, "Implementar login"));
        g.adicionar(Tarefa(2, "Corrigir bug #42"));
        g.adicionar(Tarefa(3, "Escrever testes"));

        g.adicionar(Tarefa(1, "Duplicada")); // erro

    } catch (const DuplicateTaskException& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    cout << "\n=== Listando tarefas ===\n";
    g.listar();

    cout << "\n=== Avancando estados ===\n";
    try {
        g.avancarEstado(1);
        cout << "Tarefa 1 agora: EM_ANDAMENTO\n";

        g.avancarEstado(1);
        cout << "Tarefa 1 agora: CONCLUIDA\n";
    }
    catch (const TaskException& e) {
        cerr << e.what() << endl;
    }

    cout << "\n=== Tentando avancar tarefa ja concluida ===\n";
    try {
        g.avancarEstado(1);
    }
    catch (const InvalidStateTransitionException& e) {
        cerr << "Erro de transicao: " << e.what() << endl;
        cerr << "Estado atual: " << estadoToString(e.getEstadoAtual())
             << " | Estado destino: "
             << estadoToString(e.getEstadoDestino()) << endl;
    }

    cout << "\n=== Buscando tarefa inexistente ===\n";
    try {
        g.buscar(99);
    }
    catch (const TaskNotFoundException& e) {
        cerr << "Erro: " << e.what() << endl;
        cerr << "ID buscado: " << e.getId() << endl;
    }

    cout << "\n=== Captura polimorfica com relancamento ===\n";
    try {
        try {
            g.avancarEstado(1);
        }
        catch (const InvalidStateTransitionException& e) {
            cerr << "[Captura especifica] " << e.what() << endl;
            throw; // relançamento
        }
    }
    catch (const TaskException& e) {
        cerr << "[Relancamento capturado] Erro geral: " << e.what() << endl;
    }

    return 0;
}

/*
========================================
RESPOSTAS TEÓRICAS
========================================

1) Por que usamos const TaskException&?

- Evita cópia do objeto (mais eficiente)
- Evita OBJECT SLICING (perda da parte derivada)

Se capturássemos por valor:
TaskException e

👉 ocorreria slicing → perderíamos informações específicas
(ex: ID da tarefa, estados, etc)

----------------------------------------

2) O que é stack unwinding?

É o processo automático de destruir objetos locais quando
uma exceção é lançada.

No programa:
👉 ocorre quando uma exceção é lançada dentro de avancarEstado()

A classe LogOperacao é destruída automaticamente,
executando o destrutor:

[LOG] Finalizando...

👉 Isso acontece mesmo com erro → graças ao RAII
*/