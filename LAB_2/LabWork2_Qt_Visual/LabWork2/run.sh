#!/bin/bash
# ─────────────────────────────────────────────────────────────
# run.sh — сборка и запуск LabWork2
# Использование:  chmod +x run.sh && ./run.sh
# ─────────────────────────────────────────────────────────────

set -e  # остановить при любой ошибке

# ── Цвета для вывода ─────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

info()    { echo -e "${CYAN}[INFO]${NC}  $1"; }
success() { echo -e "${GREEN}[OK]${NC}    $1"; }
warn()    { echo -e "${YELLOW}[WARN]${NC}  $1"; }
error()   { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }

echo ""
echo -e "${CYAN}╔══════════════════════════════════════╗${NC}"
echo -e "${CYAN}║        LabWork2 — Build & Run        ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"
echo ""

# ── 1. Определяем директорию проекта ─────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_DIR/build"

info "Проект: $PROJECT_DIR"
info "Сборка: $BUILD_DIR"

# ── 2. Ищем qmake6 / cmake ───────────────────────────────────
info "Поиск инструментов сборки..."

# Найти cmake
CMAKE_BIN=$(which cmake 2>/dev/null) || error "cmake не найден. Установите: sudo apt install cmake"
success "cmake: $CMAKE_BIN  ($(cmake --version | head -1))"

# Найти qmake6 и вычислить CMAKE_PREFIX_PATH
QMAKE=""
for candidate in qmake6 qmake; do
    if command -v "$candidate" &>/dev/null; then
        QMAKE=$(which "$candidate")
        break
    fi
done

if [ -z "$QMAKE" ]; then
    error "qmake не найден. Установите Qt6:\n  sudo apt install qt6-base-dev qt6-declarative-dev"
fi
success "qmake: $QMAKE  ($($QMAKE --version | tail -1))"

# Получить Qt prefix path из qmake
QT_PREFIX=$($QMAKE -query QT_INSTALL_PREFIX 2>/dev/null)
QT_VERSION=$($QMAKE -query QT_VERSION 2>/dev/null)
success "Qt prefix: $QT_PREFIX  (версия: $QT_VERSION)"

# Проверяем наличие нужных Qt6 модулей
info "Проверка Qt6 модулей..."
for mod in Qt6Core Qt6Quick Qt6QuickControls2; do
    if find "$QT_PREFIX" -name "${mod}Config.cmake" 2>/dev/null | grep -q .; then
        success "  $mod — найден"
    else
        warn "  $mod — НЕ найден! Попробуйте:"
        warn "  sudo apt install qt6-declarative-dev libqt6quickcontrols2-6"
    fi
done

# ── 3. Найти ninja или make ───────────────────────────────────
GENERATOR="Unix Makefiles"
BUILD_TOOL="make"
JOBS=$(nproc 2>/dev/null || echo 4)

if command -v ninja &>/dev/null; then
    GENERATOR="Ninja"
    BUILD_TOOL="ninja"
    success "Используется ninja (быстрее)"
else
    warn "ninja не найден, используется make -j$JOBS"
    warn "  Установить ninja: sudo apt install ninja-build"
fi

# ── 4. Очистка старого билда (опционально) ───────────────────
if [ "$1" = "--clean" ] || [ "$1" = "-c" ]; then
    info "Очистка директории сборки..."
    rm -rf "$BUILD_DIR"
    success "Очищено"
fi

# ── 5. CMake Configure ───────────────────────────────────────
echo ""
info "Конфигурация CMake..."
mkdir -p "$BUILD_DIR"

cmake \
    -S "$PROJECT_DIR" \
    -B "$BUILD_DIR" \
    -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="$QT_PREFIX" \
    -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
    -DCMAKE_C_COMPILER=/usr/bin/gcc \
    2>&1

success "CMake configure — OK"

# ── 6. Сборка ────────────────────────────────────────────────
echo ""
info "Сборка..."

if [ "$BUILD_TOOL" = "ninja" ]; then
    ninja -C "$BUILD_DIR"
else
    make -C "$BUILD_DIR" -j"$JOBS"
fi

success "Сборка завершена"

# ── 7. Найти бинарник ────────────────────────────────────────
BINARY=$(find "$BUILD_DIR" -maxdepth 2 -name "LabWork2App" -type f 2>/dev/null | head -1)

if [ -z "$BINARY" ]; then
    # fallback name
    BINARY=$(find "$BUILD_DIR" -maxdepth 2 -name "LabWork2" -type f 2>/dev/null | head -1)
fi

[ -z "$BINARY" ] && error "Бинарник не найден в $BUILD_DIR"
success "Бинарник: $BINARY"

# ── 8. Запуск с подробным QML логом ─────────────────────────
echo ""
echo -e "${CYAN}════════════════════════════════════════${NC}"
info "Запуск приложения..."
info "Нажмите Ctrl+C для остановки"
echo -e "${CYAN}════════════════════════════════════════${NC}"
echo ""

# QT_LOGGING_RULES показывает все QML ошибки
# QML_IMPORT_TRACE=1 показывает где ищутся QML модули
export QT_LOGGING_RULES="qt.qml.binding.removal.info=true;qml=true"
export QML_IMPORT_TRACE=0   # поставьте 1 если нужен полный трейс импортов
export QT_QPA_PLATFORM="${QT_QPA_PLATFORM:-xcb}"  # xcb для X11, wayland для Wayland

"$BINARY" 2>&1

EXIT_CODE=$?
echo ""
if [ $EXIT_CODE -eq 0 ]; then
    success "Приложение завершилось нормально (код: $EXIT_CODE)"
elif [ $EXIT_CODE -eq 255 ]; then
    echo -e "${RED}════════════════════════════════════════${NC}"
    error "Код выхода 255 — QML не загрузился.\n\n  Запустите с трейсом импортов:\n    QML_IMPORT_TRACE=1 $BINARY\n\n  Или проверьте путь в main.cpp:\n    qrc:/qt/qml/LabWork2/Main.qml"
else
    warn "Приложение завершилось с кодом: $EXIT_CODE"
fi
