#pragma once
#include <string>
#include <vector>
#include <array>


class Student {
public:
    static constexpr int GRADES_PER_SESSION = 5;

    Student();
    Student(const std::string& group,
            const std::string& fullName,
            const std::array<int, GRADES_PER_SESSION>& winterGrades,
            const std::array<int, GRADES_PER_SESSION>& summerGrades);

    // ── Getters ──────────────────────────────────────────────
    const std::string& group()    const;
    const std::string& fullName() const;
    const std::array<int, GRADES_PER_SESSION>& winterGrades() const;
    const std::array<int, GRADES_PER_SESSION>& summerGrades() const;

    // ── Setters ──────────────────────────────────────────────
    void setGroup   (const std::string& g);
    void setFullName(const std::string& n);
    void setWinterGrades(const std::array<int, GRADES_PER_SESSION>& g);
    void setSummerGrades(const std::array<int, GRADES_PER_SESSION>& g);

  
    double avgWinter() const;   // средний балл зимней сессии
    double avgSummer() const;   // средний балл летней сессии

    bool hasWinterDebt()  const;  // есть хотя бы одна 2 зимой
    bool shouldExpel()    const;  // ≥2 неуд летом И долг зимой

    // ── Serialisation ────────────────────────────────────────
    std::string serialize()   const;  // CSV line
    static Student deserialize(const std::string& line);

private:
    std::string m_group;
    std::string m_fullName;
    std::array<int, GRADES_PER_SESSION> m_winterGrades;
    std::array<int, GRADES_PER_SESSION> m_summerGrades;

    double averageOf(const std::array<int, GRADES_PER_SESSION>& g) const;
    int    countFails(const std::array<int, GRADES_PER_SESSION>& g) const;
};
