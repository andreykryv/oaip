// ═══════════════════════════════════════════════════════════════════
//  test_whitebox.cpp — Белый ящик (White-Box Testing)
//  Стратегия: проверяем внутренние пути, ветки и граничные условия.
//  Запуск: g++ -std=c++17 -o test_wb test_whitebox.cpp src/*.cpp && ./test_wb
// ═══════════════════════════════════════════════════════════════════
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <algorithm>

#include "src/backend/Date.h"
#include "src/backend/Student.h"
#include "src/backend/Expression.h"

// ─── Мини-фреймворк для тестирования ────────────────────────────
static int g_passed = 0, g_failed = 0;
static std::string g_section = "";

void section(const std::string& name) {
    g_section = name;
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║  " << name << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void check(const std::string& name, bool condition) {
    if (condition) {
        std::cout << "  [PASS] " << name << "\n";
        ++g_passed;
    } else {
        std::cout << "  [FAIL] *** " << name << " ***\n";
        ++g_failed;
    }
}

template<typename F>
void checkThrows(const std::string& name, F fn) {
    bool threw = false;
    try { fn(); } catch (...) { threw = true; }
    check(name, threw);
}

template<typename F>
void checkNoThrow(const std::string& name, F fn) {
    bool threw = false;
    try { fn(); } catch (...) { threw = true; }
    check(name, !threw);
}

