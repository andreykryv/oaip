#pragma once
// ─────────────────────────────────────────────────────────────
//  ExpressionController.h
//  Qt-объект для Заданий 3 и 4.
//  Подключён к QML как "exprBE".
//
//  КАК ПОДКЛЮЧИТЬ В QML:
//    AppButton {
//        onClicked: {
//            var r = exprBE.evaluate(exprInput.text)
//            resultText = r
//        }
//    }
//    AppButton {
//        onClicked: {
//            var r = exprBE.checkEquals(leftCombo.currentIndex,
//                                       rightCombo.currentIndex)
//            // r == true/false
//        }
//    }
// ─────────────────────────────────────────────────────────────
#include <QObject>
#include <QString>

class ExpressionController : public QObject
{
    Q_OBJECT

public:
    explicit ExpressionController(QObject* parent = nullptr);

public slots:
    // ── Задание 3 ─────────────────────────────────────────────
    // Принимает строку вида "3 + 4.5 * 5"
    // Возвращает результат как строку, или "Ошибка: ..."
    // Вызвать из QML: var r = exprBE.evaluate("3 + 4.5 * 5")
    Q_INVOKABLE QString evaluate(const QString& expression);

    // ── Задание 4 ─────────────────────────────────────────────
    // typeA, typeB: 0 = Number, 1 = BinaryOperation
    // Возвращает true если оба одного класса
    // Вызвать из QML: var eq = exprBE.checkEquals(leftCombo.currentIndex,
    //                                              rightCombo.currentIndex)
    Q_INVOKABLE bool checkEquals(int typeA, int typeB);

signals:
    void error(const QString& message);
};
