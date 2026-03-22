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
    // Диалоги из QML напрямую 
    // Открывают нативный диалог ОС, сами вызывают openFile/saveFile.
   
    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE void saveFileDialog();

    //  Работа с файлом 
    //  построчно.
    //   Каждую строку десериализует в Student 
    //   Если строка корректна  норм
    //   Если строка некорректна скип
    //   После чтения  emit studentsChanged().
    void openFile(const QString& path);
    void saveFile(const QString& path);

   
    void addStudent(const QString& group, const QString& name,
                    const QVariantList& winter, const QVariantList& summer);
    void deleteStudent(int index);
    void editStudent(int index, const QString& group, const QString& name,
                     const QVariantList& winter, const QVariantList& summer);

    Q_INVOKABLE QVariantMap  getStudent(int index) const;
    Q_INVOKABLE QVariantList filtered(int filterType,
                                      const QString& nameSearch) const;

   
    // Первый вызов  по возрастанию.
    // Повторный вызов той же колонки  инвертирует направление.
    void sortByName();
    void sortByGroup();
    void sortByAvgDesc();

signals:
    void studentsChanged();
    void sortChanged();
    void error(const QString& message);

private:
    std::vector<Student> m_students;
    QString m_sortColumn;
    bool    m_sortAsc = true;
    QString m_lastPath;

    QVariantMap studentToMap(int index) const;
    void applySort();

    // Проверяет Student перед добавлением в коллекцию.
    // Возвращает true если все оценки в диапазоне [1..10] и поля непусты.
    bool validateAndAdd(const Student& s, const QString& sourceLabel);
};