
#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"
#include "animwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setFont(QFont("Consolas", 11));

    // Apply global stylesheet
    app.setStyleSheet(globalStyle());

    MainWindow w;
    w.show();

    return app.exec();
}