#include "DateController.h"
#include <QFile>
#include <QTextStream>
#include <stdexcept>

DateController::DateController(QObject* parent) : QObject(parent) {}

QString DateController::todayStr() const {
    return QString::fromStdString(Date::today().toString());
}
QVariantList DateController::dates() const {
    QVariantList list;
    for (int i=0; i<static_cast<int>(m_dates.size()); ++i) list.append(dateToMap(i));
    return list;
}
QVariantMap DateController::dateToMap(int i) const {
    const Date& d = m_dates[i];
    QVariantMap m;
    m["dateVal"] = QString::fromStdString(d.toString());
    m["nextDay"] = QString::fromStdString(d.nextDay().toString());
    try   { m["prevDay"] = QString::fromStdString(d.previousDay().toString()); }
    catch (...) { m["prevDay"] = "—"; }
    m["leap"]  = d.isLeap() ? "Да" : "Нет";
    m["weekN"] = QString::number(d.weekNumber());
    m["diff"]  = i>0 ? QString::number(m_dates[i-1].duration(d)) : "—";
    return m;
}
void DateController::addDate(const QString& dateStr) {
    try   { m_dates.push_back(Date::fromString(dateStr.toStdString())); emit datesChanged(); }
    catch (const std::exception& e) { emit error(QString("Некорректная дата: %1").arg(e.what())); }
}
void DateController::deleteDate(int index) {
    if (index<0 || index>=static_cast<int>(m_dates.size())) return;
    m_dates.erase(m_dates.begin()+index); emit datesChanged();
}
void DateController::editDate(int index, const QString& newDateStr) {
    if (index<0 || index>=static_cast<int>(m_dates.size())) return;
    try   { m_dates[index]=Date::fromString(newDateStr.toStdString()); emit datesChanged(); }
    catch (const std::exception& e) { emit error(QString("Некорректная дата: %1").arg(e.what())); }
}
void DateController::openFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) { emit error("Не удалось открыть: "+path); return; }
    m_dates.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try   { m_dates.push_back(Date::fromString(line.toStdString())); }
        catch (const std::exception& e) { emit error(QString("Ошибка «%1»: %2").arg(line, e.what())); }
    }
    emit datesChanged();
}
void DateController::saveFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) { emit error("Не удалось сохранить: "+path); return; }
    QTextStream out(&file);
    for (const auto& d : m_dates) out << QString::fromStdString(d.toString()) << "\n";
}
void DateController::appendToFile(const QString& path, const QString& dateStr) {
    try {
        Date d = Date::fromString(dateStr.toStdString());
        QFile file(path);
        if (!file.open(QIODevice::Append|QIODevice::Text)) { emit error("Не удалось дозаписать: "+path); return; }
        QTextStream(&file) << QString::fromStdString(d.toString()) << "\n";
        m_dates.push_back(d); emit datesChanged();
    } catch (const std::exception& e) { emit error(QString("Ошибка: %1").arg(e.what())); }
}
QVariantMap DateController::getResult(const QString& dateStr, const QString& birthday) const {
    QVariantMap r;
    try {
        Date d = Date::fromString(dateStr.toStdString());
        r["next"] = QString::fromStdString(d.nextDay().toString());
        try   { r["prev"] = QString::fromStdString(d.previousDay().toString()); }
        catch (...) { r["prev"] = "—"; }
        r["leap"] = d.isLeap() ? "Да" : "Нет";
        r["week"] = QString::number(d.weekNumber());
        if (!birthday.isEmpty()) {
            try {
                Date b = Date::fromString(birthday.toStdString());
                r["tillBday"] = QString::number(d.daysTillYourBirthday(b));
            } catch (...) { r["tillBday"] = "—"; }
        } else { r["tillBday"] = "—"; }
    } catch (const std::exception& e) { r["error"] = QString(e.what()); }
    return r;
}
