#include "StudentController.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <stdexcept>

StudentController::StudentController(QObject* parent) : QObject(parent)
{
    // Демо-данные на старте
    auto mk = [](auto g,auto n,auto w,auto s){
        return Student(g,n,w,s);
    };
    using A = std::array<int,5>;
    m_students.push_back(Student("ИВТ-21","Иванов А.А.",   A{5,4,3,5,4}, A{5,4,4,5,5}));
    m_students.push_back(Student("ИВТ-21","Петрова М.С.",  A{3,2,4,3,4}, A{4,3,3,4,4}));
    m_students.push_back(Student("ИВТ-22","Сидоров Д.О.",  A{2,3,2,4,3}, A{2,3,2,4,3}));
    m_students.push_back(Student("ИВТ-22","Козлова Е.В.",  A{5,5,4,5,5}, A{5,5,5,5,4}));
    m_students.push_back(Student("ИВТ-23","Морозов К.П.",  A{4,4,3,4,3}, A{2,2,3,4,3}));
    m_students.push_back(Student("ИВТ-23","Волкова Н.Н.",  A{3,2,4,4,3}, A{3,3,2,2,3}));
}

QVariantMap StudentController::studentToMap(int i) const
{
    const Student& s = m_students[i];
    QVariantMap m;
    m["grp"]  = QString::fromStdString(s.group());
    m["nm"]   = QString::fromStdString(s.fullName());

    // Оценки
    const auto& w = s.winterGrades();
    const auto& su = s.summerGrades();
    m["w1"]=w[0]; m["w2"]=w[1]; m["w3"]=w[2]; m["w4"]=w[3]; m["w5"]=w[4];
    m["s1"]=su[0]; m["s2"]=su[1]; m["s3"]=su[2]; m["s4"]=su[3]; m["s5"]=su[4];

    
    m["avgW"] = QString::number(s.avgWinter(), 'f', 2);
     m["avgS"] = QString::number(s.avgSummer(), 'f', 2);
     m["st"]   = s.shouldExpel() ? "expel" : s.hasWinterDebt() ? "debt" : "ok";

    
    return m;
}

QVariantList StudentController::allStudents() const
{
    QVariantList list;
    for (int i = 0; i < (int)m_students.size(); ++i)
        list.append(studentToMap(i));
    return list;
}

int StudentController::debtCount() const {
    return std::count_if(m_students.begin(), m_students.end(),
        [](const Student& s){ return s.hasWinterDebt() && !s.shouldExpel(); });
}

int StudentController::expelCount() const {
    return std::count_if(m_students.begin(), m_students.end(),
        [](const Student& s){ return s.shouldExpel(); });
}
int StudentController::okCount() const {
    return (int)m_students.size() - debtCount() - expelCount();
}

void StudentController::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error("Не удалось открыть файл: " + path); return;
    }
    m_students.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            m_students.push_back(Student::deserialize(line.toStdString()));
        } catch (const std::exception& e) {
            emit error(QString("Ошибка в строке: %1").arg(e.what()));
        }
    }
    emit studentsChanged();
}

void StudentController::saveFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error("Не удалось сохранить: " + path); return;
    }
    QTextStream out(&file);
    for (const auto& s : m_students)
        out << QString::fromStdString(s.serialize()) << "\n";
}

void StudentController::addStudent(const QString& group, const QString& name,
                                    const QVariantList& winter, const QVariantList& summer)
{
    if (winter.size() != 5 || summer.size() != 5) {
        emit error("Нужно ровно 5 оценок за каждую сессию"); return;
    }
    std::array<int,5> w, s;
    for (int i = 0; i < 5; ++i) { w[i]=winter[i].toInt(); s[i]=summer[i].toInt(); }
    m_students.push_back(Student(group.toStdString(), name.toStdString(), w, s));
    emit studentsChanged();
}

void StudentController::deleteStudent(int index)
{
    if (index < 0 || index >= (int)m_students.size()) return;
    m_students.erase(m_students.begin() + index);
    emit studentsChanged();
}

void StudentController::editStudent(int index, const QString& group, const QString& name,
                                     const QVariantList& winter, const QVariantList& summer)
{
    if (index < 0 || index >= (int)m_students.size()) return;
    if (winter.size() != 5 || summer.size() != 5) return;
    std::array<int,5> w, s;
    for (int i = 0; i < 5; ++i) { w[i]=winter[i].toInt(); s[i]=summer[i].toInt(); }
    m_students[index] = Student(group.toStdString(), name.toStdString(), w, s);
    emit studentsChanged();
}

QVariantList StudentController::filtered(int filterType, const QString& nameSearch) const
{
    QVariantList list;
    for (int i = 0; i < (int)m_students.size(); ++i) {
        const Student& s = m_students[i];
        // Name filter
        if (!nameSearch.isEmpty()) {
            QString nm = QString::fromStdString(s.fullName()).toLower();
            if (!nm.contains(nameSearch.toLower())) continue;
        }
        //  раскомментируйте после реализации hasWinterDebt / shouldExpel:
        if (filterType == 1 && !s.hasWinterDebt()) continue;
        if (filterType == 2 && !s.shouldExpel())   continue;
        list.append(studentToMap(i));
    }
    return list;
}

void StudentController::sortByName() {
    std::sort(m_students.begin(), m_students.end(),
        [](const Student& a, const Student& b){ return a.fullName() < b.fullName(); });
    emit studentsChanged();
}
void StudentController::sortByGroup() {
    std::sort(m_students.begin(), m_students.end(),
        [](const Student& a, const Student& b){
            return a.group() != b.group() ? a.group() < b.group() : a.fullName() < b.fullName();
        });
    emit studentsChanged();
}
void StudentController::sortByAvgDesc() {
    // раскомментируйте после реализации avgSummer():
    std::sort(m_students.begin(), m_students.end(),
      [](const Student& a, const Student& b){ return a.avgSummer() > b.avgSummer(); });
    emit studentsChanged();
}
