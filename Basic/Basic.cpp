/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: happypig]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <ios>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line); // 输入流已设置    值传入
   string token; TokenType token_type;
   if(scanner.hasMoreTokens()) token = scanner.nextToken();
   else return;
   token_type = scanner.getTokenType(token);
   if(token_type == NUMBER){
       int lineNumber;
       try{
           lineNumber = stringToInteger(token);
       } catch (...) {
           cout << "SYNTAX ERROR" << endl;
           return;
       }
       if(!scanner.hasMoreTokens()){
           program.removeSourceLine(lineNumber);
           return;
       }
       try{
           Statement *stmt = parseStmt(scanner,line);
           program.addSourceLine(lineNumber,line); program.setParsedStatement(lineNumber,stmt);
           return;
       } catch (...) {
           cout << "SYNTAX ERROR" << endl;
           return;
       }
   }else if(token_type == WORD){
       if(token == "RUN"){
           if(scanner.hasMoreTokens()){cout << "SYNTAX ERROR" << endl;return;}
           try {
               program.runProgram(state);
           } catch (ErrorException &ex) {
               if(ex.getMessage() == "[tag] end") {
                   return;
               }else if(ex.getMessage() == "[tag] zero") {
                   cout << "DIVIDE BY ZERO" << endl;
                   return;
               }else if(ex.getMessage() == "[tag] goto"){
                   cout << "LINE NUMBER ERROR" << endl;
                   return;
               }else{
                   cout << "VARIABLE NOT DEFINED" << endl;
                   return;
               }
           }
       }
       else if(token == "LIST"){
           if(scanner.hasMoreTokens()){cout << "SYNTAX ERROR" << endl;return;}
           program.showList();
       }
       else if(token == "CLEAR"){
           if(scanner.hasMoreTokens()){cout << "SYNTAX ERROR" << endl;return;}
           program.clear();
           state.clear();
           return;
       }
       else if(token == "QUIT"){
           exit(0);
       }
       else if(token == "HELP"){
           //todo
       }
       else if(token == "LET" || token == "PRINT" || token == "INPUT"){
           scanner.setInput(line);
           Statement *stmt = nullptr;
           try {
               stmt = parseStmt(scanner,line); // 动态对象 需要delete
           } catch (...) {
               cout << "SYNTAX ERROR" << endl;
               return;
           }
           try {
               stmt->execute(state);
               delete stmt;
           } catch (ErrorException &ex) {
               delete stmt;
               if(ex.getMessage() == "[tag] zero"){
                   cout << "DIVIDE BY ZERO" << endl;
                   return;
               }else{
                   cout << "VARIABLE NOT DEFINED" << endl;
                   return;
               }
           }
       }else{
           cout << "SYNTAX ERROR" << endl;
           return;
       }
   }else{
       cout << "SYNTAX ERROR" << endl;
       return;
   }
}
