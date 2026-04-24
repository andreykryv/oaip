#include "StudentController.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <stdexcept>

StudentController::StudentController(QObject* parent) : QObject(parent) {}

QVariantMap StudentController::studentToMap(int i) const {
    const Student& s = m_students[i];
    QVariantMap m;
    m["grp"]=QString::fromStdString(s.group());
    m["nm"] =QString::fromStdString(s.fullName());
    const auto& w=s.winterGrades(); const auto& su=s.summerGrades();
    m["w1"]=w[0];m["w2"]=w[1];m["w3"]=w[2];m["w4"]=w[3];m["w5"]=w[4];
    m["s1"]=su[0];m["s2"]=su[1];m["s3"]=su[2];m["s4"]=su[3];m["s5"]=su[4];
    m["avgW"]=QString::number(s.avgWinter(),'f',2);
    m["avgS"]=QString::number(s.avgSummer(),'f',2);
    m["st"]=s.shouldExpel()?"expel":s.hasWinterDebt()?"debt":"ok";
    m["idx"]=i;
    return m;
}
QVariantList StudentController::allStudents() const {
    QVariantList list;
    for (int i=0;i<static_cast<int>(m_students.size());++i) list.append(studentToMap(i));
    return list;
}
int StudentController::debtCount() const {
    return std::count_if(m_students.begin(),m_students.end(),
        [](const Student& s){ return s.hasWinterDebt()&&!s.shouldExpel(); });
}
int StudentController::expelCount() const {
    return std::count_if(m_students.begin(),m_students.end(),
        [](const Student& s){ return s.shouldExpel(); });
}
int StudentController::okCount() const {
    return static_cast<int>(m_students.size())-debtCount()-expelCount();
}
void StudentController::addStudent(const QString& group,const QString& name,
                                    const QVariantList& winter,const QVariantList& summer) {
    if (winter.size()!=5||summer.size()!=5){emit error("Нужно ровно 5 оценок");return;}
    if (group.isEmpty()){emit error("Группа не может быть пустой");return;}
    if (name.isEmpty()) {emit error("ФИО не может быть пустым");return;}
    std::array<int,5> w{},s{};
    for(int i=0;i<5;++i){w[i]=winter[i].toInt();s[i]=summer[i].toInt();}
    m_students.emplace_back(group.toStdString(),name.toStdString(),w,s);
    emit studentsChanged();
}
void StudentController::deleteStudent(int index) {
    if(index<0||index>=static_cast<int>(m_students.size())) return;
    m_students.erase(m_students.begin()+index); emit studentsChanged();
}
void StudentController::editStudent(int index,const QString& group,const QString& name,
                                     const QVariantList& winter,const QVariantList& summer) {
    if(index<0||index>=static_cast<int>(m_students.size())) return;
    if(winter.size()!=5||summer.size()!=5){emit error("Нужно ровно 5 оценок");return;}
    if(group.isEmpty()){emit error("Группа пустая");return;}
    std::array<int,5> w{},s{};
    for(int i=0;i<5;++i){w[i]=winter[i].toInt();s[i]=summer[i].toInt();}
    m_students[index]=Student(group.toStdString(),name.toStdString(),w,s);
    emit studentsChanged();
}
void StudentController::openFile(const QString& path) {
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){emit error("Не удалось открыть: "+path);return;}
    m_students.clear();
    QTextStream in(&file);
    while(!in.atEnd()){
        QString line=in.readLine().trimmed();
        if(line.isEmpty()||line.startsWith('#')) continue;
        try { m_students.push_back(Student::deserialize(line.toStdString())); }
        catch(const std::exception& e){ emit error(QString("Ошибка: %1").arg(e.what())); }
    }
    emit studentsChanged();
}
void StudentController::saveFile(const QString& path) {
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){emit error("Не удалось сохранить: "+path);return;}
    QTextStream out(&file);
    for(const auto& s:m_students) out<<QString::fromStdString(s.serialize())<<"\n";
}
QVariantList StudentController::filtered(int filterType,const QString& nameSearch) const {
    QVariantList list;
    for(int i=0;i<static_cast<int>(m_students.size());++i){
        const Student& s=m_students[i];
        if(!nameSearch.isEmpty()&&!QString::fromStdString(s.fullName()).toLower().contains(nameSearch.toLower())) continue;
        if(filterType==1&&!(s.hasWinterDebt()&&!s.shouldExpel())) continue;
        if(filterType==2&&!s.shouldExpel())   continue;
        list.append(studentToMap(i));
    }
    return list;
}
QVariantMap StudentController::getStudent(int index) const {
    if(index<0||index>=static_cast<int>(m_students.size())) return {};
    return studentToMap(index);
}
void StudentController::applySort() {
    if(m_sortColumn=="name")
        std::sort(m_students.begin(),m_students.end(),[this](const Student& a,const Student& b){
            return m_sortAsc?a.fullName()<b.fullName():a.fullName()>b.fullName();});
    else if(m_sortColumn=="group")
        std::sort(m_students.begin(),m_students.end(),[this](const Student& a,const Student& b){
            return m_sortAsc?a.group()<b.group():a.group()>b.group();});
    else if(m_sortColumn=="avg")
        std::sort(m_students.begin(),m_students.end(),[this](const Student& a,const Student& b){
            return m_sortAsc?a.avgSummer()<b.avgSummer():a.avgSummer()>b.avgSummer();});
    emit studentsChanged(); emit sortChanged();
}
void StudentController::sortByName() {
    if(m_sortColumn=="name") { m_sortAsc=!m_sortAsc; }
    else { m_sortColumn="name"; m_sortAsc=true; }
    applySort();
}
void StudentController::sortByGroup() {
    if(m_sortColumn=="group") { m_sortAsc=!m_sortAsc; }
    else { m_sortColumn="group"; m_sortAsc=true; }
    applySort();
}
void StudentController::sortByAvgDesc() {
    if(m_sortColumn=="avg") { m_sortAsc=!m_sortAsc; }
    else { m_sortColumn="avg"; m_sortAsc=false; }
    applySort();
}
