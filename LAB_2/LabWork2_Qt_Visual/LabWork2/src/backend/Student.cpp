#include "Student.h"
#include <numeric>
#include <sstream>
#include <stdexcept>

//конструктор
Student::Student()
    : m_winterGrades{3,3,3,3,3}, m_summerGrades{3,3,3,3,3} {}

Student::Student(const std::string& group,
                 const std::string& fullName,
                 const std::array<int,GRADES_PER_SESSION>& winterGrades,
                 const std::array<int,GRADES_PER_SESSION>& summerGrades)
    : m_group(group)
    , m_fullName(fullName)
    , m_winterGrades(winterGrades)
    , m_summerGrades(summerGrades)
{}

//get set
const std::string& Student::group()    const { return m_group;    }
const std::string& Student::fullName() const { return m_fullName; }
const std::array<int,Student::GRADES_PER_SESSION>& Student::winterGrades() const { return m_winterGrades; }
const std::array<int,Student::GRADES_PER_SESSION>& Student::summerGrades() const { return m_summerGrades; }

void Student::setGroup   (const std::string& g) { m_group    = g; }
void Student::setFullName(const std::string& n) { m_fullName = n; }
void Student::setWinterGrades(const std::array<int,GRADES_PER_SESSION>& g) { m_winterGrades = g; }
void Student::setSummerGrades(const std::array<int,GRADES_PER_SESSION>& g) { m_summerGrades = g; }


double Student::averageOf(const std::array<int,GRADES_PER_SESSION>& g) const {
    return std::accumulate(g.begin(), g.end(), 0.0) / GRADES_PER_SESSION;
}
int Student::countFails(const std::array<int,GRADES_PER_SESSION>& g) const {
    int n = 0;
    for (int grade : g) if (grade < 3) ++n;
    return n;
}




double Student::avgWinter() const { return averageOf(m_winterGrades); }
double Student::avgSummer() const { return averageOf(m_summerGrades); }

bool Student::hasWinterDebt() const { return countFails(m_winterGrades) > 0; }

bool Student::shouldExpel() const {
    return countFails(m_summerGrades) >= 2 && hasWinterDebt();
}


//допы
std::string Student::serialize() const
{
    std::ostringstream oss;
    oss << m_group << ";" << m_fullName << ";";
    for (int i = 0; i < GRADES_PER_SESSION; ++i)
        oss << m_winterGrades[i] << (i+1<GRADES_PER_SESSION?" ":"");
    oss << ";";
    for (int i = 0; i < GRADES_PER_SESSION; ++i)
        oss << m_summerGrades[i] << (i+1<GRADES_PER_SESSION?" ":"");
    return oss.str();
}

Student Student::deserialize(const std::string& line)
{
    // Split by ';'
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ';'))
        parts.push_back(part);

    if (parts.size() < 4)
        throw std::invalid_argument("Bad student record: " + line);

    auto parseGrades = [](const std::string& s) {
        std::array<int,GRADES_PER_SESSION> g{};
        std::istringstream is(s);
        for (int& v : g) { is >> v; if (!is) v = 3; }
        return g;
    };

    return Student(parts[0], parts[1],
                   parseGrades(parts[2]),
                   parseGrades(parts[3]));
}
