#include <QApplication>
#include <QFontDatabase>
#include <QSettings>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Lab5App");
    app.setOrganizationName("University");

    // Устанавливаем современный стиль
    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();
    return app.exec();
}