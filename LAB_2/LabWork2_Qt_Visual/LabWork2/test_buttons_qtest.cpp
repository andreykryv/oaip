// ═══════════════════════════════════════════════════════════════════
//  test_buttons_qtest.cpp — Чёрный ящик кнопок через Qt Test
//
//  «Нажать кнопку» = вызвать слот контроллера.
//  QSignalSpy перехватывает сигналы и даёт проверить,
//  что UI получил правильный отклик.
//
//  Классы тестов:
//    TestDatesPage   — вкладка Даты    (DatesPage.qml)
//    TestDeanPage    — вкладка Деканат (DeanPage.qml)
//    TestExprPage    — вкладка Выражения (ExprPage.qml)
//    TestTypePage    — вкладка Типы    (TypeCheckPage.qml)
// ═══════════════════════════════════════════════════════════════════
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QtCore/QTemporaryFile>
#include <QTextStream>
#include <QVariantList>
#include <QVariantMap>

#include "src/DateController.h"
#include "src/StudentController.h"
#include "src/ExpressionController.h"

// ── Хелперы ──────────────────────────────────────────────────────

static QString writeTempFile(const QStringList& lines) {
    auto* f = new QTemporaryFile();
    f->setAutoRemove(false);
    f->open();
    QTextStream out(f);
    for (const auto& l : lines) out << l << "\n";
    f->close();
    QString path = f->fileName();
    delete f;
    return path;
}

static QVariantList G(int a,int b,int c,int d,int e) {
    return {a,b,c,d,e};
}

// ═══════════════════════════════════════════════════════════════════
//  Вкладка 1 — Даты
// ═══════════════════════════════════════════════════════════════════
class TestDatesPage : public QObject
{
    Q_OBJECT

    DateController* ctrl = nullptr;

private slots:
    void init()    { ctrl = new DateController; }
    void cleanup() { delete ctrl; ctrl = nullptr; }

    // ── Кнопка «+ Добавить дату» ─────────────────────────────

