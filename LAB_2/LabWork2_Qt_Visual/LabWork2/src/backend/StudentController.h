#pragma once
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

    // ── Состояние сортировки — QML читает для стрелок ────────
    Q_PROPERTY(QString sortColumn READ sortColumn NOTIFY sortChanged)
    Q_PROPERTY(bool    sortAsc    READ sortAsc    NOTIFY sortChanged)

public:
    explicit StudentController(QObject* parent = nullptr);

    QVariantList allStudents() const;
    int debtCount()  const;
    int expelCount() const;
    int okCount()    const;

    QString sortColumn() const { return m_sortColumn; }
    bool    sortAsc()    const { return m_sortAsc;    }

public slots:
    // ── Файлы ────────────────────────────────────────────────
    void openFile(const QString& path);
    void saveFile(const QString& path);

    // ── CRUD ─────────────────────────────────────────────────
    void addStudent(const QString& group, const QString& name,
                    const QVariantList& winter, const QVariantList& summer);

    void deleteStudent(int index);

    void editStudent(int index, const QString& group, const QString& name,
                     const QVariantList& winter, const QVariantList& summer);

    // ── Получить данные одного студента для формы редактирования
    Q_INVOKABLE QVariantMap getStudent(int index) const;

    // ── Фильтрация ───────────────────────────────────────────
    Q_INVOKABLE QVariantList filtered(int filterType,
                                      const QString& nameSearch) const;

    // ── Сортировка с переключением направления ───────────────
    // Первый вызов — по возрастанию.
    // Повторный вызов той же колонки — меняет направление.
    void sortByName();
    void sortByGroup();
    void sortByAvgDesc();   // колонка avgSummer, направление тоже toggleable

signals:
    void studentsChanged();
    void sortChanged();
    void error(const QString& message);

private:
    std::vector<Student> m_students;

    // Состояние сортировки
    QString m_sortColumn;   // "name" | "group" | "avg" | ""
    bool    m_sortAsc = true;

    QVariantMap studentToMap(int index) const;

    // Применяет текущий m_sortColumn + m_sortAsc к m_students
    void applySort();
};