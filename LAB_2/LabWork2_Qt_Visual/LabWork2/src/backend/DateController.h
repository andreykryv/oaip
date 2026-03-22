#pragma once
// ─────────────────────────────────────────────────────────────
//  DateController.h
//  Qt-объект, связывающий Date/файлы с QML.
//  Подключён к контексту QML как "dateBE".
//
//  КАК ПОДКЛЮЧИТЬ КНОПКУ В QML:
//    AppButton {
//        text: "Открыть файл"
//        onClicked: dateBE.openFile(fileDialog.selectedFile)
//    }
// ─────────────────────────────────────────────────────────────
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <vector>
#include "Date.h"

class DateController : public QObject
{
    Q_OBJECT

    // Эти свойства читаются прямо из QML как dateBE.dates и т.д.
    Q_PROPERTY(QVariantList dates    READ dates    NOTIFY datesChanged)
    Q_PROPERTY(QString      todayStr READ todayStr NOTIFY datesChanged)

public:
    explicit DateController(QObject* parent = nullptr);

    // ── Свойства ─────────────────────────────────────────────
    QVariantList dates()    const;   // [{date,nextDay,prevDay,diff,leap,week}, ...]
    QString      todayStr() const;   // "ДД.ММ.ГГГГ"

public slots:
     Q_INVOKABLE void openFileDialog();           
    Q_INVOKABLE void saveFileDialog();           
    Q_INVOKABLE void appendToFileDialog(const QString& dateStr);
    void openFile(const QString& path);

    // Вызвать из QML: dateBE.saveFile(path)
    void saveFile(const QString& path);

    // Дозапись одной даты без перезаписи файла
    // Вызвать из QML: dateBE.appendToFile(path, "29.02.2024")
    void appendToFile(const QString& path, const QString& dateStr);

    // ── CRUD ─────────────────────────────────────────────────
    // Вызвать из QML: dateBE.addDate("29.02.2024")
    void addDate(const QString& dateStr);

    // Вызвать из QML: dateBE.deleteDate(index)
    void deleteDate(int index);

    // Вызвать из QML: dateBE.editDate(index, "31.12.2025")
    void editDate(int index, const QString& newDateStr);

    // ── Вычисления (возвращают строку для вывода) ─────────────
    // Вызвать из QML:
    //   resultText = dateBE.getNextDay("24.04.2000")
    QString getNextDay    (const QString& dateStr) const;
    QString getPrevDay    (const QString& dateStr) const;
    QString getIsLeap     (const QString& dateStr) const;   // "Да" / "Нет"
    QString getWeekNumber (const QString& dateStr) const;
    QString getDaysTillBirthday(const QString& dateStr,
                                const QString& birthday) const;
    QString getDuration   (const QString& dateA,
                           const QString& dateB) const;

    // Полный результат для правой панели (один вызов = вся карточка)
    // Вызвать из QML: var r = dateBE.getResult(model.dateVal, birthdayInput.text)
    // Возвращает QVariantMap: {next, prev, leap, week, tillBday}
    Q_INVOKABLE QVariantMap getResult(const QString& dateStr,
                                      const QString& birthday) const;

signals:
    void datesChanged();
    void error(const QString& message);   // показать AppErrorDialog

private:
   std::vector<Date> m_dates;
    QVariantMap dateToMap(int index) const;
    QString m_lastPath;  
};