    void addDate_valid_emitsDatesChanged() {
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->addDate("15.06.2023");
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 1);
    }

    void addDate_threeItems_correctOrder() {
        ctrl->addDate("01.01.2020");
        ctrl->addDate("15.06.2023");
        ctrl->addDate("31.12.2025");
        QCOMPARE(ctrl->count(), 3);
        QCOMPARE(ctrl->dates()[0].toMap()["dateVal"].toString(), "01.01.2020");
        QCOMPARE(ctrl->dates()[2].toMap()["dateVal"].toString(), "31.12.2025");
    }

    void addDate_invalid_emitsError_notAdded() {
        QSignalSpy errSpy(ctrl, &DateController::error);
        QSignalSpy chgSpy(ctrl, &DateController::datesChanged);
        ctrl->addDate("99.99.9999");
        QCOMPARE(errSpy.count(), 1);
        QCOMPARE(chgSpy.count(), 0);
        QCOMPARE(ctrl->count(), 0);
    }

    void addDate_empty_emitsError() {
        QSignalSpy spy(ctrl, &DateController::error);
        ctrl->addDate("");
        QCOMPARE(spy.count(), 1);
    }

    void addDate_slashSeparator_emitsError() {
        QSignalSpy spy(ctrl, &DateController::error);
        ctrl->addDate("15/06/2023");
        QCOMPARE(spy.count(), 1);
    }

    void addDate_feb29_leapYear_ok() {
        QSignalSpy err(ctrl, &DateController::error);
        ctrl->addDate("29.02.2024");
        QCOMPARE(err.count(), 0);
        QCOMPARE(ctrl->count(), 1);
    }

    void addDate_feb29_nonLeap_emitsError() {
        QSignalSpy err(ctrl, &DateController::error);
        ctrl->addDate("29.02.2023");
        QCOMPARE(err.count(), 1);
        QCOMPARE(ctrl->count(), 0);
    }

    void addDate_minDate_ok() {
        QSignalSpy err(ctrl, &DateController::error);
        ctrl->addDate("01.01.0001");
        QCOMPARE(err.count(), 0);
        QCOMPARE(ctrl->count(), 1);
    }

    void addDate_day0_emitsError() {
        QSignalSpy spy(ctrl, &DateController::error);
        ctrl->addDate("00.06.2023");
        QCOMPARE(spy.count(), 1);
    }

    void addDate_month13_emitsError() {
        QSignalSpy spy(ctrl, &DateController::error);
        ctrl->addDate("01.13.2023");
        QCOMPARE(spy.count(), 1);
    }

    // ── Кнопка «✕ Удалить» ───────────────────────────────────

    void deleteDate_first_removesCorrectly() {
        ctrl->addDate("01.01.2020");
        ctrl->addDate("15.06.2023");
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->deleteDate(0);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 1);
        QCOMPARE(ctrl->dates()[0].toMap()["dateVal"].toString(), "15.06.2023");
    }

    void deleteDate_last_listEmpty() {
        ctrl->addDate("01.01.2020");
        ctrl->deleteDate(0);
        QCOMPARE(ctrl->count(), 0);
    }

    void deleteDate_negativeIndex_noChange() {
        ctrl->addDate("01.01.2020");
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->deleteDate(-1);
        QCOMPARE(spy.count(), 0);
        QCOMPARE(ctrl->count(), 1);
    }

    void deleteDate_outOfRange_noChange() {
        ctrl->addDate("01.01.2020");
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->deleteDate(99);
        QCOMPARE(spy.count(), 0);
    }

    void deleteDate_emptyList_noChange() {
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->deleteDate(0);
        QCOMPARE(spy.count(), 0);
    }

    // ── Кнопка «Изменить выбранную» ──────────────────────────

    void editDate_valid_changesValue() {
        ctrl->addDate("01.01.2020");
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->editDate(0, "25.12.2025");
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->dates()[0].toMap()["dateVal"].toString(), "25.12.2025");
    }

    void editDate_invalid_emitsError_keepsOld() {
        ctrl->addDate("01.01.2020");
        QSignalSpy errSpy(ctrl, &DateController::error);
        ctrl->editDate(0, "ПЛОХАЯ");
        QCOMPARE(errSpy.count(), 1);
        QCOMPARE(ctrl->dates()[0].toMap()["dateVal"].toString(), "01.01.2020");
    }

    void editDate_invalidIndex_noChange() {
        ctrl->addDate("01.01.2020");
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->editDate(99, "25.12.2025");
        QCOMPARE(spy.count(), 0);
    }

    // ── Кнопка «📂 Открыть файл» ─────────────────────────────

    void openFile_threeDates_loadsAll() {
        QString path = writeTempFile({"01.01.2020","15.06.2023","31.12.2025"});
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->openFile(path);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 3);
        QCOMPARE(ctrl->dates()[1].toMap()["dateVal"].toString(), "15.06.2023");
        QFile::remove(path);
    }

    void openFile_mixedContent_skipsInvalid() {
        QString path = writeTempFile({"01.01.2020","НЕВЕРНО","31.12.2025"});
        QSignalSpy err(ctrl, &DateController::error);
        ctrl->openFile(path);
        QCOMPARE(ctrl->count(), 2);
        QVERIFY(err.count() >= 1);
        QFile::remove(path);
    }

    void openFile_nonExistent_emitsError() {
        QSignalSpy err(ctrl, &DateController::error);
        QSignalSpy chg(ctrl, &DateController::datesChanged);
        ctrl->openFile("/tmp/no_such_file_xyzabc123.txt");
        QCOMPARE(err.count(), 1);
        QCOMPARE(chg.count(), 0);
    }

    void openFile_replacesOldList() {
        ctrl->addDate("01.01.2020");
        QString path = writeTempFile({"10.10.2010","20.11.2011"});
        ctrl->openFile(path);
        QCOMPARE(ctrl->count(), 2);
        QCOMPARE(ctrl->dates()[0].toMap()["dateVal"].toString(), "10.10.2010");
        QFile::remove(path);
    }

    // ── Кнопка «💾 Сохранить» ────────────────────────────────

    void saveAndOpen_roundTrip() {
        ctrl->addDate("01.01.2020");
        ctrl->addDate("29.02.2024");
        ctrl->addDate("31.12.9999");
        QTemporaryFile tmp; tmp.open();
        QString path = tmp.fileName(); tmp.close();
        ctrl->saveFile(path);
        DateController ctrl2;
        ctrl2.openFile(path);
        QCOMPARE(ctrl2.count(), 3);
        QCOMPARE(ctrl2.dates()[1].toMap()["dateVal"].toString(), "29.02.2024");
    }

    // ── Кнопка «Дозапись в файл» ─────────────────────────────

    void appendToFile_addsItem() {
        QString path = writeTempFile({"01.01.2020"});
        ctrl->openFile(path);
        QSignalSpy spy(ctrl, &DateController::datesChanged);
        ctrl->appendToFile(path, "31.12.2025");
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 2);
        QCOMPARE(ctrl->dates()[1].toMap()["dateVal"].toString(), "31.12.2025");
        QFile::remove(path);
    }

    void appendToFile_invalidDate_emitsError() {
        QTemporaryFile tmp; tmp.open();
        QString path = tmp.fileName(); tmp.close();
        QSignalSpy err(ctrl, &DateController::error);
        ctrl->appendToFile(path, "ПЛОХАЯ_ДАТА");
        QCOMPARE(err.count(), 1);
    }

    // ── Правая панель: getResult ──────────────────────────────

    void getResult_validDate_allFieldsFilled() {
        auto r = ctrl->getResult("15.06.2023", "");
        QCOMPARE(r["next"].toString(), "16.06.2023");
        QCOMPARE(r["prev"].toString(), "14.06.2023");
        QCOMPARE(r["leap"].toString(), "Нет");
        QVERIFY(!r["week"].toString().isEmpty());
    }

    void getResult_leapYear_flagDa() {
        QCOMPARE(ctrl->getResult("01.01.2024","")["leap"].toString(), "Да");
    }

    void getResult_withBirthday_daysFilled() {
        auto r = ctrl->getResult("01.01.2023","31.01.1990");
        bool ok=false;
        int days = r["tillBday"].toString().toInt(&ok);
        QVERIFY(ok);
        QCOMPARE(days, 30);
    }

    void getResult_invalidDate_hasErrorKey() {
        auto r = ctrl->getResult("НЕВЕРНО","");
        QVERIFY(r.contains("error"));
    }

    void getResult_minDate_prevDash() {
        QCOMPARE(ctrl->getResult("01.01.0001","")["prev"].toString(), "—");
    }
};

