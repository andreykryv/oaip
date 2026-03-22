#!/bin/bash
# ─────────────────────────────────────────────────────────────
# debug.sh — запуск с полным QML-трейсом для диагностики
# Использование: chmod +x debug.sh && ./debug.sh
# ─────────────────────────────────────────────────────────────

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

echo "=== LabWork2 DEBUG RUN ==="
echo ""

# Найти бинарник
BINARY=$(find "$BUILD_DIR" -maxdepth 3 \( -name "LabWork2App" -o -name "LabWork2" \) -type f 2>/dev/null | head -1)

if [ -z "$BINARY" ]; then
    echo "[ERROR] Бинарник не найден. Сначала запустите: ./run.sh"
    exit 1
fi

echo "Бинарник: $BINARY"
echo ""

# Полный трейс QML
export QML_IMPORT_TRACE=1
export QT_LOGGING_RULES="*=true"
export QT_QPA_PLATFORM="${QT_QPA_PLATFORM:-xcb}"

echo "=== ВЫВОД ПРИЛОЖЕНИЯ ==="
"$BINARY" 2>&1 | tee /tmp/labwork2_debug.log
EXIT=$?

echo ""
echo "=== Код выхода: $EXIT ==="
echo "Полный лог сохранён: /tmp/labwork2_debug.log"
echo ""
echo "=== Строки с ошибками: ==="
grep -i 'error\|cannot\|failed\|qml\|warning' /tmp/labwork2_debug.log | head -40
