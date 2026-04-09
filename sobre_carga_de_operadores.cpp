#include <iostream>     // Para cout, endl
#include <stdexcept>    // Para std::out_of_range

using std::cout;
using std::endl;
using std::ostream;

// ==========================================
// Classe Vetor2D
// Representa um vetor no plano cartesiano (x, y)
// ==========================================
class Vetor2D {
    double x, y; // Componentes do vetor

public:
    // ==========================================
    // Construtor
    // Permite criar Vetor2D com valores padrão (0,0)
    // ==========================================
    Vetor2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Métodos de acesso (getters)
    double get_x() const { return x; }
    double get_y() const { return y; }

    // ==========================================
    // operator+=
    // Soma outro vetor ao atual
    // Retorna referência para permitir encadeamento
    // Ex: a += b += c;
    // ==========================================
    Vetor2D& operator+=(const Vetor2D& other) {
        x += other.get_x(); // soma componente x
        y += other.get_y(); // soma componente y
        return *this;       // retorna o próprio objeto
    }

    // ==========================================
    // operator+
    // Soma dois vetores
    // Implementado usando operator+= (boa prática)
    // ==========================================
    Vetor2D operator+(const Vetor2D& other) const {
        Vetor2D temp = *this; // cria cópia do vetor atual
        temp += other;        // reutiliza lógica do +=
        return temp;          // retorna resultado
    }

    // ==========================================
    // operator- (binário)
    // Subtração entre vetores
    // ==========================================
    Vetor2D operator-(const Vetor2D& other) const {
        return Vetor2D(x - other.x, y - other.y);
    }

    // ==========================================
    // operator- (unário)
    // Inverte o sinal do vetor
    // Ex: -v
    // ==========================================
    Vetor2D operator-() const {
        return Vetor2D(-x, -y);
    }

    // ==========================================
    // operator*
    // Multiplicação por escalar (Vetor * double)
    // ==========================================
    Vetor2D operator*(double escalar) const {
        return Vetor2D(x * escalar, y * escalar);
    }

    // ==========================================
    // operator==
    // Verifica igualdade entre vetores
    // ==========================================
    bool operator==(const Vetor2D& other) const {
        return (x == other.x && y == other.y);
    }

    // ==========================================
    // operator!=
    // Implementado em termos de ==
    // ==========================================
    bool operator!=(const Vetor2D& other) const {
        return !(*this == other);
    }

    // ==========================================
    // operator[] (não-const)
    // Permite modificar valores
    // Ex: v[0] = 10;
    // ==========================================
    double& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;

        // Índice inválido
        throw std::out_of_range("Indice invalido!");
    }

    // ==========================================
    // operator[] (const)
    // Apenas leitura
    // Ex: double a = v[0];
    // ==========================================
    const double& operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;

        throw std::out_of_range("Indice invalido!");
    }

    // ==========================================
    // operator<< (friend)
    // Permite usar cout << vetor
    // Precisa ser friend pois o operando da esquerda é ostream
    // ==========================================
    friend ostream& operator<<(ostream& os, const Vetor2D& v);
};

// ==========================================
// Implementação do operator<<
// ==========================================
ostream& operator<<(ostream& os, const Vetor2D& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os; // permite encadeamento: cout << a << b;
}

// ==========================================
// operator* externo
// Permite: double * Vetor2D
// ==========================================
Vetor2D operator*(double escalar, const Vetor2D& v) {
    return v * escalar; // reutiliza o operador membro
}

// ==========================================
// Função main para demonstração
// ==========================================
int main() {
    Vetor2D a(3, 4);
    Vetor2D b(1, -2);

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    cout << "\na + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "-a = " << -a << endl;

    a += b;
    cout << "\na += b => " << a << endl;

    cout << "\na * 2.0 = " << a * 2.0 << endl;
    cout << "3.0 * b = " << 3.0 * b << endl;

    cout << "\na == Vetor2D(4, 2)? " 
         << (a == Vetor2D(4, 2) ? "sim" : "nao") << endl;

    cout << "a != b? " 
         << (a != b ? "sim" : "nao") << endl;

    cout << "\na[0] = " << a[0] << ", a[1] = " << a[1] << endl;

    a[0] = 10.0;
    cout << "Apos a[0] = 10: " << a << endl;

    cout << "\nEncadeamento: " << a << " e " << b << endl;

    return 0;
}

/*
=========================================================
RESPOSTA CONCEITUAL:

Por que operator<< precisa ser friend?

Porque o operando da esquerda é um objeto ostream (cout),
e não um Vetor2D.

Se fosse método membro, teria que ser algo como:
v.operator<<(cout)  -> o que NÃO faz sentido.

Logo, ele precisa ser uma função externa.

Mas como precisa acessar atributos privados (x e y),
é declarado como friend.

=========================================================
*/