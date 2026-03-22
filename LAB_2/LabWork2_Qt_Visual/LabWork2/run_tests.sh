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

# ── Компиляция белого ящика ───────────────────────────────────
echo -e "${YELLOW}[1/4] Компиляция: Белый ящик...${NC}"
$CXX $FLAGS \
    "$TESTS_DIR/test_whitebox.cpp" \
    "$TESTS_DIR/src/backend/Date.cpp" \
    "$TESTS_DIR/src/backend/Student.cpp" \
    "$TESTS_DIR/src/backend/Expression.cpp" \
    -I"$TESTS_DIR" \
    -o "$TESTS_DIR/test_wb"
echo -e "${GREEN}[OK]${NC} Белый ящик скомпилирован"

# ── Компиляция чёрного ящика ──────────────────────────────────
echo -e "${YELLOW}[2/4] Компиляция: Чёрный ящик...${NC}"
$CXX $FLAGS \
    "$TESTS_DIR/test_blackbox.cpp" \
    "$TESTS_DIR/src/backend/Date.cpp" \
    "$TESTS_DIR/src/backend/Student.cpp" \
    "$TESTS_DIR/src/backend/Expression.cpp" \
    -I"$TESTS_DIR" \
    -o "$TESTS_DIR/test_bb"
echo -e "${GREEN}[OK]${NC} Чёрный ящик скомпилирован"

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
