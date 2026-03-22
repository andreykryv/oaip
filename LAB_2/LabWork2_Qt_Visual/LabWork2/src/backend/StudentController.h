#pragma once
// ─────────────────────────────────────────────────────────────
//  StudentController.h
//  Qt-объект для Задания 2 (Деканат).
//  Подключён к QML как "studentBE".
//
//  КАК ПОДКЛЮЧИТЬ В QML:
//    AppButton { onClicked: studentBE.openFile(path) }
//    AppComboBox { onCurrentIndexChanged: studentListView.model = studentBE.filtered(currentIndex, searchInput.text) }
// ─────────────────────────────────────────────────────────────
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <vector>
#include "Student.h"

class StudentController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList allStudents READ allStudents NOTIFY studentsChanged)
    Q_PROPERTY(int debtCount   READ debtCount   NOTIFY studentsChanged)
    Q_PROPERTY(int expelCount  READ expelCount  NOTIFY studentsChanged)
    Q_PROPERTY(int okCount     READ okCount     NOTIFY studentsChanged)

public:
    explicit StudentController(QObject* parent = nullptr);

    // ── Свойства ─────────────────────────────────────────────
    QVariantList allStudents() const;
    int debtCount()  const;
    int expelCount() const;
    int okCount()    const;

public slots:
    // ── Файлы ────────────────────────────────────────────────
    // studentBE.openFile(path)
    void openFile(const QString& path);
    // studentBE.saveFile(path)
    void saveFile(const QString& path);

    // ── CRUD ─────────────────────────────────────────────────
    // studentBE.addStudent("ИВТ-21","Иванов А.А.",[5,4,3,5,4],[5,4,4,5,5])
    void addStudent(const QString& group, const QString& name,
                    const QVariantList& winter, const QVariantList& summer);

    // studentBE.deleteStudent(index)
    void deleteStudent(int index);

    // studentBE.editStudent(index, "ИВТ-21","Петров И.И.",[...],[...])
    void editStudent(int index, const QString& group, const QString& name,
                     const QVariantList& winter, const QVariantList& summer);

    // ── Фильтрация / поиск ───────────────────────────────────
    // filterType: 0=все, 1=задолжники, 2=к отчислению
    // studentBE.filtered(filterCombo.currentIndex, searchInput.text)
    Q_INVOKABLE QVariantList filtered(int filterType, const QString& nameSearch) const;

    // ── Сортировка ───────────────────────────────────────────
    // studentBE.sortByName()
    void sortByName();
    // studentBE.sortByGroup()
    void sortByGroup();
    // studentBE.sortByAvgDesc()
    void sortByAvgDesc();

signals:
    void studentsChanged();
    void error(const QString& message);

private:
    std::vector<Student> m_students;
    QVariantMap studentToMap(int index) const;
};
