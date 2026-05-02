#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QSettings>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>

// ═══════════════════════════════════════════════════════════
//  Единая цветовая палитра — кибер-терминал
// ═══════════════════════════════════════════════════════════
namespace Theme {
    // Фоны
    constexpr auto BG_DEEP     = "#07080f";   // самый тёмный фон
    constexpr auto BG_MAIN     = "#0d0e1a";   // основной фон
    constexpr auto BG_CARD     = "#11131f";   // карточки
    constexpr auto BG_SURFACE  = "#171928";   // поверхности элементов
    constexpr auto BG_INPUT    = "#1a1c2e";   // поля ввода

    // Акценты
    constexpr auto NEON_GREEN  = "#00ff88";
    constexpr auto NEON_CYAN   = "#00d4ff";
    constexpr auto NEON_PURPLE = "#a855f7";
    constexpr auto NEON_PINK   = "#f472b6";
    constexpr auto NEON_YELLOW = "#fbbf24";

    // Текст
    constexpr auto TEXT_PRI    = "#e8eaf6";
    constexpr auto TEXT_SEC    = "#8892aa";
    constexpr auto TEXT_DIM    = "#3d4460";

    // Рамки
    constexpr auto BORDER      = "#1e2238";
    constexpr auto BORDER_LIT  = "#2d3356";
    constexpr auto BORDER_ACC  = "#1e3a5f";

    // Состояния
    constexpr auto SUCCESS     = "#10b981";
    constexpr auto WARNING     = "#f59e0b";
    constexpr auto ERROR_CLR   = "#ef4444";
    constexpr auto INFO        = "#3b82f6";

    // Шрифты
    constexpr auto FONT_MONO   = "'Cascadia Code','JetBrains Mono','Fira Code','Consolas',monospace";
    constexpr auto FONT_UI     = "'Segoe UI','Ubuntu','Helvetica Neue',sans-serif";
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupGlobalStyles();
    void loadSettings();
    void saveSettings();
    QWidget* buildHeader();

    QTabWidget *tabs;
};

#endif