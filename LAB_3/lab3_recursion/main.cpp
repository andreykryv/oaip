#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Лабораторная работа №3");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("КПИ");

    // Базовый шрифт
    QFont font("Segoe UI", 10);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    MainWindow w;
    w.show();
    return app.exec();
}
