#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QStackedWidget>
#include <QListWidget>
#include <QSplitter>
#include <QStatusBar>
#include <QScrollBar>
#include <QFrame>
#include <QComboBox>
#include <QApplication>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QFont>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QVector>

// ═══════════════════════════════════════════════════════════════
//  Структура «Государство» — Вариант 7
//  Поля: int, double, char, bool, char[] (статич. строк. массив),
//        double[] (статич. числовой массив)
// ═══════════════════════════════════════════════════════════════
struct Gosudarstvo {
    int     population;      // int:    население (тыс. чел.)
    double  area;            // double: площадь (тыс. кв.км)
    char    continentCode;   // char:   код континента (E/A/N/S/O/F)
    bool    isUNMember;      // bool:   является членом ООН
    char    name[50];        // char[]: название государства (статич. строк. массив)
    double  gdpHistory[5];   // double[]: ВВП за 5 лет, млрд $ (статич. числ. массив)
};

// ═══════════════════════════════════════════════════════════════
//  Цветовая палитра  (тёмная «терминальная» тема)
// ═══════════════════════════════════════════════════════════════
namespace Theme {
    const QString BG_MAIN   = "#0a0a14";
    const QString BG_CARD   = "#12121f";
    const QString BG_INPUT  = "#1a1a2e";
    const QString BG_OUTPUT = "#080810";
    const QString BORDER    = "#2d2d4a";
    const QString ACCENT    = "#8b5cf6";
    const QString ACCENT2   = "#06b6d4";
    const QString ACCENT3   = "#f472b6";
    const QString SUCCESS   = "#10b981";
    const QString WARNING   = "#fbbf24";
    const QString ERROR_CLR = "#ef4444";
    const QString TEXT_PRI  = "#f1f5f9";
    const QString TEXT_SEC  = "#94a3b8";
    const QString TEXT_DIM  = "#475569";
    const QString HIGHLIGHT = "#c4b5fd";
}

// ═══════════════════════════════════════════════════════════════
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // ── Задание 1 ──────────────────────────────────────────────
    void runTask1_explicitAndFields();
    void runTask1_userInput();
    void runTask1_pointer();
    void runTask1_reference();

    // ── Задание 2 — меню ───────────────────────────────────────
    void t2_menuChanged(int row);
    void t2_writeText();
    void t2_readText();
    void t2_writeArray();
    void t2_readArray();
    void t2_writeStructText();
    void t2_readStructsText();
    void t2_writeStructBin();
    void t2_readStructsBin();
    void t2_writeSentence();
    void t2_readSentences();

