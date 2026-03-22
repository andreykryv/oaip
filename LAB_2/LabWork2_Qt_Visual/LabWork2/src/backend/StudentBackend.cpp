#include "StudentBackend.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <stdexcept>


StudentModel::StudentModel(QObject* parent) : QAbstractListModel(parent) {}

int StudentModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(m_students.size());
}

QVariant StudentModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid() || idx.row() >= rowCount()) return {};
    const Student& s = m_students[idx.row()];

    switch (role) {
        case Group:    return QString::fromStdString(s.group());
        case FullName: return QString::fromStdString(s.fullName());
        case W1: return s.winterGrades()[0]; case W2: return s.winterGrades()[1];
        case W3: return s.winterGrades()[2]; case W4: return s.winterGrades()[3];
        case W5: return s.winterGrades()[4];
        case S1: return s.summerGrades()[0]; case S2: return s.summerGrades()[1];
        case S3: return s.summerGrades()[2]; case S4: return s.summerGrades()[3];
        case S5: return s.summerGrades()[4];
        case AvgWinter: return QString::number(s.avgWinter(), 'f', 2);
        case AvgSummer: return QString::number(s.avgSummer(), 'f', 2);
        case Status:
            if (s.shouldExpel())   return QStringLiteral("expel");
            if (s.hasWinterDebt()) return QStringLiteral("debt");
            return QStringLiteral("ok");
        default: return {};
    }
}

QHash<int,QByteArray> StudentModel::roleNames() const {
    return {
        { Group,    "group"    }, { FullName, "fullName"  },
        { W1,"w1"  }, { W2,"w2"  }, { W3,"w3"  }, { W4,"w4"  }, { W5,"w5"  },
        { S1,"s1"  }, { S2,"s2"  }, { S3,"s3"  }, { S4,"s4"  }, { S5,"s5"  },
        { AvgWinter,"avgWinter" }, { AvgSummer,"avgSummer" },
        { Status,  "status"    },
    };
}

void StudentModel::setStudents(const std::vector<Student>& students) {
    beginResetModel();
    m_students = students;
    endResetModel();
}

\
StudentBackend::StudentBackend(QObject* parent)
    : QObject(parent)
    , m_model(new StudentModel(this))
{}

void StudentBackend::refreshModel() {
    m_model->setStudents(m_current);
    emit statsChanged();
}

std::array<int,Student::GRADES_PER_SESSION>
StudentBackend::toArray(const QVariantList& list) const
{
    std::array<int,Student::GRADES_PER_SESSION> arr{3,3,3,3,3};
    for (int i = 0; i < Student::GRADES_PER_SESSION && i < list.size(); ++i)
        arr[i] = list[i].toInt();
    return arr;
}

// ── Stats ────────────────────────────────────────────────────
int StudentBackend::debtCount()  const {
    return std::count_if(m_all.begin(), m_all.end(), [](const Student& s){ return s.hasWinterDebt() && !s.shouldExpel(); });
}
int StudentBackend::expelCount() const {
    return std::count_if(m_all.begin(), m_all.end(), [](const Student& s){ return s.shouldExpel(); });
}
int StudentBackend::okCount() const {
    return std::count_if(m_all.begin(), m_all.end(), [](const Student& s){ return !s.hasWinterDebt() && !s.shouldExpel(); });
}

// работа с файлами 
void StudentBackend::loadFromFile(const QString& filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred("Не удалось открыть файл: " + filePath);
        return;
    }

    m_all.clear();
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            m_all.push_back(Student::deserialize(line.toStdString()));
        } catch (const std::exception& e) {
            emit errorOccurred(QString("Ошибка парсинга строки: ") + e.what());
        }
    }

    m_current = m_all;
    refreshModel();
}

void StudentBackend::saveToFile(const QString& filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred("Не удалось сохранить файл: " + filePath);
        return;
    }

    QTextStream out(&f);
    for (const Student& s : m_all)
        out << QString::fromStdString(s.serialize()) << "\n";
}

// ── CRUD ─────────────────────────────────────────────────────
void StudentBackend::addStudent(const QString& group, const QString& name,
                                const QVariantList& winter, const QVariantList& summer)
{
    m_all.emplace_back(group.toStdString(), name.toStdString(),
                       toArray(winter), toArray(summer));
    m_current = m_all;
    refreshModel();
}

void StudentBackend::removeStudent(int index)
{
    if (index < 0 || index >= static_cast<int>(m_all.size())) return;
    m_all.erase(m_all.begin() + index);
    m_current = m_all;
    refreshModel();
}

void StudentBackend::editStudent(int index, const QString& group, const QString& name,
                                  const QVariantList& winter, const QVariantList& summer)
{
    if (index < 0 || index >= static_cast<int>(m_all.size())) return;
    m_all[index].setGroup   (group.toStdString());
    m_all[index].setFullName(name.toStdString());
    m_all[index].setWinterGrades(toArray(winter));
    m_all[index].setSummerGrades(toArray(summer));
    m_current = m_all;
    refreshModel();
}

// фильтры и сорт
void StudentBackend::applyFilter(int filterMode)
{
    m_current.clear();
    for (const Student& s : m_all) {
        if (filterMode == 1 && !s.hasWinterDebt()) continue;
        if (filterMode == 2 && !s.shouldExpel())   continue;
        m_current.push_back(s);
    }
    refreshModel();
}

void StudentBackend::sortByName()
{
    std::sort(m_current.begin(), m_current.end(),
        [](const Student& a, const Student& b){ return a.fullName() < b.fullName(); });
    refreshModel();
}

void StudentBackend::sortByGroup()
{
    std::stable_sort(m_current.begin(), m_current.end(),
        [](const Student& a, const Student& b){
            if (a.group() != b.group()) return a.group() < b.group();
            return a.fullName() < b.fullName();
        });
    refreshModel();
}

void StudentBackend::sortByAvgSummer()
{
    std::sort(m_current.begin(), m_current.end(),
        [](const Student& a, const Student& b){ return a.avgSummer() > b.avgSummer(); });
    refreshModel();
}

// ── Search ───────────────────────────────────────────────────
int StudentBackend::findByName(const QString& name) const
{
    std::string needle = name.toLower().toStdString();
    for (int i = 0; i < static_cast<int>(m_all.size()); ++i) {
        std::string hay = m_all[i].fullName();
        std::transform(hay.begin(), hay.end(), hay.begin(), ::tolower);
        if (hay.find(needle) != std::string::npos)
            return i;
    }
    return -1;
}
