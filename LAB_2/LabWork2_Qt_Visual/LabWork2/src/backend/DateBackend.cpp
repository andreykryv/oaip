#include "DateBackend.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdexcept>

// ══════════════════════════════════════════════════════════════
//  DateModel
// ══════════════════════════════════════════════════════════════
DateModel::DateModel(QObject* parent) : QAbstractListModel(parent) {}

int DateModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(m_dates.size());
}

QVariant DateModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount()) return {};
    const Date& d = m_dates[index.row()];

    switch (role) {
        case DateStr:  return QString::fromStdString(d.toString());
        case NextDay:  return QString::fromStdString(d.nextDay().toString());
        case PrevDay: {
            try { return QString::fromStdString(d.previousDay().toString()); }
            catch (...) { return "—"; }
        }
        case DiffDays: {
            // diff with next element (or — for last)
            if (index.row() + 1 < rowCount()) {
                int diff = std::abs(d.duration(m_dates[index.row() + 1]));
                return QString::number(diff);
            }
            return "—";
        }
        case IsLeap: return d.isLeap() ? QStringLiteral("Да") : QStringLiteral("Нет");
        case WeekNum: return static_cast<int>(d.weekNumber());
        default: return {};
    }
}

QHash<int,QByteArray> DateModel::roleNames() const {
    return {
        { DateStr,  "dateStr"  },
        { NextDay,  "nextDay"  },
        { PrevDay,  "prevDay"  },
        { DiffDays, "diffDays" },
        { IsLeap,   "isLeap"   },
        { WeekNum,  "weekNum"  },
    };
}

void DateModel::setDates(const std::vector<Date>& dates) {
    beginResetModel();
    m_dates = dates;
    endResetModel();
}

// ══════════════════════════════════════════════════════════════
//  DateBackend
// ══════════════════════════════════════════════════════════════
DateBackend::DateBackend(QObject* parent)
    : QObject(parent)
    , m_model(new DateModel(this))
{}

QString DateBackend::todayStr() const {
    return QString::fromStdString(Date::today().toString());
}

// ── parseOrThrow ─────────────────────────────────────────────
Date DateBackend::parseOrThrow(const QString& s) const {
    try {
        return Date::fromString(s.toStdString());
    } catch (const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
}

// ── refresh ──────────────────────────────────────────────────
void DateBackend::refresh() {
    m_model->setDates(m_dates);
}

// ── File: load ───────────────────────────────────────────────
void DateBackend::loadFromFile(const QString& filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred("Не удалось открыть файл: " + filePath);
        return;
    }

    m_dates.clear();
    QTextStream in(&f);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        // Support space-separated dates on one line
        const QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
        for (const QString& tok : tokens) {
            try {
                m_dates.push_back(parseOrThrow(tok));
            } catch (const std::exception& e) {
                qWarning() << "Skipped invalid date token:" << tok << "->" << e.what();
            }
        }
    }

    refresh();
    emit fileLoaded(filePath);
}

// ── File: save ───────────────────────────────────────────────
void DateBackend::saveToFile(const QString& filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred("Не удалось сохранить файл: " + filePath);
        return;
    }

    QTextStream out(&f);
    for (const Date& d : m_dates)
        out << QString::fromStdString(d.toString()) << "\n";

    emit dataSaved();
}

// ── File: append (no full rewrite) ───────────────────────────
void DateBackend::appendToFile(const QString& filePath, const QString& dateStr)
{
    try {
        Date d = parseOrThrow(dateStr);

        QFile f(filePath);
        if (!f.open(QIODevice::Append | QIODevice::Text)) {
            emit errorOccurred("Не удалось открыть файл для дозаписи: " + filePath);
            return;
        }
        QTextStream out(&f);
        out << QString::fromStdString(d.toString()) << "\n";

        m_dates.push_back(d);
        refresh();
    } catch (const std::exception& e) {
        emit errorOccurred(QString("Ошибка: ") + e.what());
    }
}

// ── CRUD ─────────────────────────────────────────────────────
void DateBackend::addDate(const QString& dateStr)
{
    try {
        m_dates.push_back(parseOrThrow(dateStr));
        refresh();
    } catch (const std::exception& e) {
        emit errorOccurred(QString("Некорректная дата: ") + e.what());
    }
}

void DateBackend::removeDate(int index)
{
    if (index < 0 || index >= static_cast<int>(m_dates.size())) return;
    m_dates.erase(m_dates.begin() + index);
    refresh();
}

void DateBackend::editDate(int index, const QString& newDateStr)
{
    if (index < 0 || index >= static_cast<int>(m_dates.size())) return;
    try {
        m_dates[index] = parseOrThrow(newDateStr);
        m_model->dataChanged(
            m_model->index(index),
            m_model->index(index)
        );
    } catch (const std::exception& e) {
        emit errorOccurred(QString("Некорректная дата: ") + e.what());
    }
}

// ── calcForRow ───────────────────────────────────────────────
QVariantMap DateBackend::calcForRow(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_dates.size()))
        return {};

    const Date& d = m_dates[index];
    QVariantMap result;

    result["date"]   = QString::fromStdString(d.toString());
    result["next"]   = QString::fromStdString(d.nextDay().toString());
    result["isLeap"] = d.isLeap();
    result["week"]   = static_cast<int>(d.weekNumber());

    try {
        result["prev"] = QString::fromStdString(d.previousDay().toString());
    } catch (...) {
        result["prev"] = "—";
    }

    if (index + 1 < static_cast<int>(m_dates.size()))
        result["diff"] = std::abs(d.duration(m_dates[index + 1]));
    else
        result["diff"] = "—";

    return result;
}

// ── daysTillBirthday ─────────────────────────────────────────
int DateBackend::daysTillBirthday(const QString& birthdayStr) const
{
    try {
        Date bday = parseOrThrow(birthdayStr);
        Date now  = Date::today();
        return now.daysTillYourBirthday(bday);
    } catch (const std::exception& e) {
        emit const_cast<DateBackend*>(this)->errorOccurred(
            QString("Ошибка дня рождения: ") + e.what()
        );
        return -1;
    }
}
