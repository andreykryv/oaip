#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QVariantList>
#include "Student.h"
#include <vector>


class StudentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        Group = Qt::UserRole + 1,
        FullName,
        W1,W2,W3,W4,W5,   // winter grades
        S1,S2,S3,S4,S5,   // summer grades
        AvgWinter,
        AvgSummer,
        Status             // "ok" | "debt" | "expel"
    };

    explicit StudentModel(QObject* parent = nullptr);

    int     rowCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int,QByteArray> roleNames() const override;

    void setStudents(const std::vector<Student>& students);
    const std::vector<Student>& students() const { return m_students; }

private:
    std::vector<Student> m_students;
};

//мост для qml
class StudentBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StudentModel* model READ model CONSTANT)
    Q_PROPERTY(int debtCount   READ debtCount   NOTIFY statsChanged)
    Q_PROPERTY(int expelCount  READ expelCount  NOTIFY statsChanged)
    Q_PROPERTY(int okCount     READ okCount     NOTIFY statsChanged)

public:
    explicit StudentBackend(QObject* parent = nullptr);
    StudentModel* model() const { return m_model; }

    int debtCount()  const;
    int expelCount() const;
    int okCount()    const;

public slots:
    //файлы
    void loadFromFile(const QString& filePath);
    void saveToFile  (const QString& filePath);

    //работа с коллекцией 
    void addStudent   (const QString& group, const QString& name,
                       const QVariantList& winter, const QVariantList& summer);
    void removeStudent(int index);
    void editStudent  (int index,
                       const QString& group, const QString& name,
                       const QVariantList& winter, const QVariantList& summer);

    //фильтры
    // filterMode: 0=all, 1=debts, 2=expel
    void applyFilter(int filterMode);
    void sortByName ();
    void sortByGroup();
    void sortByAvgSummer();

    // ── Search ───────────────────────────────────────────────
    int findByName(const QString& name) const;   // returns first matching index or -1

signals:
    void errorOccurred(const QString& message);
    void statsChanged();

private:
    StudentModel*        m_model;
    std::vector<Student> m_all;      // master copy (unfiltered)
    std::vector<Student> m_current;  // currently displayed

    void refreshModel();
    std::array<int,Student::GRADES_PER_SESSION> toArray(const QVariantList& list) const;
};
