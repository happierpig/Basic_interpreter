/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"


using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

REM_statement::REM_statement() = default;
REM_statement::~REM_statement()  = default;
void REM_statement::execute(EvalState &state) {}

INPUT_statement::INPUT_statement(string &x) {var_name = x;}
INPUT_statement::~INPUT_statement() = default;
void INPUT_statement::execute(EvalState &state) { // " ? "
    cout << " ? ";
    int var_value;
    string token; TokenType token_type; TokenScanner scanner;
    scanner.ignoreWhitespace(); scanner.scanNumbers();
    while(true){
        scanner.setInput(getLine());
        if(!scanner.hasMoreTokens()){cout << "INVALID NUMBER" << endl << " ? ";continue;}
        token = scanner.nextToken();token_type = scanner.getTokenType(token);
        if(token_type != NUMBER || scanner.hasMoreTokens()){cout << "INVALID NUMBER" << endl << " ? ";continue;}
        var_value = stringToInteger(token);
        state.setValue(var_name,var_value);
        break;
    }
}

PRINT_statement::PRINT_statement(Expression *exp):exp(exp){};
PRINT_statement::~PRINT_statement(){
    delete exp;
}
void PRINT_statement::execute(EvalState &state) {
    cout << exp->eval(state) <<endl;
}

END_statement::END_statement() = default;
END_statement::~END_statement() = default;
void END_statement::execute(EvalState &state) {
    error("[tag] end");
}

LET_statement::LET_statement(Expression *exp):exp(exp){}
LET_statement::~LET_statement(){
    delete exp;
}
void LET_statement::execute(EvalState &state) {
    exp->eval(state);
}

GOTO_statement::GOTO_statement(int a):toLineNumber(a){}
GOTO_statement::~GOTO_statement() = default;
void GOTO_statement::execute(EvalState &state) {
    error(integerToString(toLineNumber));
}

IF_statement::IF_statement(string &a, Expression *b, Expression *c, GOTO_statement *d):op(a),first(b),second(c),go_to(d){}
IF_statement::~IF_statement(){
    delete first;delete second;delete go_to;
}
void IF_statement::execute(EvalState &state) {
    bool flag;
    int var_first = first->eval(state),var_second = second->eval(state);
    if(op == "=") flag = (var_second == var_first);
    if(op == "<") flag = (var_first < var_second);
    if(op == ">") flag = (var_first > var_second);
    if(!flag)return;
    else go_to->execute(state);
}