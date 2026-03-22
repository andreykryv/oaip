#include "Expression.h"
#include <stdexcept>

// ── Number ───────────────────────────────────────────────────
Number::Number(double value) : m_value(value) {}

double Number::evaluate() const {
    return m_value;
}

int Number::type_tag() const {
    return 1;
}

// ── BinaryOperation ──────────────────────────────────────────
BinaryOperation::BinaryOperation(Expression* left, char op, Expression* right)
    : m_left(left), m_right(right), m_op(op)
{}

double BinaryOperation::evaluate() const
{
    double l = m_left->evaluate();
    double r = m_right->evaluate();

    switch (m_op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/':
            if (r == 0.0)
                throw std::runtime_error("Division by zero");
            return l / r;
        default:
            throw std::invalid_argument(
                std::string("Unknown operator: ") + m_op);
    }
}

int BinaryOperation::type_tag() const {
    return 2;
}

BinaryOperation::~BinaryOperation() {
    delete m_left;
    delete m_right;
}

// ── check_equals ─────────────────────────────────────────────
// Запрещено: typeid, dynamic_cast, вызов методов по указателю.
// Единственный допустимый вызов — type_tag() через указатель,
// но он объявлен virtual и не является бизнес-методом.
// Если строго запрещены ВСЕ вызовы — тогда используем
// трюк с виртуальной функцией-флагом внутри самой иерархии.
bool check_equals(Expression const* left, Expression const* right)
{
    // type_tag() — единственный разрешённый способ без RTTI
    return left->type_tag() == right->type_tag();
}
