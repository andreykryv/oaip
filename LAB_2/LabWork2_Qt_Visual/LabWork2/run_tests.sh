#!/bin/bash
# ══════════════════════════════════════════════════════════════
#  run_tests.sh — Сборка и запуск тестов
#  Белый ящик: Date, Student, Expression
#  Чёрный ящик: все вкладки + диалог ошибок
# ══════════════════════════════════════════════════════════════
set -e

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CXX=${CXX:-g++}
FLAGS="-std=c++17 -Wall -Wextra -O0 -fsanitize=address,undefined"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo ""
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  LabWork2 — Функциональное тестирование              ${NC}"
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo ""


echo -e "${YELLOW}[1/4] Компиляция: Белый ящик...${NC}"
$CXX $FLAGS \
    "$TESTS_DIR/test_whitebox.cpp" \
    "$TESTS_DIR/src/backend/Date.cpp" \
    "$TESTS_DIR/src/backend/Student.cpp" \
    "$TESTS_DIR/src/backend/Expression.cpp" \
    -I"$TESTS_DIR" \
    -o "$TESTS_DIR/test_wb"
echo -e "${GREEN}[OK]${NC} Белый ящик скомпилирован"


echo -e "${YELLOW}[2/4] Компиляция: Чёрный ящик...${NC}"
$CXX $FLAGS \
    "$TESTS_DIR/test_blackbox.cpp" \
    "$TESTS_DIR/src/backend/Date.cpp" \
    "$TESTS_DIR/src/backend/Student.cpp" \
    "$TESTS_DIR/src/backend/Expression.cpp" \
    -I"$TESTS_DIR" \
    -o "$TESTS_DIR/test_bb"
echo -e "${GREEN}[OK]${NC} Чёрный ящик скомпилирован"

echo -e "${YELLOW}[3/4] Компиляция: QtTest (test_buttons)...${NC}"

if command -v qmake6 &>/dev/null; then
    QT_VER=6
    QMAKE_BIN=qmake6
elif command -v qmake &>/dev/null; then
    QT_VER=5
    QMAKE_BIN=qmake
else
    echo -e "${RED}[ERROR]${NC} qmake не найден, пропускаем test_buttons_qtest"
    QT_CODE=1
fi

if [ -n "$QMAKE_BIN" ]; then
    QT_INCLUDE=$($QMAKE_BIN -query QT_INSTALL_HEADERS)
    QT_LIB=$($QMAKE_BIN -query QT_INSTALL_LIBS)

    # Добавляем -I"$TESTS_DIR" для поиска src/ и других локальных заголовков
    CXXFLAGS_QT="-std=c++17 -fPIC -I$QT_INCLUDE -I$QT_INCLUDE/QtCore -I$QT_INCLUDE/QtTest -I\"$TESTS_DIR\""
    LDFLAGS_QT="-L$QT_LIB -lQt${QT_VER}Test -lQt${QT_VER}Core"

    # Если нужны виджеты (QPushButton и т.п.), раскомментируйте:
    # CXXFLAGS_QT="$CXXFLAGS_QT -I$QT_INCLUDE/QtWidgets"
    # LDFLAGS_QT="$LDFLAGS_QT -lQt${QT_VER}Widgets"

    $CXX $FLAGS $CXXFLAGS_QT \
        "$TESTS_DIR/test_buttons_qtest.cpp" \
        -o "$TESTS_DIR/test_buttons_qt" \
        $LDFLAGS_QT

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK]${NC} test_buttons_qtest скомпилирован"
    else
        echo -e "${RED}[ERROR]${NC} Ошибка компиляции test_buttons_qtest"
        exit 1
    fi
fi

# Запуск QtTest
if [ -f "$TESTS_DIR/test_buttons_qt" ]; then
    echo ""
    echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}  Запуск: QtTest (test_buttons)                        ${NC}"
    echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
    "$TESTS_DIR/test_buttons_qt"
    QT_CODE=$?
else
    QT_CODE=1
fi


echo ""
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  Запуск: БЕЛЫЙ ЯЩИК                                  ${NC}"
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
"$TESTS_DIR/test_wb"
WB_CODE=$?

echo ""
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  Запуск: ЧЁРНЫЙ ЯЩИК                                 ${NC}"
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
"$TESTS_DIR/test_bb"
BB_CODE=$?

echo ""
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  ОБЩИЙ ИТОГ                                          ${NC}"
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
if [ $WB_CODE -eq 0 ] && [ $BB_CODE -eq 0 ]; then
    echo -e "${GREEN}  ✅  ВСЕ НАБОРЫ ТЕСТОВ ПРОЙДЕНЫ!${NC}"
    exit 0
else
    [ $WB_CODE -ne 0 ] && echo -e "${RED}  ❌  Белый ящик: есть провалы${NC}"
    [ $BB_CODE -ne 0 ] && echo -e "${RED}  ❌  Чёрный ящик: есть провалы${NC}"
    exit 1
fi
echo -e "${YELLOW}[3/4] Компиляция: QtTest (test_buttons)...${NC}"

# Найти qmake, чтобы получить пути Qt
if command -v qmake6 &>/dev/null; then
    QT_VER=6
    QMAKE_BIN=qmake6
elif command -v qmake &>/dev/null; then
    QT_VER=5
    QMAKE_BIN=qmake
else
    echo -e "${RED}[ERROR]${NC} qmake не найден, QtTest не будет собран"
    exit 1
fi

QT_INCLUDE=$($QMAKE_BIN -query QT_INSTALL_HEADERS)
QT_LIB=$($QMAKE_BIN -query QT_INSTALL_LIBS)

# Флаги для QtTest (без графики используем QTEST_APPLESS_MAIN)
CXXFLAGS_QT="-std=c++17 -fPIC -I$QT_INCLUDE -I$QT_INCLUDE/QtCore -I$QT_INCLUDE/QtTest"
LDFLAGS_QT="-L$QT_LIB -lQt${QT_VER}Test -lQt${QT_VER}Core"

$CXX $FLAGS $CXXFLAGS_QT \
    "$TESTS_DIR/test_buttons_qtest.cpp" \
    -o "$TESTS_DIR/test_buttons_qt" \
    $LDFLAGS_QT

echo -e "${GREEN}[OK]${NC} QtTest скомпилирован"

# Запуск
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  Запуск: QtTest (test_buttons)                        ${NC}"
echo -e "${CYAN}══════════════════════════════════════════════════════${NC}"
"$TESTS_DIR/test_buttons_qt"
QT_CODE=$?
if [ $WB_CODE -eq 0 ] && [ $BB_CODE -eq 0 ] && [ $QT_CODE -eq 0 ]; then
    echo -e "${GREEN}  ✅  ВСЕ НАБОРЫ ТЕСТОВ ПРОЙДЕНЫ!${NC}"
    exit 0
else
    [ $WB_CODE -ne 0 ] && echo -e "${RED}  ❌  Белый ящик: есть провалы${NC}"
    [ $BB_CODE -ne 0 ] && echo -e "${RED}  ❌  Чёрный ящик: есть провалы${NC}"
    [ $QT_CODE -ne 0 ] && echo -e "${RED}  ❌  QtTest (test_buttons): провалы${NC}"
    exit 1
fi
