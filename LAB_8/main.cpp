#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setFont(QFont("Consolas", 11));

    MainWindow w;
    w.show();

    return app.exec();
}
