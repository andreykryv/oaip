#include "DateController.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdexcept>
#include <QFileDialog>
#include <QStandardPaths>

DateController::DateController(QObject* parent) : QObject(parent)
{
    //стартовая набивка
    try {
        m_dates.push_back(Date::today());
    } catch (...) {}
}



QString DateController::todayStr() const
{
    return QString::fromStdString(Date::today().toString());
}

QVariantList DateController::dates() const
{
    QVariantList list;
    for (int i = 0; i < (int)m_dates.size(); ++i)
        list.append(dateToMap(i));
    return list;
}

QVariantMap DateController::dateToMap(int i) const
{
    QVariantMap m;
    const Date& d = m_dates[i];
    m["dateVal"] = QString::fromStdString(d.toString());

    


    // Пока — заглушки
    m["nextDay"] = QString::fromStdString(d.nextDay().toString()) ;
    m["prevDay"] = QString::fromStdString(d.previousDay().toString());
    m["leap"] = d.isLeap() ? "Да" : "Нет";;
    m["weekN"]   = QString::number(d.weekNumber());;

    if (i > 0) {
        // m["diff"] = 
        m["diff"] = QString::number(m_dates[i-1].duration(d));
    } else {
        m["diff"] = "Первый элемент!";
    }
    return m;
}



  
//работа с файлами

void DateController::openFileDialog() {
    QString path = QFileDialog::getOpenFileName(nullptr,
        "Открыть файл с датами",
        m_lastPath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) : m_lastPath,
        "Текстовые файлы (*.txt);;Все файлы (*)");
    if (path.isEmpty()) return;
    m_lastPath = path;
    openFile(path); //метод
}

void DateController::saveFileDialog() {
    QString path = QFileDialog::getSaveFileName(nullptr,
        "Сохранить файл с датами",
        m_lastPath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) : m_lastPath,
        "Текстовые файлы (*.txt);;Все файлы (*)");
    if (path.isEmpty()) return;
    m_lastPath = path;
    saveFile(path);  //метод
}

void DateController::appendToFileDialog(const QString& dateStr) {
    QString path = QFileDialog::getSaveFileName(nullptr,
        "Дозаписать дату в файл",
        m_lastPath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) : m_lastPath,
        "Текстовые файлы (*.txt);;Все файлы (*)");
    if (path.isEmpty()) return;
    m_lastPath = path;
    appendToFile(path, dateStr);  //метод
}















void DateController::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error("Не удалось открыть файл: " + path);
        return;
    }
    m_dates.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            m_dates.push_back(Date::fromString(line.toStdString()));
        } catch (const std::exception& e) {
            emit error(QString("Ошибка в строке «%1»: %2").arg(line).arg(e.what()));
        }
    }
    emit datesChanged();
}
void DateController::saveFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error("Не удалось сохранить файл: " + path);
        return;
    }
    QTextStream out(&file);
    for (const auto& d : m_dates)
        out << QString::fromStdString(d.toString()) << "\n";
}
void DateController::appendToFile(const QString& path, const QString& dateStr)
{
    try {
        Date d = Date::fromString(dateStr.toStdString());
        QFile file(path);
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            emit error("Не удалось дозаписать в файл: " + path);
            return;
        }
        QTextStream out(&file);
        out << dateStr << "\n";
        m_dates.push_back(d);
        emit datesChanged();
    } catch (const std::exception& e) {
        emit error(QString("Ошибка даты: %1").arg(e.what()));
    }
}





//добавить,удалить,изменить
void DateController::addDate(const QString& dateStr)
{
    try {
        m_dates.push_back(Date::fromString(dateStr.toStdString()));
        emit datesChanged();
    } catch (const std::exception& e) {
        emit error(QString("Неверный формат даты: %1").arg(e.what()));
    }
}
void DateController::deleteDate(int index)
{
    if (index < 0 || index >= (int)m_dates.size()) return;
    m_dates.erase(m_dates.begin() + index);
    emit datesChanged();
}
void DateController::editDate(int index, const QString& newDateStr)
{
    if (index < 0 || index >= (int)m_dates.size()) return;
    try {
        m_dates[index] = Date::fromString(newDateStr.toStdString());
        emit datesChanged();
    } catch (const std::exception& e) {
        emit error(QString("Неверный формат даты: %1").arg(e.what()));
    }
}








//вычисления
QString DateController::getNextDay(const QString& dateStr) const
{
    try {
        Date d = Date::fromString(dateStr.toStdString());
        return QString::fromStdString(d.nextDay().toString());
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(e.what());
    }
}

QString DateController::getPrevDay(const QString& dateStr) const
{
    try {
        Date d = Date::fromString(dateStr.toStdString());
        return QString::fromStdString(d.previousDay().toString());
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(e.what());
    }
}

QString DateController::getIsLeap(const QString& dateStr) const
{
    try {
        Date d = Date::fromString(dateStr.toStdString());
        return d.isLeap() ? "Да" : "Нет";
    } catch (...) { return "—"; }
}

QString DateController::getWeekNumber(const QString& dateStr) const
{
    try {
        Date d = Date::fromString(dateStr.toStdString());
        return QString::number(d.weekNumber());
    } catch (...) { return "—"; }
}

QString DateController::getDaysTillBirthday(const QString& dateStr,const QString& birthday) const
{
    try {
        Date current  = Date::fromString(dateStr.toStdString());
        Date bday     = Date::fromString(birthday.toStdString());
        return QString::number(current.daysTillYourBirthday(bday));
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(e.what());
    }
}

QString DateController::getDuration(const QString& dateA,const QString& dateB) const
{
    try {
        Date a = Date::fromString(dateA.toStdString());
        Date b = Date::fromString(dateB.toStdString());
        return QString::number(a.duration(b));
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(e.what());
    }
}

QVariantMap DateController::getResult(const QString& dateStr, const QString& birthday) const
{
    QVariantMap r;
    try {
        Date d = Date::fromString(dateStr.toStdString());
        r["next"] = QString::fromStdString(d.nextDay().toString());
        r["prev"] = QString::fromStdString(d.previousDay().toString());
        r["leap"]= d.isLeap() ? "Да" : "Нет";
        r["week"]= QString::number(d.weekNumber());
        if (!birthday.isEmpty()) {
        try {
                Date b = Date::fromString(birthday.toStdString());
                r["tillBday"] = QString::number(d.daysTillYourBirthday(b));
            } catch (...) { r["tillBday"] = "—"; }
        } else {
                  r["tillBday"] = "—";
        }
    } catch (const std::exception& e) {
                  r["error"] = QString(e.what());
    }
    return r;
}
