#include "StudentController.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <stdexcept>

StudentController::StudentController(QObject* parent)
    : QObject(parent)
{
    using A = std::array<int,5>;
    m_students.push_back(Student("ИВТ-21","Иванов А.А.",    A{5,4,3,5,4}, A{5,4,4,5,5}));
    m_students.push_back(Student("ИВТ-21","Петрова М.С.",   A{3,2,4,3,4}, A{4,3,3,4,4}));
    m_students.push_back(Student("ИВТ-22","Сидоров Д.О.",   A{2,3,2,4,3}, A{2,3,2,4,3}));
    m_students.push_back(Student("ИВТ-22","Козлова Е.В.",   A{5,5,4,5,5}, A{5,5,5,5,4}));
    m_students.push_back(Student("ИВТ-23","Морозов К.П.",   A{4,4,3,4,3}, A{2,2,3,4,3}));
    m_students.push_back(Student("ИВТ-23","Волкова Н.Н.",   A{3,2,4,4,3}, A{3,3,2,2,3}));
}

// ─────────────────────────────────────────────────────────────
//  studentToMap  — конвертация Student → QVariantMap для QML
// ─────────────────────────────────────────────────────────────
QVariantMap StudentController::studentToMap(int i) const
{
    const Student& s = m_students[i];
    QVariantMap m;

    m["grp"] = QString::fromStdString(s.group());
    m["nm"]  = QString::fromStdString(s.fullName());

    const auto& w  = s.winterGrades();
    const auto& su = s.summerGrades();
    m["w1"]=w[0];  m["w2"]=w[1];  m["w3"]=w[2];  m["w4"]=w[3];  m["w5"]=w[4];
    m["s1"]=su[0]; m["s2"]=su[1]; m["s3"]=su[2]; m["s4"]=su[3]; m["s5"]=su[4];

    m["avgW"] = QString::number(s.avgWinter(), 'f', 2);
    m["avgS"] = QString::number(s.avgSummer(), 'f', 2);
    m["st"]   = s.shouldExpel() ? "expel"
              : s.hasWinterDebt() ? "debt"
              : "ok";

    // Индекс нужен делегату для кнопок редактирования/удаления
    m["idx"] = i;

    return m;
}

// ─────────────────────────────────────────────────────────────
//  Свойства
// ─────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────
//  getStudent — данные одного студента для формы редактирования
// ─────────────────────────────────────────────────────────────
QVariantMap StudentController::getStudent(int index) const
{
    if (index < 0 || index >= (int)m_students.size()) return {};
    return studentToMap(index);
}

// ─────────────────────────────────────────────────────────────
//  Файлы
// ─────────────────────────────────────────────────────────────
void StudentController::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error("Не удалось открыть файл: " + path);
        return;
    }
    m_students.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            m_students.push_back(Student::deserialize(line.toStdString()));
        } catch (const std::exception& e) {
            emit error(QString("Ошибка в строке «%1»: %2").arg(line, e.what()));
        }
    }
    emit studentsChanged();
}

void StudentController::saveFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error("Не удалось сохранить: " + path);
        return;
    }
    QTextStream out(&file);
    for (const auto& s : m_students)
        out << QString::fromStdString(s.serialize()) << "\n";
}

// ─────────────────────────────────────────────────────────────
//  CRUD
// ─────────────────────────────────────────────────────────────
void StudentController::addStudent(const QString& group,
                                    const QString& name,
                                    const QVariantList& winter,
                                    const QVariantList& summer)
{
    if (winter.size() != 5 || summer.size() != 5) {
        emit error("Нужно ровно 5 оценок за каждую сессию");
        return;
    }
    std::array<int,5> w{}, s{};
    for (int i = 0; i < 5; ++i) {
        w[i] = winter[i].toInt();
        s[i] = summer[i].toInt();
        if (w[i] < 1 || w[i] > 10 || s[i] < 1 || s[i] > 10) {
            emit error("Оценка должна быть от 1 до 10");
            return;
        }
    }
    m_students.push_back(Student(group.toStdString(), name.toStdString(), w, s));
    emit studentsChanged();
}

void StudentController::deleteStudent(int index)
{
    if (index < 0 || index >= (int)m_students.size()) return;
    m_students.erase(m_students.begin() + index);
    emit studentsChanged();
}

void StudentController::editStudent(int index,
                                     const QString& group,
                                     const QString& name,
                                     const QVariantList& winter,
                                     const QVariantList& summer)
{
    if (index < 0 || index >= (int)m_students.size()) return;
    if (winter.size() != 5 || summer.size() != 5) {
        emit error("Нужно ровно 5 оценок за каждую сессию");
        return;
    }
    std::array<int,5> w{}, s{};
    for (int i = 0; i < 5; ++i) {
        w[i] = winter[i].toInt();
        s[i] = summer[i].toInt();
    }
    m_students[index] = Student(group.toStdString(), name.toStdString(), w, s);
    emit studentsChanged();
}

// ─────────────────────────────────────────────────────────────
//  Фильтрация
// ─────────────────────────────────────────────────────────────
QVariantList StudentController::filtered(int filterType,
                                          const QString& nameSearch) const
{
    QVariantList list;
    for (int i = 0; i < (int)m_students.size(); ++i) {
        const Student& s = m_students[i];

        // Фильтр по имени
        if (!nameSearch.isEmpty()) {
            QString nm = QString::fromStdString(s.fullName()).toLower();
            if (!nm.contains(nameSearch.toLower())) continue;
        }
        // Фильтр по статусу
        if (filterType == 1 && !s.hasWinterDebt()) continue;
        if (filterType == 2 && !s.shouldExpel())   continue;

        list.append(studentToMap(i));
    }
    return list;
}

// ─────────────────────────────────────────────────────────────
//  Сортировка
//  Логика: если нажата та же колонка — инвертируем направление.
//          если нажата другая — сортируем по возрастанию.
// ─────────────────────────────────────────────────────────────
void StudentController::applySort()
{
    if (m_sortColumn == "name") {
        std::sort(m_students.begin(), m_students.end(),
            [this](const Student& a, const Student& b){
                return m_sortAsc ? a.fullName() < b.fullName()
                                 : a.fullName() > b.fullName();
            });
    }
    else if (m_sortColumn == "group") {
        std::sort(m_students.begin(), m_students.end(),
            [this](const Student& a, const Student& b){
                if (a.group() != b.group())
                    return m_sortAsc ? a.group() < b.group()
                                     : a.group() > b.group();
                return a.fullName() < b.fullName();
            });
    }
    else if (m_sortColumn == "avg") {
        std::sort(m_students.begin(), m_students.end(),
            [this](const Student& a, const Student& b){
                return m_sortAsc ? a.avgSummer() < b.avgSummer()
                                 : a.avgSummer() > b.avgSummer();
            });
    }

    emit studentsChanged();
    emit sortChanged();
}

void StudentController::sortByName()
{
    if (m_sortColumn == "name")
        m_sortAsc = !m_sortAsc;   // повторное нажатие — смена направления
    else {
        m_sortColumn = "name";
        m_sortAsc    = true;
    }
    applySort();
}

void StudentController::sortByGroup()
{
    if (m_sortColumn == "group")
        m_sortAsc = !m_sortAsc;
    else {
        m_sortColumn = "group";
        m_sortAsc    = true;
    }
    applySort();
}

void StudentController::sortByAvgDesc()
{
    if (m_sortColumn == "avg")
        m_sortAsc = !m_sortAsc;
    else {
        m_sortColumn = "avg";
        m_sortAsc    = false;   // средний балл по умолчанию — по убыванию
    }
    applySort();
}