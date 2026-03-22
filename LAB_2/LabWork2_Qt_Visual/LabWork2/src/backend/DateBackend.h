#pragma once
#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QString>
#include <QAbstractListModel>
#include "Date.h"
#include <vector>

// ─────────────────────────────────────────────────────────────
//  DateModel — exposes a dynamic array of Date objects to QML
// ─────────────────────────────────────────────────────────────
class DateModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        DateStr = Qt::UserRole + 1,
        NextDay,
        PrevDay,
        DiffDays,
        IsLeap,
        WeekNum
    };

    explicit DateModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int,QByteArray> roleNames() const override;

    // Called from DateBackend
    void setDates(const std::vector<Date>& dates);
    const std::vector<Date>& dates() const { return m_dates; }

private:
    std::vector<Date> m_dates;
};

// ─────────────────────────────────────────────────────────────
//  DateBackend — main bridge between QML and C++ logic
// ─────────────────────────────────────────────────────────────
class DateBackend : public QObject
{
    Q_OBJECT

    // Expose model to QML
    Q_PROPERTY(DateModel* model READ model CONSTANT)
    Q_PROPERTY(QString    today READ todayStr CONSTANT)

public:
    explicit DateBackend(QObject* parent = nullptr);
    DateModel* model() const { return m_model; }
    QString todayStr() const;

public slots:
    // ── File operations ──────────────────────────────────────
    // Call these from QML buttons; filePath comes from QML FileDialog
    void loadFromFile(const QString& filePath);
    void saveToFile  (const QString& filePath);
    void appendToFile(const QString& filePath, const QString& dateStr);

    // ── CRUD ────────────────────────────────────────────────
    void addDate   (const QString& dateStr);
    void removeDate(int index);
    void editDate  (int index, const QString& newDateStr);

    // ── Calculations (return result as QVariantMap to QML) ───
    QVariantMap calcForRow     (int index) const;
    int         daysTillBirthday(const QString& birthdayStr) const;

signals:
    void errorOccurred(const QString& message);   // shown in QML via Connections
    void fileLoaded   (const QString& filePath);
    void dataSaved    ();

private:
    DateModel*        m_model;
    std::vector<Date> m_dates;   // master copy

    void refresh();
    Date parseOrThrow(const QString& s) const;
};
