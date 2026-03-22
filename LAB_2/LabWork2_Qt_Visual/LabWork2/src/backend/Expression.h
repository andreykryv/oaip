#pragma once

// ─────────────────────────────────────────────────────────────
//  Задание 3 — Иерархия арифметических выражений
//  Задание 4 — check_equals без typeid / dynamic_cast
// ─────────────────────────────────────────────────────────────

// ── Base ─────────────────────────────────────────────────────
struct Expression {
    virtual double evaluate()  const = 0;

    // Задание 4: уникальный тег класса (без typeid/dynamic_cast)
    virtual int    type_tag()  const = 0;

    virtual ~Expression() = default;
};

// ── Number ───────────────────────────────────────────────────
struct Number : Expression {
    explicit Number(double value);

    double evaluate() const override;
    int    type_tag() const override;   // returns 1

private:
    double m_value;
};

// ── BinaryOperation ──────────────────────────────────────────
struct BinaryOperation : Expression {
    // Takes ownership of left and right
    BinaryOperation(Expression* left, char op, Expression* right);

    double evaluate() const override;
    int    type_tag() const override;   // returns 2

    ~BinaryOperation() override;   // deletes left and right

private:
    Expression* m_left;
    Expression* m_right;
    char        m_op;
};

// ─────────────────────────────────────────────────────────────
//  Задание 4 — check_equals
//  Запрещено: typeid, dynamic_cast, вызов методов по указателю
//  Разрешено: виртуальный type_tag()
// ─────────────────────────────────────────────────────────────
bool check_equals(Expression const* left, Expression const* right);
