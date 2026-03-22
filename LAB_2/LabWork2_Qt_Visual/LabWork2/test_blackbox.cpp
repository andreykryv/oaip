// ═══════════════════════════════════════════════════════════════════
//  test_blackbox.cpp — Чёрный ящик (Black-Box Testing)
//  Стратегия: тестируем поведение КАЖДОЙ ВКЛАДКИ/диалога через их
//  публичный интерфейс (что видит пользователь) — без знания о
//  внутренней реализации. Методы: классы эквивалентности,
//  граничные значения, таблицы решений.
//
//  Вкладки:
//    Вкладка 1 — Даты (DatesPage / DateController)
//    Вкладка 2 — Деканат (DeanPage / StudentController)
//    Вкладка 3 — Выражения (ExprPage / ExpressionController)
//    Вкладка 4 — Типы (TypeCheckPage / check_equals)
//  Диалог — AppErrorDialog (показывается при ошибке)
// ═══════════════════════════════════════════════════════════════════
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <array>
#include <sstream>

// Подключаем ТОЛЬКО публичные интерфейсы (чёрный ящик: видим только объявления)
#include "src/backend/Date.h"
#include "src/backend/Student.h"
#include "src/backend/Expression.h"

// ─── Мини-фреймворк ──────────────────────────────────────────────
static int g_passed = 0, g_failed = 0;
static std::string g_section;

void bbSection(const std::string& name) {
    g_section = name;
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║  " << name << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void bbCheck(const std::string& id, const std::string& desc, bool ok) {
    if (ok) {
        std::cout << "  [PASS] " << id << " — " << desc << "\n";
        ++g_passed;
    } else {
        std::cout << "  [FAIL] *** " << id << " — " << desc << " ***\n";
        ++g_failed;
    }
}

template<typename F>
bool tryCall(F fn) {
    try { fn(); return true; } catch (...) { return false; }
}

template<typename F>
bool throws(F fn) {
    try { fn(); return false; } catch (...) { return true; }
}

