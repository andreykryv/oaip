#pragma once
#include <string>

struct Expression {
    virtual double evaluate()  const = 0;
    virtual int    type_tag()  const = 0;

    virtual std::string nodeLabel()  const = 0;
    virtual Expression* leftChild()  const { return nullptr; }
    virtual Expression* rightChild() const { return nullptr; }

    virtual ~Expression() = default;
};

struct Number : Expression {
    explicit Number(double value);
    double      evaluate() const override;
    int         type_tag() const override;
    std::string nodeLabel() const override;
private:
    double m_value;
};

struct BinaryOperation : Expression {
    BinaryOperation(Expression* left, char op, Expression* right);
    double      evaluate() const override;
    int         type_tag() const override;
    std::string nodeLabel() const override;
    Expression* leftChild()  const override;
    Expression* rightChild() const override;
    ~BinaryOperation() override;
private:
    Expression* m_left;
    Expression* m_right;
    char        m_op;
};

bool check_equals(Expression const* left, Expression const* right);