#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Лаб. работа №4");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("БГТУ");

    QFont font("Segoe UI", 10);
    font.setHintingPreference(QFont::PreferFullHinting);
    app.setFont(font);

    MainWindow w;
    w.show();

    return app.exec();
}
