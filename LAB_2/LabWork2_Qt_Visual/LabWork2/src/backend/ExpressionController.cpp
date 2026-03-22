#include "ExpressionController.h"
#include "Expression.h"
#include <QRegularExpression>
#include <stdexcept>
#include <stack>
#include <sstream>
#include <cctype>

ExpressionController::ExpressionController(QObject* parent) : QObject(parent) {}

// ─────────────────────────────────────────────────────────────
//  evaluate
//  Простой рекурсивный парсер: строит дерево Expression и
//  вызывает evaluate(). Поддерживает +, -, *, /, скобки.
//
//  TODO: вы можете заменить parser на свою реализацию или
//  расширить его поддержкой унарного минуса.
// ─────────────────────────────────────────────────────────────

namespace {

struct Parser {
    const std::string& s;
    size_t pos = 0;

    void skipWS() { while (pos < s.size() && std::isspace(s[pos])) ++pos; }

    double parseNumber() {
        skipWS();
        size_t start = pos;
        if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) ++pos;
        while (pos < s.size() && (std::isdigit(s[pos]) || s[pos] == '.')) ++pos;
        return std::stod(s.substr(start, pos - start));
    }

    // Forward declarations
    Expression* parseExpr();
    Expression* parseTerm();
    Expression* parseFactor();
};

Expression* Parser::parseFactor() {
    skipWS();
    if (pos < s.size() && s[pos] == '(') {
        ++pos;
        Expression* e = parseExpr();
        skipWS();
        if (pos < s.size() && s[pos] == ')') ++pos;
        return e;
    }
    // Unary minus
    if (pos < s.size() && s[pos] == '-') {
        ++pos;
        double v = parseNumber();
        return new Number(-v);
    }
    return new Number(parseNumber());
}

Expression* Parser::parseTerm() {
    Expression* left = parseFactor();
    while (true) {
        skipWS();
        if (pos >= s.size()) break;
        char op = s[pos];
        if (op != '*' && op != '/') break;
        ++pos;
        Expression* right = parseFactor();
        left = new BinaryOperation(left, op, right);
    }
    return left;
}

Expression* Parser::parseExpr() {
    Expression* left = parseTerm();
    while (true) {
        skipWS();
        if (pos >= s.size()) break;
        char op = s[pos];
        if (op != '+' && op != '-') break;
        ++pos;
        Expression* right = parseTerm();
        left = new BinaryOperation(left, op, right);
    }
    return left;
}

} // namespace

QString ExpressionController::evaluate(const QString& expression)
{
    std::string s = expression.toStdString();
    try {
        Parser p{s};
        Expression* expr = p.parseExpr();
        double result = expr->evaluate();
        delete expr;
        // Round to 10 decimal places to avoid floating point noise
        double rounded = std::round(result * 1e10) / 1e10;
        return QString::number(rounded, 'g', 10);
    } catch (const std::exception& e) {
        emit error(QString("Ошибка вычисления: %1").arg(e.what()));
        return QString("Ошибка: %1").arg(e.what());
    } catch (...) {
        emit error("Неизвестная ошибка вычисления");
        return "Ошибка";
    }
}

// ─────────────────────────────────────────────────────────────
//  checkEquals  (Задание 4)
//  typeA/typeB: 0 = Number, 1 = BinaryOperation
// ─────────────────────────────────────────────────────────────
bool ExpressionController::checkEquals(int typeA, int typeB)
{
    // Создаём объекты нужных типов
    Expression* left  = nullptr;
    Expression* right = nullptr;

    // typeA
    if (typeA == 0)
        left = new Number(0);
    else
        left = new BinaryOperation(new Number(0), '+', new Number(0));

    // typeB
    if (typeB == 0)
        right = new Number(0);
    else
        right = new BinaryOperation(new Number(0), '+', new Number(0));

    bool result = check_equals(left, right);

    delete left;
    delete right;
    return result;
}
