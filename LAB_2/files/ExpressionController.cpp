#include "ExpressionController.h"
#include "Expression.h"
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <sstream>

ExpressionController::ExpressionController(QObject* parent) : QObject(parent) {}

namespace {
struct Parser {
    const std::string& s; size_t pos=0;
    void skipWS(){while(pos<s.size()&&std::isspace((unsigned char)s[pos]))++pos;}
    Expression* parseExpr();
    Expression* parseTerm();
    Expression* parseFactor();
    double parseNum(){
        skipWS(); size_t start=pos;
        if(pos<s.size()&&(s[pos]=='-'||s[pos]=='+'))++pos;
        while(pos<s.size()&&(std::isdigit((unsigned char)s[pos])||s[pos]=='.'))++pos;
        if(pos==start) throw std::invalid_argument("Expected number");
        return std::stod(s.substr(start,pos-start));
    }
};
Expression* Parser::parseFactor(){
    skipWS();
    if(pos<s.size()&&s[pos]=='('){++pos;auto*e=parseExpr();skipWS();if(pos<s.size()&&s[pos]==')')++pos;return e;}
    if(pos<s.size()&&s[pos]=='-'){++pos;skipWS();
        if(pos<s.size()&&(std::isdigit((unsigned char)s[pos])||s[pos]=='.')) return new Number(-parseNum());
        return new BinaryOperation(new Number(0),'-',parseFactor());
    }
    return new Number(parseNum());
}
Expression* Parser::parseTerm(){
    auto*l=parseFactor();
    while(true){skipWS();if(pos>=s.size())break;char op=s[pos];if(op!='*'&&op!='/')break;++pos;auto*r=parseFactor();l=new BinaryOperation(l,op,r);}
    return l;
}
Expression* Parser::parseExpr(){
    auto*l=parseTerm();
    while(true){skipWS();if(pos>=s.size())break;char op=s[pos];if(op!='+'&&op!='-')break;++pos;auto*r=parseTerm();l=new BinaryOperation(l,op,r);}
    return l;
}
QVariantMap buildMap(Expression* node){
    if(!node) return {};
    QVariantMap m;
    m["label"]=QString::fromStdString(node->nodeLabel());
    m["isOp"]=(node->type_tag()==2);
    if(node->leftChild())  m["left"] =buildMap(node->leftChild());
    if(node->rightChild()) m["right"]=buildMap(node->rightChild());
    return m;
}
} // namespace

QString ExpressionController::evaluate(const QString& expression){
    std::string s=expression.toStdString();
    if(s.empty()){emit error("Выражение пустое");return "Ошибка: пустое выражение";}
    try{
        Parser p{s};
        Expression* expr=p.parseExpr();
        double result=expr->evaluate();
        delete expr;
        double rounded=std::round(result*1e10)/1e10;
        return QString::number(rounded,'g',10);
    }catch(const std::exception& e){
        emit error(QString("Ошибка: %1").arg(e.what()));
        return QString("Ошибка: %1").arg(e.what());
    }
}
QString ExpressionController::evaluateEmpty(){ return evaluate(""); }

bool ExpressionController::checkEquals(int typeA, int typeB){
    Expression* l=(typeA==0)?(Expression*)new Number(0):new BinaryOperation(new Number(0),'+',new Number(0));
    Expression* r=(typeB==0)?(Expression*)new Number(0):new BinaryOperation(new Number(0),'+',new Number(0));
    bool res=check_equals(l,r);
    delete l; delete r;
    return res;
}
QVariantMap ExpressionController::parseTree(const QString& expression){
    std::string s=expression.toStdString();
    try{
        Parser p{s};
        Expression* root=p.parseExpr();
        QVariantMap res=buildMap(root);
        delete root;
        return res;
    }catch(const std::exception& e){
        emit error(QString("Ошибка парсинга: %1").arg(e.what()));
        return {};
    }
}
