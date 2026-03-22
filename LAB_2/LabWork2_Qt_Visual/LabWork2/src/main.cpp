#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>

#include "backend/DateController.h"
#include "backend/StudentController.h"
#include "backend/ExpressionController.h"

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
    app.setApplicationName("LabWork2");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("BSUIR");

  
    DateController       dateBE;
    StudentController    studentBE;
    ExpressionController exprBE;

    QQmlApplicationEngine engine;

  
    //   dateBE.openFile(path)       / dateBE.dates  / etc.
    //   studentBE.openFile(path)    / studentBE.allStudents / etc.
    //   exprBE.evaluate("3+4.5*5") / exprBE.checkEquals(0,1)
    engine.rootContext()->setContextProperty("dateBE",    &dateBE);
    engine.rootContext()->setContextProperty("studentBE", &studentBE);
    engine.rootContext()->setContextProperty("exprBE",    &exprBE);

    const QUrl url(QStringLiteral("qrc:/qt/qml/LabWork2/Main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(url);
    return app.exec();
}
