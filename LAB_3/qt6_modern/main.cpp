#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Настройка приложения
    app.setApplicationName("Лабораторная работа №3 — Рекурсия");
    app.setApplicationVersion("2.0 Qt6 Modern");
    app.setOrganizationName("КПИ им. Игоря Сикорского");
    app.setOrganizationDomain("kpi.ua");
    
    // Базовый шрифт
    QFont font("Segoe UI", 10);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);
    
    // Тёмная тема по умолчанию
    app.setStyle("Fusion");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