private:
    // ── Построители вкладок ────────────────────────────────────
    QWidget* createTask1Widget();
    QWidget* createTask2Widget();

    // ── Страницы StackedWidget (Задание 2) ─────────────────────
    QWidget* makeT2Page_welcome();
    QWidget* makeT2Page_writeText();
    QWidget* makeT2Page_readText();
    QWidget* makeT2Page_writeArray();
    QWidget* makeT2Page_readArray();
    QWidget* makeT2Page_writeStructTxt();
    QWidget* makeT2Page_readStructsTxt();
    QWidget* makeT2Page_writeStructBin();
    QWidget* makeT2Page_readStructsBin();
    QWidget* makeT2Page_writeSentence();
    QWidget* makeT2Page_readSentences();

    // ── UI-хелперы ─────────────────────────────────────────────
    void        applyAppStyle();
    QGroupBox*  makeCard(const QString &title);
    QPushButton* makeRunButton(const QString &text = "▶  Выполнить");
    QTextEdit*  makeOutput();
    void        printHeader(QTextEdit *o, const QString &title, const QString &sub);
    void        printLine(QTextEdit *o, const QString &txt,
                          const QString &color = Theme::TEXT_PRI);
    void        printKV(QTextEdit *o, const QString &key, const QString &val,
                        const QString &vc = Theme::SUCCESS);
    void        printSep(QTextEdit *o);
    void        scrollBottom(QTextEdit *o);

    // ── Вспомогательные методы ─────────────────────────────────
    QString     formatStruct(const Gosudarstvo &g, const QString &sep);
    void        fadeIn(QWidget *w);
    // Вспомогательный виджет для ввода структуры
    QWidget*    makeStructForm(
                    QLineEdit **nameEdit,
                    QSpinBox  **popSpin,
                    QDoubleSpinBox **areaSpin,
                    QComboBox **contCombo,
                    QCheckBox **unCheck,
                    QDoubleSpinBox *gdp[5]);
    Gosudarstvo readStructFromForm(
                    QLineEdit *nameEdit,
                    QSpinBox  *popSpin,
                    QDoubleSpinBox *areaSpin,
                    QComboBox *contCombo,
                    QCheckBox *unCheck,
                    QDoubleSpinBox *gdp[5]);

    // ══════════════════════════════════════════════════════════
    //  Задание 1 — виджеты
    // ══════════════════════════════════════════════════════════
    QTextEdit *t1Out;

    // Ввод с клавиатуры (под-задача 2)
    QLineEdit     *t1i_name;
    QSpinBox      *t1i_pop;
    QDoubleSpinBox *t1i_area;
    QComboBox     *t1i_cont;
    QCheckBox     *t1i_un;
    QDoubleSpinBox *t1i_gdp[5];

    // Указатель (под-задача 3)
    QLineEdit     *t1p_name;
    QSpinBox      *t1p_pop;
    QDoubleSpinBox *t1p_area;
    QComboBox     *t1p_cont;
    QCheckBox     *t1p_un;
    QDoubleSpinBox *t1p_gdp[5];

    // Ссылка (под-задача 4)
    QLineEdit     *t1r_name;
    QSpinBox      *t1r_pop;
    QDoubleSpinBox *t1r_area;
    QComboBox     *t1r_cont;
    QCheckBox     *t1r_un;
    QDoubleSpinBox *t1r_gdp[5];

    // ══════════════════════════════════════════════════════════
    //  Задание 2 — виджеты
    // ══════════════════════════════════════════════════════════
    QTextEdit      *t2Out;
    QStackedWidget *t2Stack;
    QListWidget    *t2Menu;

    // Стр. 1 — запись текста
    QLineEdit *t2_textInput;

    // Стр. 3 — запись массива
    QLineEdit *t2_arrInput;   // значения через пробел

    // Стр. 5 — запись структуры (текст)
    QLineEdit     *t2st_name;
    QSpinBox      *t2st_pop;
    QDoubleSpinBox *t2st_area;
    QComboBox     *t2st_cont;
    QCheckBox     *t2st_un;
    QDoubleSpinBox *t2st_gdp[5];

    // Стр. 7 — запись структуры (бинарный)
    QLineEdit     *t2sb_name;
    QSpinBox      *t2sb_pop;
    QDoubleSpinBox *t2sb_area;
    QComboBox     *t2sb_cont;
    QCheckBox     *t2sb_un;
    QDoubleSpinBox *t2sb_gdp[5];

    // Стр. 9 — запись предложения
    QLineEdit *t2_sentenceInput;

    // ── Имена файлов ────────────────────────────────────────────
    const QString FILE_TEXT       = "FamiliyaLab20.txt";
    const QString FILE_ARRAY      = "FamiliyaLab20_0.txt";
    const QString FILE_STRUCT_TXT = "FamiliyaLab20_1.txt";
    const QString FILE_STRUCT_BIN = "FamiliyaLab20_2.bin";
    const QString FILE_SENTENCES  = "FamiliyaLab20_3.txt";

    // Разделитель и тип массива: номер в журнале = 7 → float, '$'
    const char ARRAY_SEP = '$';
};

#endif // MAINWINDOW_H
