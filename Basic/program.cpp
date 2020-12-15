/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() = default;

Program::~Program() {this->clear();}

void Program::clear() {
    auto it = LineTable.begin();
    while(it != LineTable.end()){
        if(it->second.stmt != nullptr){
            delete it->second.stmt; it->second.stmt = nullptr;
        }
        ++it;
    }
    LineTable.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    if(!this->isContained(lineNumber)){
        this->LineTable.insert(std::make_pair(lineNumber,Line(line)));
    }else{
        auto it = this->LineTable.find(lineNumber);
        it->second.source_Line = line; delete it->second.stmt;it->second.stmt = nullptr;
    }
}

void Program::removeSourceLine(int lineNumber) {
    if(!this->isContained(lineNumber)) return;
    auto it = this->LineTable.find(lineNumber);
    delete it->second.stmt;it->second.stmt = nullptr;
    this->LineTable.erase(it);
}

string Program::getSourceLine(int lineNumber) {
   if(!this->isContained(lineNumber)) return "";
   return (this->LineTable.find(lineNumber))->second.source_Line;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(!this->isContained(lineNumber)) error("[Warning] SYNTAX ERROR in setParsedStatement");
    auto it = this->LineTable.find(lineNumber);
    delete it->second.stmt; it->second.stmt = stmt;
}

Statement * Program::getParsedStatement(int lineNumber) {
    if(!this->isContained(lineNumber)) error("[Warning] SYNTAX ERROR in getParsedStatement");
    return (this->LineTable.find(lineNumber))->second.stmt;
}

int Program::getFirstLineNumber(){return (this->LineTable.begin()->first);}

int Program::getNextLineNumber(int lineNumber) {
    auto it = this->LineTable.upper_bound(lineNumber);
    if(it == this->LineTable.end()) return -1;
    else return it->first;
}

bool Program::isContained(int lineNumber){
    if(LineTable.empty()){return false;}
    auto it = LineTable.find(lineNumber);
    if(it == LineTable.end()) return false;
    else return true;
}

void Program::showList(){
    if(this->LineTable.empty()){return;}
    else{
        auto it = this->LineTable.begin();
        while(it != this->LineTable.end()){
            cout << it->second.source_Line << endl;
            ++it;
        }
        return;
    }
}

void Program::runProgram(EvalState &state) {
    if(this->LineTable.empty()) return;
    auto it = this->LineTable.begin();
    while(it != this->LineTable.end()){
        (it->second.stmt)->execute(state);
        ++it;
    }
}