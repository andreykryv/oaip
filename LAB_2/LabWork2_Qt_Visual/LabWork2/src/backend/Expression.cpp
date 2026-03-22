#include "Expression.h"
#include <stdexcept>
#include <string>

// Number
Number::Number(double value) : m_value(value) {}
double Number::evaluate()        const { return m_value; }
int    Number::type_tag()        const { return 1; }
std::string Number::nodeLabel()  const { return std::to_string(m_value); }

// BinaryOperation
BinaryOperation::BinaryOperation(Expression* left, char op, Expression* right)
    : m_left(left), m_right(right), m_op(op) {}

double BinaryOperation::evaluate() const {
    double l = m_left->evaluate(), r = m_right->evaluate();
    switch (m_op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/':
            if (r == 0.0) throw std::runtime_error("Division by zero");
            return l / r;
        default:
            throw std::invalid_argument(std::string("Unknown op: ") + m_op);
    }
}
int         BinaryOperation::type_tag()    const { return 2; }
std::string BinaryOperation::nodeLabel()   const { return std::string(1, m_op); }
Expression* BinaryOperation::leftChild()   const { return m_left;  }
Expression* BinaryOperation::rightChild()  const { return m_right; }
BinaryOperation::~BinaryOperation() { delete m_left; delete m_right; }

bool check_equals(Expression const* left, Expression const* right) {
    return left->type_tag() == right->type_tag();
}