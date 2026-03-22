#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    // Set OpenGL compatibility profile globally before QApplication
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setVersion(2, 1);
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication app(argc, argv);
    app.setApplicationName("Mercedes 190 Evo II — Лаб. №1");

    app.setStyleSheet(R"(
        QMainWindow, QWidget { background-color:#12122a; color:#dde; }
        QGroupBox {
            border:1px solid #334; border-radius:5px;
            margin-top:8px; font-weight:bold; color:#88aaff;
        }
        QGroupBox::title { subcontrol-origin:margin; left:8px; padding:0 4px; }
        QPushButton {
            background:#1e2040; border:1px solid #446;
            border-radius:4px; padding:5px 8px; color:#ccd;
        }
        QPushButton:hover   { background:#2a3060; border-color:#88aaff; }
        QPushButton:pressed { background:#88aaff; color:#111; }
        QPushButton:disabled{ color:#445; border-color:#223; }
        QStatusBar { background:#0a0a18; color:#00ee88; font-size:12px; }
        QLabel { color:#bbc; }
    )");

    MainWindow w;
    w.show();
    return app.exec();
}