bool nearEq(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

// ═══════════════════════════════════════════════════════════════════
//  БЛОК 1: КЛАСС Date — БЕЛЫЙ ЯЩИК
//  Покрываем все ветки: isLeap (4 правила), daysInMonth (все 12 месяцев),
//  nextDay/previousDay (смена месяца, года), граничные даты и т.д.
// ═══════════════════════════════════════════════════════════════════

void test_date_whitebox() {
    section("Date — Белый ящик (60 тестов)");

    // ── Конструктор и геттеры ─────────────────────────────────
    {
        Date d(15, 6, 2000);
        check("WB-D01: Конструктор — day()",   d.day()   == 15);
        check("WB-D02: Конструктор — month()", d.month() == 6);
        check("WB-D03: Конструктор — year()",  d.year()  == 2000);
    }

    // ── Дефолтный конструктор ─────────────────────────────────
    {
        Date d;
        check("WB-D04: Дефолтный конструктор — 01.01.0001",
              d.day()==1 && d.month()==1 && d.year()==1);
    }

    // ── Невалидные даты: исключения ───────────────────────────
    checkThrows("WB-D05: день=0 → исключение",   []{ Date(0,  1, 2000); });
    checkThrows("WB-D06: день=32 → исключение",  []{ Date(32, 1, 2000); });
    checkThrows("WB-D07: месяц=0 → исключение",  []{ Date(1,  0, 2000); });
    checkThrows("WB-D08: месяц=13 → исключение", []{ Date(1, 13, 2000); });
    checkThrows("WB-D09: год=0 → исключение",    []{ Date(1,  1,    0); });
    checkThrows("WB-D10: 30 фев → исключение",   []{ Date(30, 2, 2000); });

    // ── isLeap: все 4 ветки алгоритма ─────────────────────────
    // Ветка 1: % 4 != 0 → не високосный
    check("WB-D11: isLeap — 2001 не високосный (% 4 != 0)",
          !Date(1,1,2001).isLeap());
    // Ветка 2: % 4 == 0, % 100 != 0 → високосный
    check("WB-D12: isLeap — 2004 високосный (% 4 == 0, % 100 != 0)",
          Date(1,1,2004).isLeap());
    // Ветка 3: % 100 == 0, % 400 != 0 → НЕ високосный
    check("WB-D13: isLeap — 1900 не високосный (% 100 == 0, % 400 != 0)",
          !Date(1,1,1900).isLeap());
    // Ветка 4: % 400 == 0 → високосный
    check("WB-D14: isLeap — 2000 високосный (% 400 == 0)",
          Date(1,1,2000).isLeap());
    check("WB-D15: isLeap — 1600 високосный (% 400 == 0)",
          Date(1,1,1600).isLeap());

    // ── daysInMonth: все 12 месяцев ───────────────────────────
    checkNoThrow("WB-D16: 31 янв 2023",  []{ Date(31, 1, 2023);  });
    checkThrows ("WB-D17: 32 янв 2023",  []{ Date(32, 1, 2023);  });
    checkNoThrow("WB-D18: 28 фев 2023",  []{ Date(28, 2, 2023);  });
    checkThrows ("WB-D19: 29 фев 2023",  []{ Date(29, 2, 2023);  });
    checkNoThrow("WB-D20: 29 фев 2000",  []{ Date(29, 2, 2000);  });
    checkNoThrow("WB-D21: 31 мар 2023",  []{ Date(31, 3, 2023);  });
    checkThrows ("WB-D22: 31 апр 2023",  []{ Date(31, 4, 2023);  });
    checkNoThrow("WB-D23: 31 май 2023",  []{ Date(31, 5, 2023);  });
    checkThrows ("WB-D24: 31 июн 2023",  []{ Date(31, 6, 2023);  });
    checkNoThrow("WB-D25: 31 июл 2023",  []{ Date(31, 7, 2023);  });
    checkNoThrow("WB-D26: 31 авг 2023",  []{ Date(31, 8, 2023);  });
    checkThrows ("WB-D27: 31 сен 2023",  []{ Date(31, 9, 2023);  });
    checkNoThrow("WB-D28: 31 окт 2023",  []{ Date(31,10, 2023);  });
    checkThrows ("WB-D29: 31 ноя 2023",  []{ Date(31,11, 2023);  });
    checkNoThrow("WB-D30: 31 дек 2023",  []{ Date(31,12, 2023);  });

    // ── nextDay: все ветки ────────────────────────────────────
    {
        // Обычный день внутри месяца
        Date d = Date(14, 6, 2023).nextDay();
        check("WB-D31: nextDay — обычный день",
              d.day()==15 && d.month()==6 && d.year()==2023);
    }
    {
        // Конец месяца → переход к следующему
        Date d = Date(31, 1, 2023).nextDay();
        check("WB-D32: nextDay — конец января → 01.02",
              d.day()==1 && d.month()==2 && d.year()==2023);
    }
    {
        // Конец февраля (не високосный) → 01.03
        Date d = Date(28, 2, 2023).nextDay();
        check("WB-D33: nextDay — 28 фев невисок → 01.03",
              d.day()==1 && d.month()==3 && d.year()==2023);
    }
    {
        // Конец февраля (високосный) → 29.02
        Date d = Date(28, 2, 2000).nextDay();
        check("WB-D34: nextDay — 28 фев 2000 → 29.02",
              d.day()==29 && d.month()==2 && d.year()==2000);
    }
    {
        // 29 февраля → 01 марта
        Date d = Date(29, 2, 2000).nextDay();
        check("WB-D35: nextDay — 29 фев 2000 → 01.03",
              d.day()==1 && d.month()==3 && d.year()==2000);
    }
    {
        // Конец года → 01.01 следующего года
        Date d = Date(31, 12, 2023).nextDay();
        check("WB-D36: nextDay — 31.12 → 01.01 следующего года",
              d.day()==1 && d.month()==1 && d.year()==2024);
    }

    // ── previousDay: все ветки ────────────────────────────────
    {
        // Обычный день
        Date d = Date(15, 6, 2023).previousDay();
        check("WB-D37: prevDay — обычный день",
              d.day()==14 && d.month()==6 && d.year()==2023);
    }
    {
        // Начало месяца → конец предыдущего
        Date d = Date(1, 3, 2023).previousDay();
        check("WB-D38: prevDay — 01.03 → 28.02 (не височный)",
              d.day()==28 && d.month()==2 && d.year()==2023);
    }
    {
        // 01.03 в високосный год → 29.02
        Date d = Date(1, 3, 2000).previousDay();
        check("WB-D39: prevDay — 01.03.2000 → 29.02.2000",
              d.day()==29 && d.month()==2 && d.year()==2000);
    }
    {
        // 01.01 → 31.12 предыдущего года
        Date d = Date(1, 1, 2023).previousDay();
        check("WB-D40: prevDay — 01.01 → 31.12 предыдущего года",
              d.day()==31 && d.month()==12 && d.year()==2022);
    }
    // Граница: 01.01.0001 → исключение
    checkThrows("WB-D41: prevDay — 01.01.0001 → исключение",
                []{ Date(1,1,1).previousDay(); });

    // ── duration ──────────────────────────────────────────────
    {
        Date a(1, 1, 2023), b(1, 1, 2024);
        check("WB-D42: duration — 365 дней (не високосный 2023)",
              a.duration(b) == 365);
    }
    {
        Date a(1, 1, 2000), b(1, 1, 2001);
        check("WB-D43: duration — 366 дней (2000 — високосный)",
              a.duration(b) == 366);
    }
    {
        Date a(1, 1, 2023), b(1, 1, 2023);
        check("WB-D44: duration — 0 (одна дата)",
              a.duration(b) == 0);
    }
    {
        // Отрицательное расстояние: b < a
        Date a(10, 1, 2023), b(1, 1, 2023);
        check("WB-D45: duration — отрицательное (b < a)",
              a.duration(b) == -9);
    }
    {
        Date a(1, 1, 2023), b(31, 1, 2023);
        check("WB-D46: duration — 30 дней",
              a.duration(b) == 30);
    }

    // ── toString ──────────────────────────────────────────────
    {
        check("WB-D47: toString — формат DD.MM.YYYY с нулями",
              Date(5, 3, 2023).toString() == "05.03.2023");
        check("WB-D48: toString — 01.01.0001",
              Date(1,1,1).toString() == "01.01.0001");
        check("WB-D49: toString — 31.12.9999",
              Date(31,12,9999).toString() == "31.12.9999");
    }

    // ── fromString: парсинг и ошибки ──────────────────────────
    {
        Date d = Date::fromString("15.06.2023");
        check("WB-D50: fromString — корректная строка",
              d.day()==15 && d.month()==6 && d.year()==2023);
    }
    checkThrows("WB-D51: fromString — неверный разделитель (слэш)",
                []{ Date::fromString("15/06/2023"); });
    checkThrows("WB-D52: fromString — слишком короткая строка",
                []{ Date::fromString("1.1.20"); });
    checkThrows("WB-D53: fromString — невалидная дата в строке",
                []{ Date::fromString("32.01.2023"); });

    // ── operators ─────────────────────────────────────────────
    {
        Date a(1,1,2023), b(2,1,2023), c(1,1,2023);
        check("WB-D54: operator== — равные даты",    a == c);
        check("WB-D55: operator== — неравные даты", !(a == b));
        check("WB-D56: operator<  — a < b",          a < b);
        check("WB-D57: operator<  — b не < a",       !(b < a));
        check("WB-D58: operator<= — a <= c (равны)", a <= c);
        check("WB-D59: operator<= — a <= b",         a <= b);
    }

    // ── weekNumber ────────────────────────────────────────────
    {
        // 01.01.2024 — понедельник, 1-я неделя
        check("WB-D60: weekNumber — 01.01.2024 = неделя 1",
              Date(1,1,2024).weekNumber() == 1);
    }

    // ── daysTillBirthday ──────────────────────────────────────
    {
        // Текущая дата 01.01.2023, день рождения 10.01 → 9 дней
        Date cur(1, 1, 2023);
        Date bday(10, 1, 1990);
        int days = cur.daysTillYourBirthday(bday);
        check("WB-D61: daysTillBirthday — ДР через 9 дней", days == 9);
    }
    {
        // ДР уже был в этом году → берём следующий год
        Date cur(15, 6, 2023);
        Date bday(1, 3, 1990);  // 1 марта — уже прошло
        int days = cur.daysTillYourBirthday(bday);
        check("WB-D62: daysTillBirthday — ДР в прошлом → ждём следующий год",
              days > 0);
    }
}

// ═══════════════════════════════════════════════════════════════════
//  БЛОК 2: КЛАСС Student — БЕЛЫЙ ЯЩИК
//  Покрываем все ветки: логика долгов, отчисления, сериализация,
//  граничные значения оценок.
// ═══════════════════════════════════════════════════════════════════

void test_student_whitebox() {
    section("Student — Белый ящик (55 тестов)");

    using A = std::array<int,5>;

    // ── Конструкторы ──────────────────────────────────────────
    {
        Student s;
        check("WB-S01: Дефолтный конструктор — поля пустые",
              s.group().empty() && s.fullName().empty());
        check("WB-S02: Дефолтный конструктор — оценки зимы = 3",
              s.winterGrades()[0] == 3);
        check("WB-S03: Дефолтный конструктор — оценки лета = 3",
              s.summerGrades()[4] == 3);
    }
    {
        A w{5,4,3,5,4}, su{4,5,5,4,5};
        Student s("ИВТ-21", "Иванов А.А.", w, su);
        check("WB-S04: Параметрический конструктор — group()",
              s.group() == "ИВТ-21");
        check("WB-S05: Параметрический конструктор — fullName()",
              s.fullName() == "Иванов А.А.");
        check("WB-S06: Параметрический конструктор — winterGrades()[0]=5",
              s.winterGrades()[0] == 5);
        check("WB-S07: Параметрический конструктор — summerGrades()[4]=5",
              s.summerGrades()[4] == 5);
    }

    // ── Сеттеры ───────────────────────────────────────────────
    {
        Student s;
        s.setGroup("ИВТ-99");
        s.setFullName("Тестов Т.Т.");
        check("WB-S08: setGroup()", s.group() == "ИВТ-99");
        check("WB-S09: setFullName()", s.fullName() == "Тестов Т.Т.");

        A newW{1,2,3,4,5};
        s.setWinterGrades(newW);
        check("WB-S10: setWinterGrades() — первая оценка",
              s.winterGrades()[0] == 1);

        A newS{5,4,3,2,1};
        s.setSummerGrades(newS);
        check("WB-S11: setSummerGrades() — последняя оценка",
              s.summerGrades()[4] == 1);
    }

    // ── avgWinter ─────────────────────────────────────────────
    {
        A w{5,5,5,5,5};
        Student s("G","N", w, w);
        check("WB-S12: avgWinter — все 5 → 5.0", nearEq(s.avgWinter(), 5.0));
    }
    {
        A w{1,2,3,4,5};
        A su{1,1,1,1,1};
        Student s("G","N", w, su);
        check("WB-S13: avgWinter — {1,2,3,4,5} → 3.0", nearEq(s.avgWinter(), 3.0));
    }
    {
        A w{2,2,2,2,2};
        Student s("G","N", w, w);
        check("WB-S14: avgWinter — все 2 → 2.0", nearEq(s.avgWinter(), 2.0));
    }
    {
        A w{3,3,3,3,3};
        Student s("G","N", w, w);
        check("WB-S15: avgWinter — все 3 → 3.0", nearEq(s.avgWinter(), 3.0));
    }

    // ── avgSummer ─────────────────────────────────────────────
    {
        A w{3,3,3,3,3}, su{4,4,4,4,4};
        Student s("G","N", w, su);
        check("WB-S16: avgSummer — все 4 → 4.0", nearEq(s.avgSummer(), 4.0));
    }
    {
        A w{5,5,5,5,5}, su{2,3,4,5,1};
        Student s("G","N", w, su);
        check("WB-S17: avgSummer — {2,3,4,5,1} → 3.0", nearEq(s.avgSummer(), 3.0));
    }

    // ── hasWinterDebt ─────────────────────────────────────────
    {
        // Ветка: нет двоек → false
        A w{3,3,3,3,3};
        Student s("G","N", w, w);
        check("WB-S18: hasWinterDebt — нет двоек → false", !s.hasWinterDebt());
    }
    {
        // Ветка: одна двойка → true
        A w{2,3,3,3,3}, su{3,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S19: hasWinterDebt — одна 2 → true", s.hasWinterDebt());
    }
    {
        // Ветка: все двойки → true
        A w{2,2,2,2,2}, su{3,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S20: hasWinterDebt — все 2 → true", s.hasWinterDebt());
    }
    {
        // Граница: 3 — не долг (порог < 3)
        A w{3,3,3,3,3}, su{3,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S21: hasWinterDebt — минимум 3 → не долг", !s.hasWinterDebt());
    }
    {
        // Граница: 1 — долг
        A w{1,3,3,3,3}, su{3,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S22: hasWinterDebt — оценка 1 → долг", s.hasWinterDebt());
    }

    // ── shouldExpel ───────────────────────────────────────────
    // Условие: >= 2 неудов летом И долг зимой
    {
        // Оба условия выполнены → отчислить
        A w{2,3,3,3,3}, su{2,2,3,3,3};
        Student s("G","N", w, su);
        check("WB-S23: shouldExpel — 2 неудов летом + долг зимой → true",
              s.shouldExpel());
    }
    {
        // Только долг зимой, летом норм → не отчислять
        A w{2,3,3,3,3}, su{3,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S24: shouldExpel — долг зимой, летом ОК → false",
              !s.shouldExpel());
    }
    {
        // Два неудова летом, но зимой чисто → не отчислять
        A w{3,3,3,3,3}, su{2,2,3,3,3};
        Student s("G","N", w, su);
        check("WB-S25: shouldExpel — 2 неудов летом, зимой ОК → false",
              !s.shouldExpel());
    }
    {
        // Один неудов летом + долг зимой → ещё не отчислять (нужно >= 2)
        A w{2,3,3,3,3}, su{2,3,3,3,3};
        Student s("G","N", w, su);
        check("WB-S26: shouldExpel — 1 неудов летом + долг зимой → false",
              !s.shouldExpel());
    }
    {
        // Ровно 2 неудова летом + долг → отчислять (граница)
        A w{2,3,3,3,3}, su{2,2,4,4,4};
        Student s("G","N", w, su);
        check("WB-S27: shouldExpel — ровно 2 летом + долг → true (граница)",
              s.shouldExpel());
    }
    {
        // 5 неудов летом + долг → отчислять
        A w{2,3,3,3,3}, su{1,1,1,1,1};
        Student s("G","N", w, su);
        check("WB-S28: shouldExpel — 5 неудов летом + долг → true",
              s.shouldExpel());
    }
    {
        // Все хорошие оценки → не отчислять
        A w{5,5,5,5,5}, su{5,5,5,5,5};
        Student s("G","N", w, su);
        check("WB-S29: shouldExpel — отличник → false", !s.shouldExpel());
    }

    // ── serialize ─────────────────────────────────────────────
    {
        A w{5,4,3,2,1}, su{1,2,3,4,5};
        Student s("ИВТ-21", "Иванов", w, su);
        std::string line = s.serialize();
        check("WB-S30: serialize — содержит группу",
              line.find("ИВТ-21") != std::string::npos);
        check("WB-S31: serialize — содержит имя",
              line.find("Иванов") != std::string::npos);
        // Формат: group;name;w1 w2...;s1 s2...
        check("WB-S32: serialize — 3 разделителя ';'",
              std::count(line.begin(), line.end(), ';') == 3);
    }

    // ── deserialize ───────────────────────────────────────────
    {
        std::string line = "ИВТ-21;Иванов А.А.;5 4 3 2 1;1 2 3 4 5";
        Student s = Student::deserialize(line);
        check("WB-S33: deserialize — group",    s.group()    == "ИВТ-21");
        check("WB-S34: deserialize — fullName", s.fullName() == "Иванов А.А.");
        check("WB-S35: deserialize — зима[0]=5", s.winterGrades()[0] == 5);
        check("WB-S36: deserialize — зима[4]=1", s.winterGrades()[4] == 1);
        check("WB-S37: deserialize — лето[0]=1", s.summerGrades()[0] == 1);
        check("WB-S38: deserialize — лето[4]=5", s.summerGrades()[4] == 5);
    }
    {
        // Слишком мало полей → исключение
        checkThrows("WB-S39: deserialize — неполная строка → исключение",
                    []{ Student::deserialize("ИВТ;Имя"); });
    }

    // ── serialize/deserialize: round-trip ─────────────────────
    {
        A w{5,4,3,2,1}, su{1,2,3,4,5};
        Student orig("ИВТ-22", "Петров В.В.", w, su);
        std::string line = orig.serialize();
        Student copy = Student::deserialize(line);
        check("WB-S40: round-trip — group",
              orig.group() == copy.group());
        check("WB-S41: round-trip — fullName",
              orig.fullName() == copy.fullName());
        check("WB-S42: round-trip — winter[0]",
              orig.winterGrades()[0] == copy.winterGrades()[0]);
        check("WB-S43: round-trip — summer[4]",
              orig.summerGrades()[4] == copy.summerGrades()[4]);
    }

    // ── Граничные значения оценок ─────────────────────────────
    {
        // Ровно 3 — не провал (по логике < 3)
        A w{3,3,3,3,3}, su{3,3,3,3,3};
        Student s("G","N",w,su);
        check("WB-S44: оценка 3 — не долг, не отчисление",
              !s.hasWinterDebt() && !s.shouldExpel());
    }
    {
        // Оценки 2 по обеим сессиям
        A both{2,2,2,2,2};
        Student s("G","N",both,both);
        check("WB-S45: avgWinter — все 2 = 2.0", nearEq(s.avgWinter(), 2.0));
        check("WB-S46: avgSummer — все 2 = 2.0", nearEq(s.avgSummer(), 2.0));
        check("WB-S47: hasWinterDebt — все 2 → true", s.hasWinterDebt());
        check("WB-S48: shouldExpel — 5 двоек летом + долг → true", s.shouldExpel());
    }
    {
        // Максимальные оценки (10 балльная система)
        A w{10,10,10,10,10}, su{10,10,10,10,10};
        Student s("G","N",w,su);
        check("WB-S49: avgWinter — все 10 = 10.0", nearEq(s.avgWinter(), 10.0));
        check("WB-S50: avgSummer — все 10 = 10.0", nearEq(s.avgSummer(), 10.0));
        check("WB-S51: shouldExpel — все 10 → false", !s.shouldExpel());
    }

    // ── countFails косвенно через hasWinterDebt/shouldExpel ───
    {
        // Ровно 4 неудова зимой
        A w{2,2,2,2,3}, su{3,3,3,3,3};
        Student s("G","N",w,su);
        check("WB-S52: 4 двойки зимой → долг", s.hasWinterDebt());
    }
    {
        // Ровно 3 неудова летом + долг зимой → отчислить
        A w{2,3,3,3,3}, su{2,2,2,3,3};
        Student s("G","N",w,su);
        check("WB-S53: 3 неудова летом + долг → отчислить", s.shouldExpel());
    }
    {
        // avgWinter дробный: {3,4,3,4,3} = 3.4
        A w{3,4,3,4,3}, su{3,3,3,3,3};
        Student s("G","N",w,su);
        check("WB-S54: avgWinter — {3,4,3,4,3} = 3.4", nearEq(s.avgWinter(), 3.4));
    }
    {
        // Пустые поля после дефолтного конструктора
        Student s;
        check("WB-S55: дефолтный — avgWinter = 3.0 (все 3)", nearEq(s.avgWinter(), 3.0));
    }
}

// ═══════════════════════════════════════════════════════════════════
//  БЛОК 3: КЛАССЫ Expression — БЕЛЫЙ ЯЩИК
//  Покрываем все ветки evaluate(), все операторы, type_tag,
//  check_equals, nodeLabel, дерево потомков, вложенные выражения.
// ═══════════════════════════════════════════════════════════════════

void test_expression_whitebox() {
    section("Expression — Белый ящик (55 тестов)");

    // ── Number::evaluate ──────────────────────────────────────
    {
        Number n(42.0);
        check("WB-E01: Number::evaluate — 42", nearEq(n.evaluate(), 42.0));
    }
    {
        Number n(0.0);
        check("WB-E02: Number::evaluate — 0", nearEq(n.evaluate(), 0.0));
    }
    {
        Number n(-5.5);
        check("WB-E03: Number::evaluate — отрицательное -5.5",
              nearEq(n.evaluate(), -5.5));
    }
    {
        Number n(1e10);
        check("WB-E04: Number::evaluate — большое число 1e10",
              nearEq(n.evaluate(), 1e10));
    }

    // ── Number::type_tag ─────────────────────────────────────
    {
        Number n(1.0);
        check("WB-E05: Number::type_tag == 1", n.type_tag() == 1);
    }

    // ── Number::nodeLabel ────────────────────────────────────
    {
        Number n(3.0);
        check("WB-E06: Number::nodeLabel содержит '3'",
              n.nodeLabel().find('3') != std::string::npos);
    }
    {
        Number n(-7.5);
        check("WB-E07: Number::nodeLabel содержит '-'",
              n.nodeLabel().find('-') != std::string::npos);
    }

    // ── Number: нет потомков ──────────────────────────────────
    {
        Number n(1.0);
        check("WB-E08: Number::leftChild == nullptr",
              n.leftChild() == nullptr);
        check("WB-E09: Number::rightChild == nullptr",
              n.rightChild() == nullptr);
    }

    // ── BinaryOperation::evaluate: все 4 оператора ───────────
    {
        // '+' : 3 + 4 = 7
        auto* expr = new BinaryOperation(new Number(3), '+', new Number(4));
        check("WB-E10: BinOp '+' 3+4=7", nearEq(expr->evaluate(), 7.0));
        delete expr;
    }
    {
        // '-' : 10 - 4 = 6
        auto* expr = new BinaryOperation(new Number(10), '-', new Number(4));
        check("WB-E11: BinOp '-' 10-4=6", nearEq(expr->evaluate(), 6.0));
        delete expr;
    }
    {
        // '*' : 3 * 5 = 15
        auto* expr = new BinaryOperation(new Number(3), '*', new Number(5));
        check("WB-E12: BinOp '*' 3*5=15", nearEq(expr->evaluate(), 15.0));
        delete expr;
    }
    {
        // '/' : 10 / 4 = 2.5
        auto* expr = new BinaryOperation(new Number(10), '/', new Number(4));
        check("WB-E13: BinOp '/' 10/4=2.5", nearEq(expr->evaluate(), 2.5));
        delete expr;
    }

    // ── BinaryOperation: деление на ноль ─────────────────────
    {
        auto* expr = new BinaryOperation(new Number(5), '/', new Number(0));
        checkThrows("WB-E14: BinOp '/' на 0 → исключение",
                    [&]{ expr->evaluate(); });
        delete expr;
    }

    // ── BinaryOperation: неизвестный оператор ────────────────
    {
        auto* expr = new BinaryOperation(new Number(5), '%', new Number(3));
        checkThrows("WB-E15: BinOp '%' (неизвестный) → исключение",
                    [&]{ expr->evaluate(); });
        delete expr;
    }

    // ── BinaryOperation::type_tag ────────────────────────────
    {
        auto* expr = new BinaryOperation(new Number(1), '+', new Number(1));
        check("WB-E16: BinOp::type_tag == 2", expr->type_tag() == 2);
        delete expr;
    }

    // ── BinaryOperation::nodeLabel ───────────────────────────
    {
        auto* e1 = new BinaryOperation(new Number(1), '+', new Number(1));
        check("WB-E17: nodeLabel '+' == '+'", e1->nodeLabel() == "+");
        delete e1;

        auto* e2 = new BinaryOperation(new Number(1), '-', new Number(1));
        check("WB-E18: nodeLabel '-' == '-'", e2->nodeLabel() == "-");
        delete e2;

        auto* e3 = new BinaryOperation(new Number(1), '*', new Number(1));
        check("WB-E19: nodeLabel '*' == '*'", e3->nodeLabel() == "*");
        delete e3;

        auto* e4 = new BinaryOperation(new Number(1), '/', new Number(1));
        check("WB-E20: nodeLabel '/' == '/'", e4->nodeLabel() == "/");
        delete e4;
    }

    // ── BinaryOperation: потомки ──────────────────────────────
    {
        auto* l = new Number(3);
        auto* r = new Number(4);
        auto* expr = new BinaryOperation(l, '+', r);
        // Потомки должны быть не null
        check("WB-E21: BinOp::leftChild != nullptr",
              expr->leftChild() != nullptr);
        check("WB-E22: BinOp::rightChild != nullptr",
              expr->rightChild() != nullptr);
        // Значения потомков
        check("WB-E23: BinOp::leftChild::evaluate == 3",
              nearEq(expr->leftChild()->evaluate(), 3.0));
        check("WB-E24: BinOp::rightChild::evaluate == 4",
              nearEq(expr->rightChild()->evaluate(), 4.0));
        delete expr; // удаляет l и r через ~BinaryOperation
    }

    // ── check_equals ─────────────────────────────────────────
    {
        Number a(1), b(2);
        check("WB-E25: check_equals — Number vs Number → true",
              check_equals(&a, &b));
    }
    {
        auto* op1 = new BinaryOperation(new Number(1), '+', new Number(2));
        auto* op2 = new BinaryOperation(new Number(3), '*', new Number(4));
        check("WB-E26: check_equals — BinOp vs BinOp → true",
              check_equals(op1, op2));
        delete op1; delete op2;
    }
    {
        Number n(1);
        auto* op = new BinaryOperation(new Number(1), '+', new Number(2));
        check("WB-E27: check_equals — Number vs BinOp → false",
              !check_equals(&n, op));
        check("WB-E28: check_equals — BinOp vs Number → false",
              !check_equals(op, &n));
        delete op;
    }

    // ── Вложенные выражения ───────────────────────────────────
    // (a + b) * c   = (3 + 4) * 2 = 14
    {
        auto* inner = new BinaryOperation(new Number(3), '+', new Number(4));
        auto* outer = new BinaryOperation(inner, '*', new Number(2));
        check("WB-E29: (3+4)*2 = 14", nearEq(outer->evaluate(), 14.0));
        delete outer;
    }
    // a + b * c   = 3 + 4 * 2 = 11  (нет приоритета — зависит от дерева)
    {
        auto* inner = new BinaryOperation(new Number(4), '*', new Number(2));
        auto* outer = new BinaryOperation(new Number(3), '+', inner);
        check("WB-E30: 3+(4*2) = 11", nearEq(outer->evaluate(), 11.0));
        delete outer;
    }
    // Тройное вложение: ((2+3)*4)-5 = 15
    {
        auto* e1 = new BinaryOperation(new Number(2), '+', new Number(3));
        auto* e2 = new BinaryOperation(e1, '*', new Number(4));
        auto* e3 = new BinaryOperation(e2, '-', new Number(5));
        check("WB-E31: ((2+3)*4)-5 = 15", nearEq(e3->evaluate(), 15.0));
        delete e3;
    }

    // ── Граничные числовые значения ──────────────────────────
    {
        auto* expr32 = new BinaryOperation(new Number(5), '+', new Number(0));
        check("WB-E32: 5 + 0 = 5", nearEq(expr32->evaluate(), 5.0));
        delete expr32;
    }
    {
        // Дробные числа
        auto* expr = new BinaryOperation(new Number(1.5), '+', new Number(2.5));
        check("WB-E33: 1.5 + 2.5 = 4.0", nearEq(expr->evaluate(), 4.0));
        delete expr;
    }
    {
        // Отрицательный результат
        auto* expr = new BinaryOperation(new Number(2), '-', new Number(5));
        check("WB-E34: 2 - 5 = -3", nearEq(expr->evaluate(), -3.0));
        delete expr;
    }
    {
        // Умножение на 0
        auto* expr = new BinaryOperation(new Number(999), '*', new Number(0));
        check("WB-E35: 999 * 0 = 0", nearEq(expr->evaluate(), 0.0));
        delete expr;
    }
    {
        // Деление дающее дробь
        auto* expr = new BinaryOperation(new Number(1), '/', new Number(3));
        check("WB-E36: 1/3 ≈ 0.333...", nearEq(expr->evaluate(), 1.0/3.0));
        delete expr;
    }
    {
        // Отрицательное деление
        auto* expr = new BinaryOperation(new Number(-6), '/', new Number(2));
        check("WB-E37: -6/2 = -3", nearEq(expr->evaluate(), -3.0));
        delete expr;
    }
    {
        // Умножение отрицательных: -2 * -3 = 6
        auto* expr = new BinaryOperation(new Number(-2), '*', new Number(-3));
        check("WB-E38: (-2)*(-3) = 6", nearEq(expr->evaluate(), 6.0));
        delete expr;
    }

    // ── Ассоциативность через дерево ─────────────────────────
    {
        // (10 - 3) - 2 = 5
        auto* inner = new BinaryOperation(new Number(10), '-', new Number(3));
        auto* outer = new BinaryOperation(inner, '-', new Number(2));
        check("WB-E39: (10-3)-2 = 5", nearEq(outer->evaluate(), 5.0));
        delete outer;
    }
    {
        // 10 - (3 - 2) = 9
        auto* inner = new BinaryOperation(new Number(3), '-', new Number(2));
        auto* outer = new BinaryOperation(new Number(10), '-', inner);
        check("WB-E40: 10-(3-2) = 9", nearEq(outer->evaluate(), 9.0));
        delete outer;
    }

    // ── type_tag через полиморфизм ────────────────────────────
    {
        // Проверяем через указатель на базовый класс
        Expression* e1 = new Number(1.0);
        Expression* e2 = new BinaryOperation(new Number(1), '+', new Number(2));
        check("WB-E41: полиморфизм — Number::type_tag через Expression* = 1",
              e1->type_tag() == 1);
        check("WB-E42: полиморфизм — BinOp::type_tag через Expression* = 2",
              e2->type_tag() == 2);
        delete e1; delete e2;
    }

    // ── check_equals через полиморфизм ───────────────────────
    {
        Expression* a = new Number(1.0);
        Expression* b = new Number(2.0);
        Expression* c = new BinaryOperation(new Number(1), '+', new Number(2));
        check("WB-E43: check_equals poly — N vs N → true",  check_equals(a, b));
        check("WB-E44: check_equals poly — N vs B → false", !check_equals(a, c));
        check("WB-E45: check_equals poly — B vs N → false", !check_equals(c, a));
        delete a; delete b; delete c;
    }

    // ── Глубоко вложенные выражения ───────────────────────────
    {
        // ((1+2)*(3+4)) / 7 = 3
        auto* s1 = new BinaryOperation(new Number(1), '+', new Number(2));
        auto* s2 = new BinaryOperation(new Number(3), '+', new Number(4));
        auto* prod = new BinaryOperation(s1, '*', s2);
        auto* result = new BinaryOperation(prod, '/', new Number(7));
        check("WB-E46: ((1+2)*(3+4))/7 = 3", nearEq(result->evaluate(), 3.0));
        delete result;
    }
    {
        // 2^10 через умножения: 2*2*2*... (10 шт.)
        Expression* e = new Number(2.0);
        for (int i = 1; i < 10; ++i)
            e = new BinaryOperation(e, '*', new Number(2.0));
        check("WB-E47: 2^10 через дерево = 1024",
              nearEq(e->evaluate(), 1024.0));
        delete e;
    }

    // ── nodeLabel для числа с точкой ─────────────────────────
    {
        Number n(3.14);
        std::string label = n.nodeLabel();
        check("WB-E48: nodeLabel(3.14) не пустой", !label.empty());
    }

    // ── Самопроверка type_tag через check_equals ─────────────
    {
        Number n(0.0);
        check("WB-E49: check_equals(n, n) — один объект → true",
              check_equals(&n, &n));
    }

    // ── BinaryOperation с одинаковыми потомками ───────────────
    {
        auto* expr = new BinaryOperation(new Number(5.0), '+', new Number(5.0));
        check("WB-E50: 5+5 = 10", nearEq(expr->evaluate(), 10.0));
        delete expr;
    }

    // ── Ноль делённый на число ────────────────────────────────
    {
        auto* expr = new BinaryOperation(new Number(0), '/', new Number(5));
        check("WB-E51: 0/5 = 0", nearEq(expr->evaluate(), 0.0));
        delete expr;
    }

    // ── Очень маленькое число ────────────────────────────────
    {
        auto* expr = new BinaryOperation(new Number(1e-10), '*', new Number(1e-10));
        check("WB-E52: 1e-10 * 1e-10 = 1e-20",
              nearEq(expr->evaluate(), 1e-20, 1e-30));
        delete expr;
    }

    // ── Сложение с отрицательным ──────────────────────────────
    {
        auto* expr = new BinaryOperation(new Number(5), '+', new Number(-5));
        check("WB-E53: 5 + (-5) = 0", nearEq(expr->evaluate(), 0.0));
        delete expr;
    }

    // ── Вычитание с отрицательным ─────────────────────────────
    {
        auto* expr = new BinaryOperation(new Number(5), '-', new Number(-3));
        check("WB-E54: 5 - (-3) = 8", nearEq(expr->evaluate(), 8.0));
        delete expr;
    }

    // ── BinOp::type_tag у вложенного тоже 2 ──────────────────
    {
        auto* inner = new BinaryOperation(new Number(1), '+', new Number(2));
        auto* outer = new BinaryOperation(inner, '*', new Number(3));
        check("WB-E55: вложенный BinOp::type_tag == 2", inner->type_tag() == 2);
        delete outer;
    }
}

// ═══════════════════════════════════════════════════════════════════
//  ИТОГ
// ═══════════════════════════════════════════════════════════════════
void printSummary() {
    int total = g_passed + g_failed;
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║           ИТОГИ БЕЛОГО ЯЩИКА                        ║\n";
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
    std::cout << "║   ТЕСТИРОВАНИЕ — БЕЛЫЙ ЯЩИК (White-Box Testing)     ║\n";
    std::cout << "║   Date / Student / Expression                       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";

    test_date_whitebox();
    test_student_whitebox();
    test_expression_whitebox();
    printSummary();

    return g_failed == 0 ? 0 : 1;
}
