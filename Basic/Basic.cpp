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
   scanner.setInput(line); // 输入流已设置
   //todo:
   string token; TokenType token_type;
   if(scanner.hasMoreTokens()) token = scanner.nextToken();
   else return;
   token_type = scanner.getTokenType(token);
   if(token_type == NUMBER){
       if(!scanner.hasMoreTokens()){return;} //todo:将program中一行删除
       //todo:将line当作一行命令处理进入program
       try{
           Statement *stmt = parseStmt(scanner);
//           if()
       } catch (...) {

       }
   }else if(token_type == WORD){
       if(token == "RUN"){
           //todo
       }
       if(token == "LIST"){
           //todo
       }
       if(token == "ClEAR"){
           //todo
       }
       if(token == "QUIT"){
           //todo
       }
       if(token == "HELP"){
           //todo
       }
       if(token == "LET" || token == "PRINT" || token == "INPUT"){

       }
       //todo:SYNTAX ERROR
   }else{
       //todo:SYNTAX ERROR
   }
//   Expression *exp = parseExp(scanner);
//   int value = exp->eval(state);
//   cout << value << endl;
//   delete exp;
}
