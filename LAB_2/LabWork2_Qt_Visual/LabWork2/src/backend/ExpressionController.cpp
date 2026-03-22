#include "ExpressionController.h"
#include "Expression.h"
#include <QRegularExpression>
#include <stdexcept>
#include <stack>
#include <sstream>
#include <cctype>
#include <cmath>

ExpressionController::ExpressionController(QObject* parent) : QObject(parent) {}

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

// ── evaluate ─────────────────────────────────────────────────
QString ExpressionController::evaluate(const QString& expression)
{
    std::string s = expression.toStdString();
    try {
        Parser p{s};
        Expression* expr = p.parseExpr();
        double result = expr->evaluate();
        delete expr;
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

// ── checkEquals ──────────────────────────────────────────────
bool ExpressionController::checkEquals(int typeA, int typeB)
{
    Expression* left  = (typeA == 0) ? (Expression*)new Number(0)
                                     : new BinaryOperation(new Number(0), '+', new Number(0));
    Expression* right = (typeB == 0) ? (Expression*)new Number(0)
                                     : new BinaryOperation(new Number(0), '+', new Number(0));
    bool result = check_equals(left, right);
    delete left;
    delete right;
    return result;
}

// ── buildMap — ВЫШЕ parseTree ─────────────────────────────────
static QVariantMap buildMap(Expression* node) {
    if (!node) return {};
    QVariantMap m;
    m["label"] = QString::fromStdString(node->nodeLabel());
    m["isOp"]  = (node->type_tag() == 2);
    if (node->leftChild())  m["left"]  = buildMap(node->leftChild());
    if (node->rightChild()) m["right"] = buildMap(node->rightChild());
    return m;
}

// ── parseTree ────────────────────────────────────────────────
QVariantMap ExpressionController::parseTree(const QString& expression)
{
    std::string s = expression.toStdString();
    try {
        Parser p{s};
        Expression* root = p.parseExpr();
        QVariantMap result = buildMap(root);
        delete root;
        return result;
    } catch (const std::exception& e) {
        emit error(QString("Ошибка парсинга: %1").arg(e.what()));
        return {};
    }
}