bool nearEq(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

// ═══════════════════════════════════════════════════════════════════
//  ВКЛАДКА 1: ДАТЫ
//  Чёрный ящик: пользователь вводит строку даты → получает результат.
//  Классы эквивалентности + граничные значения.
// ═══════════════════════════════════════════════════════════════════
void test_bb_dates_tab() {
    bbSection("ЧЯ-Вкладка 1: Даты (DatesPage)");

    // ─ Класс эквивалентности 1: Корректный ввод даты ─────────
    // Ожидание: дата создаётся, toString возвращает DD.MM.YYYY
    bbCheck("BB-D01", "Ввод '01.01.2023' → корректная дата",
            tryCall([]{ Date::fromString("01.01.2023"); }));
    bbCheck("BB-D02", "Ввод '31.12.9999' → корректная дата",
            tryCall([]{ Date::fromString("31.12.9999"); }));
    bbCheck("BB-D03", "Ввод '29.02.2000' (виc.год) → корректная дата",
            tryCall([]{ Date::fromString("29.02.2000"); }));
    bbCheck("BB-D04", "Формат вывода '05.03.2023' — ведущие нули",
            Date(5,3,2023).toString() == "05.03.2023");

    // ─ Класс эквивалентности 2: Некорректный ввод ─────────────
    // Ожидание: система сигнализирует об ошибке (бросает исключение)
    bbCheck("BB-D05", "Ввод '' (пустая строка) → ошибка",
            throws([]{ Date::fromString(""); }));
    bbCheck("BB-D06", "Ввод 'abc' (текст) → ошибка",
            throws([]{ Date::fromString("abc"); }));
    bbCheck("BB-D07", "Ввод '32.01.2023' (день>31) → ошибка",
            throws([]{ Date::fromString("32.01.2023"); }));
    bbCheck("BB-D08", "Ввод '00.01.2023' (день=0) → ошибка",
            throws([]{ Date::fromString("00.01.2023"); }));
    bbCheck("BB-D09", "Ввод '01.13.2023' (месяц=13) → ошибка",
            throws([]{ Date::fromString("01.13.2023"); }));
    bbCheck("BB-D10", "Ввод '29.02.2023' (не виc.год) → ошибка",
            throws([]{ Date::fromString("29.02.2023"); }));

    // ─ Класс эквивалентности 3: Граничные значения даты ──────
    bbCheck("BB-D11", "Минимальная дата 01.01.0001",
            tryCall([]{ Date(1,1,1); }));
    bbCheck("BB-D12", "Граница февраля — 28.02.2023 норма",
            tryCall([]{ Date(28,2,2023); }));
    bbCheck("BB-D13", "Граница февраля — 29.02.2023 ошибка",
            throws([]{ Date(29,2,2023); }));
    bbCheck("BB-D14", "Граница февраля — 29.02.2024 норма (2024 вис.)",
            tryCall([]{ Date(29,2,2024); }));

    // ─ Функция «Следующий день» ───────────────────────────────
    // Класс: переход через конец месяца
    bbCheck("BB-D15", "Следующий день после 31.01 = 01.02",
            []{ Date d = Date(31,1,2023).nextDay();
                return d.day()==1 && d.month()==2; }());
    // Класс: переход через конец года
    bbCheck("BB-D16", "Следующий день после 31.12.2023 = 01.01.2024",
            []{ Date d = Date(31,12,2023).nextDay();
                return d.day()==1 && d.month()==1 && d.year()==2024; }());
    // Класс: обычный день
    bbCheck("BB-D17", "Следующий день после 15.06 = 16.06",
            []{ Date d = Date(15,6,2023).nextDay();
                return d.day()==16 && d.month()==6; }());

    // ─ Функция «Предыдущий день» ─────────────────────────────
    bbCheck("BB-D18", "Предыдущий день 01.03.2023 = 28.02",
            []{ Date d = Date(1,3,2023).previousDay();
                return d.day()==28 && d.month()==2; }());
    bbCheck("BB-D19", "Предыдущий день 01.01.2023 = 31.12.2022",
            []{ Date d = Date(1,1,2023).previousDay();
                return d.day()==31 && d.month()==12 && d.year()==2022; }());
    bbCheck("BB-D20", "Предыдущий день 01.01.0001 → ошибка (нет даты ранее)",
            throws([]{ Date(1,1,1).previousDay(); }));

    // ─ Функция «Разница дней» ────────────────────────────────
    // Класс: будущая дата → положительное число
    bbCheck("BB-D21", "Разница: 01.01→10.01 = +9",
            Date(1,1,2023).duration(Date(10,1,2023)) == 9);
    // Класс: прошлая дата → отрицательное число
    bbCheck("BB-D22", "Разница: 10.01→01.01 = -9",
            Date(10,1,2023).duration(Date(1,1,2023)) == -9);
    // Класс: одинаковые даты → 0
    bbCheck("BB-D23", "Разница: одна дата = 0",
            Date(5,5,2023).duration(Date(5,5,2023)) == 0);

    // ─ Функция «Високосный год» ──────────────────────────────
    bbCheck("BB-D24", "isLeap — 2024 = true",  Date(1,1,2024).isLeap());
    bbCheck("BB-D25", "isLeap — 2023 = false", !Date(1,1,2023).isLeap());
    bbCheck("BB-D26", "isLeap — 2100 = false", !Date(1,1,2100).isLeap());
    bbCheck("BB-D27", "isLeap — 2000 = true",  Date(1,1,2000).isLeap());

    // ─ Функция «Номер недели» ────────────────────────────────
    {
        short w = Date(1,1,2024).weekNumber();
        bbCheck("BB-D28", "weekNumber — 01.01.2024 = неделя 1", w == 1);
    }
    {
        short w = Date(31,12,2024).weekNumber();
        bbCheck("BB-D29", "weekNumber — 31.12.2024 ≥ 52", w >= 52);
    }

    // ─ Функция «Дней до Дня рождения» ────────────────────────
    {
        // Дата сегодня < дата ДР в этом году → жди в этом году
        Date cur(1,1,2023), bday(31,1,1990);
        int days = cur.daysTillYourBirthday(bday);
        bbCheck("BB-D30", "ДР через 30 дней (01.01, ДР 31.01)", days == 30);
    }
    {
        // ДР уже прошёл → ждём следующего года (> 0)
        Date cur(1,6,2023), bday(1,1,1990);
        int days = cur.daysTillYourBirthday(bday);
        bbCheck("BB-D31", "ДР прошёл (1 янв) → дней > 0", days > 0);
    }
}

// ═══════════════════════════════════════════════════════════════════
//  ВКЛАДКА 2: ДЕКАНАТ
//  Чёрный ящик: пользователь вводит данные студента → наблюдает
//  статус, средний балл, фильтры.
// ═══════════════════════════════════════════════════════════════════

// Вспомогательный: создать студента с заданными оценками
Student makeStudent(const std::string& grp, const std::string& nm,
                    std::array<int,5> w, std::array<int,5> su) {
    return Student(grp, nm, w, su);
}

void test_bb_dean_tab() {
    bbSection("ЧЯ-Вкладка 2: Деканат (DeanPage)");

    // ─ Класс эквивалентности 1: Добавление студента ──────────
    // Ввод: корректные данные → студент добавлен
    bbCheck("BB-S01", "Добавление студента с корректными данными",
            tryCall([]{ makeStudent("ИВТ-21","Иванов",{5,4,3,5,4},{4,5,4,5,5}); }));

    // Ввод: пустая группа → система обрабатывает
    {
        Student s = makeStudent("","Имя",{3,3,3,3,3},{3,3,3,3,3});
        bbCheck("BB-S02", "Студент с пустой группой создаётся (контроллер валидирует)",
                s.group().empty());
    }

    // ─ Класс эквивалентности 2: Средний балл ─────────────────
    {
        // Все 5 → 5.0
        auto s = makeStudent("G","N",{5,5,5,5,5},{5,5,5,5,5});
        bbCheck("BB-S03", "Все оценки 5 → avgWinter = 5.0",
                nearEq(s.avgWinter(), 5.0));
        bbCheck("BB-S04", "Все оценки 5 → avgSummer = 5.0",
                nearEq(s.avgSummer(), 5.0));
    }
    {
        // Все 1 → 1.0
        auto s = makeStudent("G","N",{1,1,1,1,1},{1,1,1,1,1});
        bbCheck("BB-S05", "Все оценки 1 → avgWinter = 1.0",
                nearEq(s.avgWinter(), 1.0));
    }
    {
        // Смешанные: {1,2,3,4,5} → 3.0
        auto s = makeStudent("G","N",{1,2,3,4,5},{1,2,3,4,5});
        bbCheck("BB-S06", "Оценки {1..5} → avg = 3.0",
                nearEq(s.avgWinter(), 3.0) && nearEq(s.avgSummer(), 3.0));
    }

    // ─ Класс эквивалентности 3: Статус «OK» ──────────────────
    // Условие: нет долгов зимой И < 2 неудов летом
    {
        auto s = makeStudent("G","N",{3,4,5,4,3},{4,5,4,3,4});
        bbCheck("BB-S07", "Статус OK — нет долгов, нет отчисления",
                !s.hasWinterDebt() && !s.shouldExpel());
    }
    {
        auto s = makeStudent("G","N",{5,5,5,5,5},{5,5,5,5,5});
        bbCheck("BB-S08", "Отличник — статус OK",
                !s.hasWinterDebt() && !s.shouldExpel());
    }

    // ─ Класс эквивалентности 4: Статус «ДОЛГ» ────────────────
    // Условие: есть хотя бы 1 двойка зимой, но < 2 двоек летом
    {
        auto s = makeStudent("G","N",{2,3,4,5,3},{4,4,4,4,4});
        bbCheck("BB-S09", "1 двойка зимой → статус ДОЛГ",
                s.hasWinterDebt() && !s.shouldExpel());
    }
    {
        // 5 двоек зимой, летом ОК
        auto s = makeStudent("G","N",{2,2,2,2,2},{4,4,4,4,4});
        bbCheck("BB-S10", "5 двоек зимой → статус ДОЛГ (летом OK)",
                s.hasWinterDebt() && !s.shouldExpel());
    }
    {
        // Граница: зима минимально недосдана (1 двойка)
        auto s = makeStudent("G","N",{2,4,4,4,4},{4,4,4,4,4});
        bbCheck("BB-S11", "Граница долга: 1 двойка зимой",
                s.hasWinterDebt());
    }

    // ─ Класс эквивалентности 5: Статус «ОТЧИСЛИТЬ» ───────────
    // Условие: ≥ 2 двоек летом И хотя бы 1 двойка зимой
    {
        auto s = makeStudent("G","N",{2,3,4,4,4},{2,2,4,4,4});
        bbCheck("BB-S12", "2 двойки летом + 1 долг зимой → ОТЧИСЛИТЬ",
                s.shouldExpel());
    }
    {
        auto s = makeStudent("G","N",{2,3,4,4,4},{2,2,2,2,2});
        bbCheck("BB-S13", "5 двоек летом + долг зимой → ОТЧИСЛИТЬ",
                s.shouldExpel());
    }
    {
        // Граница: ровно 2 двойки летом + долг
        auto s = makeStudent("G","N",{2,4,4,4,4},{2,2,4,4,4});
        bbCheck("BB-S14", "Граница отчисления: ровно 2 летом + долг",
                s.shouldExpel());
    }
    {
        // НЕ отчислять: 2 двойки летом, но зима без долга
        auto s = makeStudent("G","N",{3,4,4,4,4},{2,2,4,4,4});
        bbCheck("BB-S15", "2 двойки летом, зима OK → НЕ отчислять",
                !s.shouldExpel());
    }
    {
        // НЕ отчислять: долг зимой, летом только 1 двойка
        auto s = makeStudent("G","N",{2,4,4,4,4},{2,4,4,4,4});
        bbCheck("BB-S16", "1 двойка летом + долг → НЕ отчислять",
                !s.shouldExpel());
    }

    // ─ Класс эквивалентности 6: Таблица решений «Отчислить?» ─
    // | зима долг | летних неудов | Отчислить? |
    // |     0     |       0       |    Нет     |
    // |     1     |       0       |    Нет     |
    // |     0     |       2       |    Нет     |
    // |     1     |       2       |    Да      |
    // |     1     |       5       |    Да      |
    {
        auto s1 = makeStudent("G","N",{4,4,4,4,4},{4,4,4,4,4});
        auto s2 = makeStudent("G","N",{2,4,4,4,4},{4,4,4,4,4});
        auto s3 = makeStudent("G","N",{4,4,4,4,4},{2,2,4,4,4});
        auto s4 = makeStudent("G","N",{2,4,4,4,4},{2,2,4,4,4});
        auto s5 = makeStudent("G","N",{2,4,4,4,4},{2,2,2,2,2});
        bbCheck("BB-S17", "Таблица решений: нет долгов → OK",     !s1.shouldExpel());
        bbCheck("BB-S18", "Таблица решений: долг зимой → ДОЛГ",   !s2.shouldExpel());
        bbCheck("BB-S19", "Таблица решений: 2 летом → не исключить", !s3.shouldExpel());
        bbCheck("BB-S20", "Таблица решений: 2 летом + зима → ИСКЛЮЧИТЬ", s4.shouldExpel());
        bbCheck("BB-S21", "Таблица решений: 5 летом + зима → ИСКЛЮЧИТЬ", s5.shouldExpel());
    }

    // ─ Класс эквивалентности 7: Сериализация/десериализация ───
    {
        auto s = makeStudent("ИВТ-21","Иванов",{5,4,3,2,1},{1,2,3,4,5});
        std::string line = s.serialize();
        Student restored = Student::deserialize(line);
        bbCheck("BB-S22", "Сохранение и загрузка — группа",
                s.group() == restored.group());
        bbCheck("BB-S23", "Сохранение и загрузка — имя",
                s.fullName() == restored.fullName());
        bbCheck("BB-S24", "Сохранение и загрузка — зима[2]",
                s.winterGrades()[2] == restored.winterGrades()[2]);
        bbCheck("BB-S25", "Сохранение и загрузка — лето[4]",
                s.summerGrades()[4] == restored.summerGrades()[4]);
    }

    // ─ Неполная строка при загрузке ──────────────────────────
    bbCheck("BB-S26", "Загрузка некорректной строки → ошибка",
            throws([]{ Student::deserialize("только;одно"); }));
    bbCheck("BB-S27", "Загрузка пустой строки → ошибка",
            throws([]{ Student::deserialize(""); }));

    // ─ Граничные оценки ──────────────────────────────────────
    {
        // Оценка 3 — не долг (порог строго < 3)
        auto s = makeStudent("G","N",{3,3,3,3,3},{3,3,3,3,3});
        bbCheck("BB-S28", "Оценка 3 не является долгом", !s.hasWinterDebt());
    }
    {
        // Оценка 2 — долг (2 < 3)
        auto s = makeStudent("G","N",{2,3,3,3,3},{3,3,3,3,3});
        bbCheck("BB-S29", "Оценка 2 является долгом", s.hasWinterDebt());
    }
    {
        // Оценка 1 — долг (1 < 3)
        auto s = makeStudent("G","N",{1,3,3,3,3},{3,3,3,3,3});
        bbCheck("BB-S30", "Оценка 1 является долгом", s.hasWinterDebt());
    }
    {
        // avgSummer дробный результат
        auto s = makeStudent("G","N",{4,4,4,4,4},{3,4,4,4,4});
        bbCheck("BB-S31", "avgSummer дробный: {3,4,4,4,4} = 3.8",
                nearEq(s.avgSummer(), 3.8));
    }
}

// ═══════════════════════════════════════════════════════════════════
//  ВКЛАДКА 3: ВЫРАЖЕНИЯ
//  Чёрный ящик: пользователь вводит строку выражения → получает
//  числовой результат или сообщение об ошибке.
// ═══════════════════════════════════════════════════════════════════

// Симулируем поведение ExpressionController::evaluate
// через прямое построение AST (как будто мы «нажимаем кнопку»)
double evalExpr(Expression* e) { return e->evaluate(); }

void test_bb_expr_tab() {
    bbSection("ЧЯ-Вкладка 3: Выражения (ExprPage)");

    // ─ Класс эквивалентности 1: Простые числа ────────────────
    {
        Number n(42.0);
        bbCheck("BB-E01", "Число 42 → вычисляется = 42",
                nearEq(n.evaluate(), 42.0));
    }
    {
        Number n(-5.0);
        bbCheck("BB-E02", "Отрицательное -5 → вычисляется = -5",
                nearEq(n.evaluate(), -5.0));
    }
    {
        Number n(0.0);
        bbCheck("BB-E03", "Ноль 0 → вычисляется = 0",
                nearEq(n.evaluate(), 0.0));
    }

    // ─ Класс эквивалентности 2: Сложение ─────────────────────
    {
        auto* e = new BinaryOperation(new Number(3), '+', new Number(4));
        bbCheck("BB-E04", "3 + 4 = 7", nearEq(e->evaluate(), 7.0));
        delete e;
    }
    {
        // Сложение с нулём
        auto* e = new BinaryOperation(new Number(5), '+', new Number(0));
        bbCheck("BB-E05", "5 + 0 = 5 (нейтральный элемент)", nearEq(e->evaluate(), 5.0));
        delete e;
    }
    {
        // Сложение двух отрицательных
        auto* e = new BinaryOperation(new Number(-3), '+', new Number(-4));
        bbCheck("BB-E06", "(-3) + (-4) = -7", nearEq(e->evaluate(), -7.0));
        delete e;
    }

    // ─ Класс эквивалентности 3: Вычитание ────────────────────
    {
        auto* e = new BinaryOperation(new Number(10), '-', new Number(3));
        bbCheck("BB-E07", "10 - 3 = 7", nearEq(e->evaluate(), 7.0));
        delete e;
    }
    {
        // Результат — ноль
        auto* e = new BinaryOperation(new Number(5), '-', new Number(5));
        bbCheck("BB-E08", "5 - 5 = 0", nearEq(e->evaluate(), 0.0));
        delete e;
    }
    {
        // Отрицательный результат
        auto* e = new BinaryOperation(new Number(3), '-', new Number(10));
        bbCheck("BB-E09", "3 - 10 = -7", nearEq(e->evaluate(), -7.0));
        delete e;
    }

    // ─ Класс эквивалентности 4: Умножение ────────────────────
    {
        auto* e = new BinaryOperation(new Number(6), '*', new Number(7));
        bbCheck("BB-E10", "6 * 7 = 42", nearEq(e->evaluate(), 42.0));
        delete e;
    }
    {
        // Умножение на 0
        auto* e = new BinaryOperation(new Number(999), '*', new Number(0));
        bbCheck("BB-E11", "999 * 0 = 0 (умножение на ноль)", nearEq(e->evaluate(), 0.0));
        delete e;
    }
    {
        // Умножение на 1
        auto* e = new BinaryOperation(new Number(55), '*', new Number(1));
        bbCheck("BB-E12", "55 * 1 = 55 (нейтральный элемент)", nearEq(e->evaluate(), 55.0));
        delete e;
    }
    {
        // Отрицательное * отрицательное = положительное
        auto* e = new BinaryOperation(new Number(-4), '*', new Number(-3));
        bbCheck("BB-E13", "(-4) * (-3) = 12", nearEq(e->evaluate(), 12.0));
        delete e;
    }

    // ─ Класс эквивалентности 5: Деление ──────────────────────
    {
        auto* e = new BinaryOperation(new Number(10), '/', new Number(2));
        bbCheck("BB-E14", "10 / 2 = 5", nearEq(e->evaluate(), 5.0));
        delete e;
    }
    {
        // Деление с дробным результатом
        auto* e = new BinaryOperation(new Number(1), '/', new Number(3));
        bbCheck("BB-E15", "1 / 3 ≈ 0.333...", nearEq(e->evaluate(), 1.0/3.0));
        delete e;
    }
    {
        // 0 / число = 0
        auto* e = new BinaryOperation(new Number(0), '/', new Number(7));
        bbCheck("BB-E16", "0 / 7 = 0", nearEq(e->evaluate(), 0.0));
        delete e;
    }

    // ─ Класс эквивалентности 6: Деление на ноль ─────────────
    // Ожидание: система сигнализирует об ошибке
    {
        auto* e = new BinaryOperation(new Number(5), '/', new Number(0));
        bbCheck("BB-E17", "5 / 0 → ошибка (деление на ноль)",
                throws([&]{ e->evaluate(); }));
        delete e;
    }

    // ─ Класс эквивалентности 7: Вложенные выражения ──────────
    {
        // (2 + 3) * 4 = 20
        auto* inner = new BinaryOperation(new Number(2), '+', new Number(3));
        auto* outer = new BinaryOperation(inner, '*', new Number(4));
        bbCheck("BB-E18", "(2+3)*4 = 20", nearEq(outer->evaluate(), 20.0));
        delete outer;
    }
    {
        // 10 / (2 + 3) = 2
        auto* inner = new BinaryOperation(new Number(2), '+', new Number(3));
        auto* outer = new BinaryOperation(new Number(10), '/', inner);
        bbCheck("BB-E19", "10/(2+3) = 2", nearEq(outer->evaluate(), 2.0));
        delete outer;
    }
    {
        // 3 + 4 * 5 = 23 (левый + правый с умножением внутри)
        auto* mul = new BinaryOperation(new Number(4), '*', new Number(5));
        auto* add = new BinaryOperation(new Number(3), '+', mul);
        bbCheck("BB-E20", "3+(4*5) = 23", nearEq(add->evaluate(), 23.0));
        delete add;
    }

    // ─ Класс эквивалентности 8: Дробные числа ────────────────
    {
        auto* e = new BinaryOperation(new Number(1.5), '+', new Number(2.5));
        bbCheck("BB-E21", "1.5 + 2.5 = 4.0", nearEq(e->evaluate(), 4.0));
        delete e;
    }
    {
        auto* e = new BinaryOperation(new Number(0.1), '*', new Number(0.2));
        bbCheck("BB-E22", "0.1 * 0.2 = 0.02", nearEq(e->evaluate(), 0.02, 1e-10));
        delete e;
    }

    // ─ Класс эквивалентности 9: Глубокое вложение ────────────
    {
        // ((1+2)*(3-1))/(1+1) = (3*2)/2 = 3
        auto* s1 = new BinaryOperation(new Number(1), '+', new Number(2));
        auto* s2 = new BinaryOperation(new Number(3), '-', new Number(1));
        auto* s3 = new BinaryOperation(new Number(1), '+', new Number(1));
        auto* mul = new BinaryOperation(s1, '*', s2);
        auto* div = new BinaryOperation(mul, '/', s3);
        bbCheck("BB-E23", "((1+2)*(3-1))/(1+1) = 3", nearEq(div->evaluate(), 3.0));
        delete div;
    }
}

// ═══════════════════════════════════════════════════════════════════
//  ВКЛАДКА 4: ПРОВЕРКА ТИПОВ (TypeCheckPage)
//  Чёрный ящик: пользователь выбирает два типа → получает
//  true/false для check_equals.
// ═══════════════════════════════════════════════════════════════════
void test_bb_typecheck_tab() {
    bbSection("ЧЯ-Вкладка 4: Проверка типов (TypeCheckPage)");

    // В UI: tag 0 = Number, tag 1 = BinaryOperation
    // check_equals возвращает true если type_tag совпадают

    // ─ Таблица решений (все 4 комбинации) ────────────────────
    {
        Number a(1), b(2);
        bbCheck("BB-T01", "Number vs Number → true  (0,0)",
                check_equals(&a, &b));
    }
    {
        auto* a = new BinaryOperation(new Number(1),'+',new Number(2));
        auto* b = new BinaryOperation(new Number(3),'*',new Number(4));
        bbCheck("BB-T02", "BinOp vs BinOp → true  (1,1)",
                check_equals(a, b));
        delete a; delete b;
    }
    {
        Number a(1);
        auto* b = new BinaryOperation(new Number(1),'+',new Number(2));
        bbCheck("BB-T03", "Number vs BinOp → false (0,1)",
                !check_equals(&a, b));
        delete b;
    }
    {
        auto* a = new BinaryOperation(new Number(1),'+',new Number(2));
        Number b(1);
        bbCheck("BB-T04", "BinOp vs Number → false (1,0)",
                !check_equals(a, &b));
        delete a;
    }

    // ─ Симметричность ─────────────────────────────────────────
    {
        Number a(1);
        auto* b = new BinaryOperation(new Number(1),'+',new Number(2));
        bool ab = check_equals(&a, b);
        bool ba = check_equals(b, &a);
        bbCheck("BB-T05", "check_equals симметрично: (N,B)==(B,N)", ab == ba);
        delete b;
    }

    // ─ Один и тот же объект ───────────────────────────────────
    {
        Number n(5);
        bbCheck("BB-T06", "check_equals(x, x) — всегда true", check_equals(&n, &n));
    }
    {
        auto* op = new BinaryOperation(new Number(1),'+',new Number(2));
        bbCheck("BB-T07", "check_equals(op, op) — BinOp с собой → true",
                check_equals(op, op));
        delete op;
    }

    // ─ Независимость от значений (только тип) ────────────────
    {
        Number a(100), b(-999);
        bbCheck("BB-T08", "Number(100) vs Number(-999) → true (значения не важны)",
                check_equals(&a, &b));
    }
    {
        auto* a = new BinaryOperation(new Number(1),'+',new Number(2));
        auto* b = new BinaryOperation(new Number(100),'*',new Number(-5));
        bbCheck("BB-T09", "BinOp(1+2) vs BinOp(100*-5) → true (операция не важна)",
                check_equals(a, b));
        delete a; delete b;
    }

    // ─ type_tag корректен ─────────────────────────────────────
    {
        Number n(1.0);
        bbCheck("BB-T10", "Number::type_tag = 1", n.type_tag() == 1);
    }
    {
        auto* op = new BinaryOperation(new Number(1),'+',new Number(2));
        bbCheck("BB-T11", "BinOp::type_tag = 2", op->type_tag() == 2);
        delete op;
    }
}

// ═══════════════════════════════════════════════════════════════════
//  ДИАЛОГ ОШИБОК (AppErrorDialog)
//  Чёрный ящик: проверяем сценарии, когда диалог должен/не должен
//  показываться. Симулируем через исключения и валидацию.
// ═══════════════════════════════════════════════════════════════════
void test_bb_error_dialog() {
    bbSection("ЧЯ-Диалог ошибок (AppErrorDialog)");

    // ─ Сценарий 1: Ввод корректной даты → нет ошибки ─────────
    bbCheck("BB-ERR01", "Корректная дата → нет исключения (диалог не нужен)",
            tryCall([]{ Date::fromString("15.06.2023"); }));
    bbCheck("BB-ERR02", "Корректный студент → нет исключения",
            tryCall([]{ makeStudent("ИВТ-21","Имя",{3,3,3,3,3},{3,3,3,3,3}); }));
    bbCheck("BB-ERR03", "Корректное выражение → нет исключения",
            tryCall([]{ Number n(5.0); n.evaluate(); }));

    // ─ Сценарий 2: Некорректный ввод → исключение (диалог) ───
    bbCheck("BB-ERR04", "Дата с неверным форматом → исключение (диалог ошибки)",
            throws([]{ Date::fromString("15-06-2023"); }));
    bbCheck("BB-ERR05", "Несуществующая дата → исключение (диалог ошибки)",
            throws([]{ Date::fromString("31.04.2023"); }));
    bbCheck("BB-ERR06", "Год = 0 → исключение (диалог ошибки)",
            throws([]{ Date(1, 1, 0); }));
    bbCheck("BB-ERR07", "Деление на 0 → исключение (диалог ошибки)",
            throws([]{ BinaryOperation e(new Number(1),'/',new Number(0));
                       e.evaluate(); }));
    bbCheck("BB-ERR08", "Неполная строка студента → исключение",
            throws([]{ Student::deserialize("только_одно_поле"); }));

    // ─ Сценарий 3: Граничные случаи ─────────────────────────
    // 01.01.0001 — минимальная корректная, НЕ должна вызвать диалог
    bbCheck("BB-ERR09", "Минимальная дата 01.01.0001 → норма (нет диалога)",
            tryCall([]{ Date(1,1,1); }));
    // 01.01.0000 — год 0, должна вызвать диалог
    bbCheck("BB-ERR10", "Год 0 → ошибка (диалог нужен)",
            throws([]{ Date(1,1,0); }));
    // 29.02 в невисокосном году → диалог
    bbCheck("BB-ERR11", "29.02.2021 → ошибка (не вис.год, диалог нужен)",
            throws([]{ Date(29,2,2021); }));
    // 29.02 в 2000 → норма, нет диалога
    bbCheck("BB-ERR12", "29.02.2000 → норма (2000 вис., диалог не нужен)",
            tryCall([]{ Date(29,2,2000); }));

    // ─ Сценарий 4: Предыдущий день у минимальной даты ────────
    // Пользователь нажимает «Предыдущий день» для 01.01.0001 → диалог
    bbCheck("BB-ERR13", "prevDay(01.01.0001) → ошибка (диалог нужен)",
            throws([]{ Date(1,1,1).previousDay(); }));

    // ─ Сценарий 5: Десериализация студентов ──────────────────
    bbCheck("BB-ERR14", "Пустая строка студента → ошибка",
            throws([]{ Student::deserialize(""); }));
    bbCheck("BB-ERR15", "Строка с 3 полями → ошибка (нужно 4)",
            throws([]{ Student::deserialize("A;B;1 2 3 4 5"); }));
    bbCheck("BB-ERR16", "Строка с 4 полями → норма",
            tryCall([]{ Student::deserialize("G;N;3 3 3 3 3;3 3 3 3 3"); }));
}

// ═══════════════════════════════════════════════════════════════════
//  ИТОГ
// ═══════════════════════════════════════════════════════════════════
void printBBSummary() {
    int total = g_passed + g_failed;
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║           ИТОГИ ЧЁРНОГО ЯЩИКА                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  Всего тестов:  " << total   << "\n";
    std::cout << "║  Пройдено:      " << g_passed << "\n";
    std::cout << "║  Провалено:     " << g_failed << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    if (g_failed == 0)
        std::cout << "\n  ✅  ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n\n";
    else
        std::cout << "\n  ❌  ЕСТЬ ОШИБКИ: " << g_failed << " тест(ов) провалено!\n\n";
}

int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║  ТЕСТИРОВАНИЕ — ЧЁРНЫЙ ЯЩИК (Black-Box Testing)     ║\n";
    std::cout << "║  Вкладка 1: Даты                                    ║\n";
    std::cout << "║  Вкладка 2: Деканат                                 ║\n";
    std::cout << "║  Вкладка 3: Выражения                               ║\n";
    std::cout << "║  Вкладка 4: Типы                                    ║\n";
    std::cout << "║  Диалог ошибок                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";

    test_bb_dates_tab();
    test_bb_dean_tab();
    test_bb_expr_tab();
    test_bb_typecheck_tab();
    test_bb_error_dialog();
    printBBSummary();

    return g_failed == 0 ? 0 : 1;
}
