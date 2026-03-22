#include "StudentController.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QStandardPaths>
#include <algorithm>
#include <stdexcept>

//демо данные констркуктор 
StudentController::StudentController(QObject* parent) : QObject(parent)
{
    using A = std::array<int,5>;
    m_students.push_back(Student("ИВТ-21","Иванов А.А.",    A{5,4,3,5,4}, A{5,4,4,5,5}));
    m_students.push_back(Student("ИВТ-21","Петрова М.С.",   A{3,2,4,3,4}, A{4,3,3,4,4}));
    m_students.push_back(Student("ИВТ-22","Сидоров Д.О.",   A{2,3,2,4,3}, A{2,3,2,4,3}));
    m_students.push_back(Student("ИВТ-22","Козлова Е.В.",   A{5,5,4,5,5}, A{5,5,5,5,4}));
    m_students.push_back(Student("ИВТ-23","Морозов К.П.",   A{4,4,3,4,3}, A{2,2,3,4,3}));
    m_students.push_back(Student("ИВТ-23","Волкова Н.Н.",   A{3,2,4,4,3}, A{3,3,2,2,3}));
}

// карта для табл
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
    m["st"]   = s.shouldExpel()   ? "expel"
              : s.hasWinterDebt() ? "debt"
              :                     "ok";

    // Реальный индекс в m_students редактирования/удаления
    
    m["idx"] = i;

    return m;
}

//св
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


//  Проверяет студента перед добавлением в коллекцию.
//  Правила:
//    - группа и ФИО не пустые
//    - все оценки в диапазоне [1..10]
//  При ошибке — испускает сигнал error и возвращает false.
//  При успехе — добавляет в m_students и возвращает true.

bool StudentController::validateAndAdd(const Student& s, const QString& sourceLabel)
{
    // Проверка полей
    if (s.group().empty()) {
        emit error(sourceLabel + ": группа не может быть пустой");
        return false;
    }
    if (s.fullName().empty()) {
        emit error(sourceLabel + ": ФИО не может быть пустым");
        return false;
    }

    // Проверка оценок
    auto checkGrades = [&](const std::array<int,5>& g, const QString& session) -> bool {
        for (int i = 0; i < 5; ++i) {
            if (g[i] < 1 || g[i] > 10) {
                emit error(QString("%1: %2 сессия, оценка %3 = %4 — должна быть от 1 до 10")
                           .arg(sourceLabel, session).arg(i+1).arg(g[i]));
                return false;
            }
        }
        return true;
    };

    if (!checkGrades(s.winterGrades(), "зимняя")) return false;
    if (!checkGrades(s.summerGrades(), "летняя"))  return false;

    // Всё корректно — добавляем в коллекцию
    m_students.push_back(s);
    return true;
}


void StudentController::openFileDialog()
{
    QString path = QFileDialog::getOpenFileName(
        nullptr,
        "Открыть файл со студентами",
        m_lastPath.isEmpty()
            ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            : m_lastPath,
        "Текстовые файлы (*.txt);;Все файлы (*)");

    if (path.isEmpty()) return;   // пользователь нажал Отмена
    m_lastPath = path;
    openFile(path);
}

void StudentController::saveFileDialog()
{
    QString path = QFileDialog::getSaveFileName(
        nullptr,
        "Сохранить файл со студентами",
        m_lastPath.isEmpty()
            ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            : m_lastPath,
        "Текстовые файлы (*.txt);;Все файлы (*)");

    if (path.isEmpty()) return;
    m_lastPath = path;
    saveFile(path);
}

//чтение + добавление в коллекцию
void StudentController::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error("Не удалось открыть файл: " + path);
        return;
    }

    // Очищаем коллекцию перед загрузкой нового файла
    m_students.clear();

    QTextStream in(&file);
    int lineNum = 0;
    int added   = 0;
    int skipped = 0;

    while (!in.atEnd()) {
        ++lineNum;
        QString line = in.readLine().trimmed();

        // Пропускаем пустые строки и комментарии
        if (line.isEmpty() || line.startsWith('#')) continue;

        QString sourceLabel = QString("Строка %1").arg(lineNum);

        try {
            // Десериализуем строку в объект Student
            Student s = Student::deserialize(line.toStdString());

            // Проверяем и добавляем в коллекцию
            if (validateAndAdd(s, sourceLabel)) {
                ++added;
            } else {
                ++skipped;
            }

        } catch (const std::exception& e) {
            // Строка имеет неверный формат (не хватает полей, не число и т.д.)
            emit error(QString("%1: неверный формат — %2").arg(sourceLabel, e.what()));
            ++skipped;
        }
    }

    // Итог загрузки
    if (skipped > 0) {
        emit error(QString("Загружено: %1 студентов. Пропущено: %2 строк с ошибками.")
                   .arg(added).arg(skipped));
    }

    emit studentsChanged();
}


//вся коллекция в файл
void StudentController::saveFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error("Не удалось сохранить файл: " + path);
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
    }

    Student student(group.toStdString(), name.toStdString(), w, s);

    // Используем ту же валидацию что и при чтении файла
    if (validateAndAdd(student, "Новый студент")) {
        emit studentsChanged();
    }
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

    Student updated(group.toStdString(), name.toStdString(), w, s);

    // Проверяем новые данные перед заменой
    QString label = QString("Студент №%1").arg(index + 1);
    if (updated.group().empty()) { emit error(label + ": группа не может быть пустой"); return; }
    if (updated.fullName().empty()) { emit error(label + ": ФИО не может быть пустым"); return; }

    m_students[index] = updated;
    emit studentsChanged();
}

QVariantMap StudentController::getStudent(int index) const
{
    if (index < 0 || index >= (int)m_students.size()) return {};
    return studentToMap(index);
}

//фильра
QVariantList StudentController::filtered(int filterType,
                                          const QString& nameSearch) const
{
    QVariantList list;
    for (int i = 0; i < (int)m_students.size(); ++i) {
        const Student& s = m_students[i];

        if (!nameSearch.isEmpty()) {
            QString nm = QString::fromStdString(s.fullName()).toLower();
            if (!nm.contains(nameSearch.toLower())) continue;
        }
        if (filterType == 1 && !s.hasWinterDebt()) continue;
        if (filterType == 2 && !s.shouldExpel())   continue;

        list.append(studentToMap(i));
    }
    return list;
}

//sortss
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

void StudentController::sortByName() {
    m_sortColumn == "name" ? m_sortAsc = !m_sortAsc
                           : (m_sortColumn = "name",  m_sortAsc = true);
    applySort();
}
void StudentController::sortByGroup() {
    m_sortColumn == "group" ? m_sortAsc = !m_sortAsc
                            : (m_sortColumn = "group", m_sortAsc = true);
    applySort();
}
void StudentController::sortByAvgDesc() {
    // Средний балл по умолчанию — по убыванию
    m_sortColumn == "avg" ? m_sortAsc = !m_sortAsc
                          : (m_sortColumn = "avg", m_sortAsc = false);
    applySort();
}