// ═══════════════════════════════════════════════════════════════════
//  Вкладка 2 — Деканат
// ═══════════════════════════════════════════════════════════════════
class TestDeanPage : public QObject
{
    Q_OBJECT

    StudentController* ctrl = nullptr;

private slots:
    void init()    { ctrl = new StudentController; }
    void cleanup() { delete ctrl; ctrl = nullptr; }

    // ── Кнопка «+ Добавить студента» ─────────────────────────

    void addStudent_valid_countsOne() {
        QSignalSpy spy(ctrl, &StudentController::studentsChanged);
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 1);
    }

    void addStudent_emptyGroup_emitsError() {
        QSignalSpy err(ctrl, &StudentController::error);
        ctrl->addStudent("","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QCOMPARE(err.count(), 1);
        QCOMPARE(ctrl->count(), 0);
    }

    void addStudent_emptyName_emitsError() {
        QSignalSpy err(ctrl, &StudentController::error);
        ctrl->addStudent("ИВТ-21","",G(5,4,3,5,4),G(4,5,4,5,5));
        QCOMPARE(err.count(), 1);
    }

    void addStudent_wrongGradeCount_emitsError() {
        QSignalSpy err(ctrl, &StudentController::error);
        ctrl->addStudent("ИВТ-21","Иванов",{5,4},G(4,5,4,5,5));
        QCOMPARE(err.count(), 1);
    }

    void addStudent_three_countCorrect() {
        ctrl->addStudent("ИВТ-21","A",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","B",G(3,2,4,3,4),G(4,3,3,4,4));
        ctrl->addStudent("ИВТ-23","C",G(2,3,2,4,3),G(2,3,2,4,3));
        QCOMPARE(ctrl->count(), 3);
    }

    // ── Кнопка «✕ Удалить» ───────────────────────────────────

    void deleteStudent_first_secondBecomesFirst() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","Петрова",G(3,2,4,3,4),G(4,3,3,4,4));
        ctrl->deleteStudent(0);
        QCOMPARE(ctrl->count(), 1);
        QCOMPARE(ctrl->allStudents()[0].toMap()["nm"].toString(),"Петрова");
    }

    void deleteStudent_invalidIndex_noChange() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QSignalSpy spy(ctrl, &StudentController::studentsChanged);
        ctrl->deleteStudent(-1);
        ctrl->deleteStudent(99);
        QCOMPARE(spy.count(), 0);
    }

    // ── Попап: кнопка «Сохранить» (editStudent) ──────────────

    void editStudent_valid_dataUpdated() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QSignalSpy spy(ctrl, &StudentController::studentsChanged);
        ctrl->editStudent(0,"ИВТ-99","Новиков",G(4,4,4,4,4),G(3,3,3,3,3));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->allStudents()[0].toMap()["grp"].toString(),"ИВТ-99");
        QCOMPARE(ctrl->allStudents()[0].toMap()["nm"].toString(), "Новиков");
    }

    void editStudent_emptyGroup_emitsError_keepsOld() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QSignalSpy err(ctrl, &StudentController::error);
        ctrl->editStudent(0,"","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QCOMPARE(err.count(), 1);
        QCOMPARE(ctrl->allStudents()[0].toMap()["grp"].toString(),"ИВТ-21");
    }

    // ── Кнопки сортировки ────────────────────────────────────

    void sortByName_firstCall_ascending() {
        ctrl->addStudent("ИВТ-21","Яковлев",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","Абрамов",G(3,2,4,3,4),G(4,3,3,4,4));
        ctrl->addStudent("ИВТ-23","Михайлов",G(2,3,2,4,3),G(2,3,2,4,3));
        ctrl->sortByName();
        QVERIFY(ctrl->sortAsc());
        QCOMPARE(ctrl->allStudents()[0].toMap()["nm"].toString(),"Абрамов");
    }

    void sortByName_secondCall_descending() {
        ctrl->addStudent("ИВТ-21","Яковлев",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","Абрамов",G(3,2,4,3,4),G(4,3,3,4,4));
        ctrl->sortByName();
        ctrl->sortByName(); // второе нажатие — инвертирует
        QVERIFY(!ctrl->sortAsc());
        QCOMPARE(ctrl->allStudents()[0].toMap()["nm"].toString(),"Яковлев");
    }

    void sortByGroup_correctOrder() {
        ctrl->addStudent("ИВТ-23","C",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-21","A",G(3,2,4,3,4),G(4,3,3,4,4));
        ctrl->addStudent("ИВТ-22","B",G(2,3,2,4,3),G(2,3,2,4,3));
        ctrl->sortByGroup();
        QCOMPARE(ctrl->allStudents()[0].toMap()["grp"].toString(),"ИВТ-21");
        QCOMPARE(ctrl->allStudents()[2].toMap()["grp"].toString(),"ИВТ-23");
    }

    void sortByAvg_defaultDescending_bestFirst() {
        ctrl->addStudent("G","Плохой", G(2,2,2,2,2),G(2,2,2,2,2));
        ctrl->addStudent("G","Хороший",G(5,5,5,5,5),G(5,5,5,5,5));
        ctrl->addStudent("G","Средний",G(3,3,3,3,3),G(3,3,3,3,3));
        ctrl->sortByAvgDesc();
        QVERIFY(!ctrl->sortAsc()); // по умолчанию убывание
        QCOMPARE(ctrl->allStudents()[0].toMap()["nm"].toString(),"Хороший");
    }

    void sortEmitsBothSignals() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QSignalSpy chg(ctrl, &StudentController::studentsChanged);
        QSignalSpy srt(ctrl, &StudentController::sortChanged);
        ctrl->sortByName();
        QCOMPARE(chg.count(), 1);
        QCOMPARE(srt.count(), 1);
    }

    // ── Комбобокс фильтра ────────────────────────────────────

    void filter_all_returnsAll() {
        ctrl->addStudent("ИВТ-21","A",G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","B",G(2,3,2,4,3),G(2,3,2,4,3));
        QCOMPARE(ctrl->filtered(0,"").count(), 2);
    }

    void filter_debtors_onlyDebt() {
        ctrl->addStudent("ИВТ-21","ОК",     G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-21","Должник",G(2,3,4,5,3),G(4,4,4,4,4));
        ctrl->addStudent("ИВТ-22","Отчисл", G(2,3,3,3,3),G(2,2,4,4,4));
        auto list = ctrl->filtered(1,"");
        QCOMPARE(list.count(), 1);
        QCOMPARE(list[0].toMap()["nm"].toString(),"Должник");
    }

    void filter_expel_onlyExpelled() {
        ctrl->addStudent("ИВТ-21","ОК",    G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","Отчисл",G(2,3,3,3,3),G(2,2,4,4,4));
        auto list = ctrl->filtered(2,"");
        QCOMPARE(list.count(), 1);
        QCOMPARE(list[0].toMap()["nm"].toString(),"Отчисл");
    }

    void filter_searchByName_caseInsensitive() {
        ctrl->addStudent("ИВТ-21","Иванов Иван", G(5,4,3,5,4),G(4,5,4,5,5));
        ctrl->addStudent("ИВТ-22","Петрова Мария",G(3,2,4,3,4),G(4,3,3,4,4));
        auto list = ctrl->filtered(0,"иван");
        QCOMPARE(list.count(), 1);
        QCOMPARE(list[0].toMap()["nm"].toString(),"Иванов Иван");
    }

    void filter_searchNoMatch_empty() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,5,4),G(4,5,4,5,5));
        QCOMPARE(ctrl->filtered(0,"НЕТАКОГО").count(), 0);
    }

    // ── Счётчики статусов ────────────────────────────────────

    void statusCounters_allThreeTypes() {
        ctrl->addStudent("G","ОК",  G(5,5,5,5,5),G(5,5,5,5,5));
        ctrl->addStudent("G","Долг",G(2,4,4,4,4),G(4,4,4,4,4));
        ctrl->addStudent("G","Отч", G(2,4,4,4,4),G(2,2,4,4,4));
        QCOMPARE(ctrl->okCount(),    1);
        QCOMPARE(ctrl->debtCount(),  1);
        QCOMPARE(ctrl->expelCount(), 1);
    }

    void statusCounters_updateAfterDelete() {
        ctrl->addStudent("G","Отч",G(2,4,4,4,4),G(2,2,4,4,4));
        QCOMPARE(ctrl->expelCount(), 1);
        ctrl->deleteStudent(0);
        QCOMPARE(ctrl->expelCount(), 0);
    }

    // ── Файловые операции ────────────────────────────────────

    void openFile_validStudents_loaded() {
        using A = std::array<int,5>;
        Student s("ИВТ-21","Иванов А.А.",A{5,4,3,2,1},A{1,2,3,4,5});
        QString path = writeTempFile({QString::fromStdString(s.serialize())});
        QSignalSpy spy(ctrl, &StudentController::studentsChanged);
        ctrl->openFile(path);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(ctrl->count(), 1);
        QCOMPARE(ctrl->allStudents()[0].toMap()["grp"].toString(),"ИВТ-21");
        QFile::remove(path);
    }

    void saveAndOpen_roundTrip() {
        ctrl->addStudent("ИВТ-21","Иванов",G(5,4,3,2,1),G(1,2,3,4,5));
        QTemporaryFile tmp; tmp.open();
        QString path = tmp.fileName(); tmp.close();
        ctrl->saveFile(path);
        StudentController ctrl2;
        ctrl2.openFile(path);
        QCOMPARE(ctrl2.count(), 1);
        QCOMPARE(ctrl2.allStudents()[0].toMap()["nm"].toString(),"Иванов");
    }

    void openFile_nonExistent_emitsError() {
        QSignalSpy err(ctrl, &StudentController::error);
        ctrl->openFile("/tmp/totally_no_file_xyz987.txt");
        QCOMPARE(err.count(), 1);
    }
};

// ═══════════════════════════════════════════════════════════════════
//  Вкладка 3 — Выражения
// ═══════════════════════════════════════════════════════════════════
class TestExprPage : public QObject
{
    Q_OBJECT

    ExpressionController* ctrl = nullptr;

private slots:
    void init()    { ctrl = new ExpressionController; }
    void cleanup() { delete ctrl; ctrl = nullptr; }

    // ── Кнопка «▶ Вычислить» ─────────────────────────────────

    void eval_addition()          { QCOMPARE(ctrl->evaluate("3 + 4"),     QString("7"));  }
    void eval_subtraction()       { QCOMPARE(ctrl->evaluate("10 - 3"),    QString("7"));  }
    void eval_multiplication()    { QCOMPARE(ctrl->evaluate("6 * 7"),     QString("42")); }
    void eval_division()          { QCOMPARE(ctrl->evaluate("10 / 2"),    QString("5"));  }
    void eval_zeroResult()        { QCOMPARE(ctrl->evaluate("5 - 5"),     QString("0"));  }
    void eval_multipleOps()       { QCOMPARE(ctrl->evaluate("2 + 3 - 1"), QString("4"));  }
    void eval_floatInput()        { QCOMPARE(ctrl->evaluate("1.5 + 2.5"), QString("4"));  }
    void eval_precedence_mulFirst(){ QCOMPARE(ctrl->evaluate("3 + 4 * 5"),QString("23")); }
    void eval_parentheses()       { QCOMPARE(ctrl->evaluate("(3 + 4) * 5"),QString("35"));}
    void eval_nested()            { QCOMPARE(ctrl->evaluate("((1+2)*(3+4))/7"),QString("3")); }

    void eval_negativeNumber() {
        QCOMPARE(ctrl->evaluate("-5 + 3"), QString("-2"));
    }

    void eval_floatResult_approximately() {
        bool ok=false;
        double v = ctrl->evaluate("1 / 3").toDouble(&ok);
        QVERIFY(ok);
        QVERIFY(qAbs(v - 1.0/3.0) < 1e-9);
    }

    void eval_divisionByZero_returnsErrorString() {
        QSignalSpy err(ctrl, &ExpressionController::error);
        QString r = ctrl->evaluate("5 / 0");
        QVERIFY(r.startsWith("Ошибка"));
        QCOMPARE(err.count(), 1);
    }

    void eval_empty_returnsErrorString() {
        QSignalSpy err(ctrl, &ExpressionController::error);
        QString r = ctrl->evaluate("");
        QVERIFY(r.startsWith("Ошибка"));
        QCOMPARE(err.count(), 1);
    }

    void eval_largeNumbers() {
        // 1000000 * 1000000 = 1e12
        bool ok=false;
        double v = ctrl->evaluate("1000000 * 1000000").toDouble(&ok);
        QVERIFY(ok);
        QVERIFY(qAbs(v - 1e12) < 1.0);
    }

    // ── Кнопка «✕ Очистить» — после очистки вычисление снова работает

    void clear_thenEvaluate_works() {
        ctrl->evaluate("2 + 2");
        ctrl->evaluateEmpty();  // «нажали очистить»
        QCOMPARE(ctrl->evaluate("5 * 5"), QString("25"));
    }

    void clear_empty_doesNotCrash() {
        ctrl->evaluateEmpty();  // без предыдущего вычисления тоже не падает
    }

    // ── parseTree — визуализация AST ─────────────────────────

    void parseTree_binop_hasCorrectStructure() {
        auto tree = ctrl->parseTree("3 + 4");
        QCOMPARE(tree["label"].toString(), "+");
        QVERIFY(tree["isOp"].toBool());
        QVERIFY(tree.contains("left"));
        QVERIFY(tree.contains("right"));
    }

    void parseTree_number_leafNode() {
        auto tree = ctrl->parseTree("42");
        QVERIFY(!tree["isOp"].toBool());
        QVERIFY(!tree.contains("left"));
    }

    void parseTree_nested_leftIsPlus() {
        auto tree = ctrl->parseTree("(1+2)*3");
        QCOMPARE(tree["label"].toString(), "*");
        QCOMPARE(tree["left"].toMap()["label"].toString(), "+");
    }
};

// ═══════════════════════════════════════════════════════════════════
//  Вкладка 4 — Типы
// ═══════════════════════════════════════════════════════════════════
class TestTypePage : public QObject
{
    Q_OBJECT

    ExpressionController* ctrl = nullptr;

private slots:
    void init()    { ctrl = new ExpressionController; }
    void cleanup() { delete ctrl; ctrl = nullptr; }

    // ── Кнопка «▶ Вызвать check_equals()» ────────────────────
    // UI: комбобокс left/right, 0=Number, 1=BinaryOperation

    void checkEquals_NN_true()  { QVERIFY( ctrl->checkEquals(0,0)); }
    void checkEquals_BB_true()  { QVERIFY( ctrl->checkEquals(1,1)); }
    void checkEquals_NB_false() { QVERIFY(!ctrl->checkEquals(0,1)); }
    void checkEquals_BN_false() { QVERIFY(!ctrl->checkEquals(1,0)); }

    void checkEquals_symmetric() {
        // (N,B) и (B,N) должны давать одинаковый результат
        QCOMPARE(ctrl->checkEquals(0,1), ctrl->checkEquals(1,0));
    }

    void checkEquals_allCombinations_tableDecision() {
        // Полная таблица решений за один тест
        QVERIFY( ctrl->checkEquals(0,0));
        QVERIFY( ctrl->checkEquals(1,1));
        QVERIFY(!ctrl->checkEquals(0,1));
        QVERIFY(!ctrl->checkEquals(1,0));
    }

    void checkEquals_neverEmitsError() {
        QSignalSpy err(ctrl, &ExpressionController::error);
        ctrl->checkEquals(0,0);
        ctrl->checkEquals(0,1);
        ctrl->checkEquals(1,0);
        ctrl->checkEquals(1,1);
        QCOMPARE(err.count(), 0);
    }

    void checkEquals_repeatedCalls_consistent() {
        // Многократный вызов с теми же аргументами даёт тот же результат
        for (int i=0; i<10; ++i) {
            QVERIFY( ctrl->checkEquals(0,0));
            QVERIFY(!ctrl->checkEquals(0,1));
        }
    }
};

// ═══════════════════════════════════════════════════════════════════
//  Точка входа
// ═══════════════════════════════════════════════════════════════════
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;
    { TestDatesPage t; status |= QTest::qExec(&t, argc, argv); }
    { TestDeanPage  t; status |= QTest::qExec(&t, argc, argv); }
    { TestExprPage  t; status |= QTest::qExec(&t, argc, argv); }
    { TestTypePage  t; status |= QTest::qExec(&t, argc, argv); }
    return status;
}

#include "test_buttons_qtest.moc"
