#include "stringdemo.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

StringDemo::StringDemo(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);

    input1 = new QLineEdit;
    input1->setPlaceholderText("Строка 1");
    input2 = new QLineEdit;
    input2->setPlaceholderText("Строка 2 (или delim/число)");
    countEdit = new QLineEdit;
    countEdit->setPlaceholderText("n (для strncpy и т.п.)");

    auto *form = new QFormLayout;
    form->addRow("Строка 1:", input1);
    form->addRow("Строка 2:", input2);
    form->addRow("Параметр n:", countEdit);
    mainLayout->addLayout(form);

    opCombo = new QComboBox;
    QStringList ops = {
        "memcpy", "memmove", "strcpy", "strncpy", "strcat", "strncat",
        "memcmp", "strcmp", "strcoll", "strncmp", "strxfrm", "strtok",
        "memset", "strerror", "strlen"
    };
    opCombo->addItems(ops);
    mainLayout->addWidget(opCombo);

    QPushButton *execBtn = new QPushButton("Выполнить операцию");
    connect(execBtn, &QPushButton::clicked, this, &StringDemo::performOperation);
    mainLayout->addWidget(execBtn);

    output = new QTextEdit;
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    iterateBtn = new QPushButton("Показать итерацию по строке 1");
    connect(iterateBtn, &QPushButton::clicked, this, &StringDemo::iterateString);
    mainLayout->addWidget(iterateBtn);

    setWindowTitle("Демонстрация библиотеки String");
    setStyleSheet("background-color: #1e1e1e; color: #ccc;");
}

void StringDemo::performOperation()
{
    String s1(input1->text().toUtf8().constData());
    String s2(input2->text().toUtf8().constData());
    QString op = opCombo->currentText();
    int n = countEdit->text().toInt();

    String result;
    if (op == "memcpy") {
        // Требует буфера dest; создадим копию s1, чтобы был буфер нужного размера
        result = s1;
        result.memcpy(result.data(), s2.c_str(), std::min(n, static_cast<int>(s2.size())));
        output->setText(QString("После memcpy: %1").arg(result.c_str()));
    }
    else if (op == "memmove") {
        result = s1;
        result.memmove(result.data(), s2.c_str(), std::min(n, static_cast<int>(s2.size())));
        output->setText(QString("После memmove: %1").arg(result.c_str()));
    }
    else if (op == "strcpy" || op == "strncpy") {
        // Выделяем буфер для dest (s1.size() + 1 или n+1)
        size_t destSize = (op == "strncpy") ? std::max(static_cast<size_t>(n), s2.size()) + 1 : s2.size() + s1.size() + 1;
        char* buffer = new char[destSize];
        memset(buffer, 0, destSize);
        if (op == "strcpy")
            s1.strcpy(buffer, s2.c_str());
        else
            s1.strncpy(buffer, s2.c_str(), n);
        result = String(buffer);
        delete[] buffer;
        output->setText(QString("Результат: %1").arg(result.c_str()));
    }
    else if (op == "strcat" || op == "strncat") {
        char* buffer = new char[s1.size() + s2.size() + 1];
        buffer[0] = '\0';
        s1.strcpy(buffer, s1.c_str());
        if (op == "strcat")
            s1.strcat(buffer, s2.c_str());
        else
            s1.strncat(buffer, s2.c_str(), n);
        result = String(buffer);
        delete[] buffer;
        output->setText(QString("Результат: %1").arg(result.c_str()));
    }
    else if (op == "memcmp") {
        int res = s1.memcmp(s1.c_str(), s2.c_str(), n);
        output->setText(QString("memcmp: %1").arg(res));
    }
    else if (op == "strcmp") {
        int res = s1.strcmp(s1.c_str(), s2.c_str());
        output->setText(QString("strcmp: %1").arg(res));
    }
    else if (op == "strcoll") {
        int res = s1.strcoll(s1.c_str(), s2.c_str());
        output->setText(QString("strcoll: %1").arg(res));
    }
    else if (op == "strncmp") {
        int res = s1.strncmp(s1.c_str(), s2.c_str(), n);
        output->setText(QString("strncmp: %1").arg(res));
    }
    else if (op == "strxfrm") {
        char buf[256] = {};
        size_t len = s1.strxfrm(buf, s2.c_str(), 255);
        output->setText(QString("strxfrm: преобразовано символов %1, результат: %2").arg(len).arg(buf));
    }
    else if (op == "strtok") {
        // Для демонстрации: разбиваем s1 по разделителю из s2
        QStringList tokens;
        char* str = new char[s1.size() + 1];
        s1.strcpy(str, s1.c_str());
        char* token = s1.strtok(str, s2.c_str());
        while (token) {
            tokens << token;
            token = s1.strtok(nullptr, s2.c_str());
        }
        delete[] str;
        output->setText("Токены: " + tokens.join(" | "));
    }
    else if (op == "memset") {
        result = s1;
        int c = input2->text().isEmpty() ? 0 : input2->text().at(0).unicode();
        result.memset(result.data(), c, std::min(n, static_cast<int>(result.size())));
        output->setText(QString("memset: %1").arg(result.c_str()));
    }
    else if (op == "strerror") {
        int errnum = n;
        char* err = s1.strerror(errnum);
        output->setText(QString("strerror(%1): %2").arg(errnum).arg(err));
    }
    else if (op == "strlen") {
        size_t len = s1.strlen(s1.c_str());
        output->setText(QString("strlen: %1").arg(len));
    }
}

void StringDemo::iterateString()
{
    String s(input1->text().toUtf8().constData());
    QStringList chars;
    for (auto it = s.begin(); it != s.end(); ++it) {
        chars << QString("'%1'").arg(*it);
    }
    output->setText("Итерация: " + chars.join(" ") + "\nВсего символов: " + QString::number(s.size()));
}