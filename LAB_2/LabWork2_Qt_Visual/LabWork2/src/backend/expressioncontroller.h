#pragma once
// expressioncontroller.h
// ─────────────────────────────────────────────────────────────

#include <QObject>
#include <QString>

class ExpressionController : public QObject
{
    Q_OBJECT

public:
    explicit ExpressionController(QObject* parent = nullptr);

public slots:
    // Parse expression string and call Expression::evaluate()
    // Returns result as string; on error emits errorOccurred
    QString evaluate(const QString& expression);

    // check_equals: compare type_tag() of two parsed expressions
    bool checkEquals(const QString& typeA, const QString& typeB);

signals:
    void errorOccurred(const QString& message);
};
