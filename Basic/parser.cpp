/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"

using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 * 大量抛出需要处理
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {///不能用于在IF时读入
       delete exp;///有待验证
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token)); // 自动报错
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}

Statement *parseStmt(TokenScanner & scanner,string line){
    string token = scanner.nextToken(); TokenType token_type = scanner.getTokenType(token);
    Expression * exp = nullptr;
    if(token_type != WORD){
        error("[Warning] SYNTAX ERROR");
    }else if(token == "PRINT"){
        exp = parseExp(scanner);//if failed ,need to be freed
        if(exp->getType() == COMPOUND){
            if(((CompoundExp *)exp)->getOp() == "="){delete exp;error("[Warning] SYNTAX ERROR");}
        }
        return new PRINT_statement(exp);
    }else if(token == "INPUT"){
        if(!scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        token = scanner.nextToken();token_type = scanner.getTokenType(token);
        if(token_type != WORD){error("[Warning] SYNTAX ERROR");}
        if(scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        return new INPUT_statement(token);
    }else if(token == "REM"){
        return new REM_statement();
    }else if(token == "END"){
        if(scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        return new END_statement();
    }else if(token == "LET"){
        if(!scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        exp = parseExp(scanner);
        if(exp->getType() != COMPOUND){delete exp;error("[Warning] SYNTAX ERROR");}
        if(((CompoundExp *)exp)->getOp() != "="){delete exp;error("[Warning] SYNTAX ERROR");}
        if((((CompoundExp *)exp)->getLHS())->getType() != IDENTIFIER){delete exp;error("[Warning] SYNTAX ERROR");}
        if(!checkLegal(((IdentifierExp *)(((CompoundExp *)exp)->getLHS()))->getName())){delete exp;error("[Warning] SYNTAX ERROR");}
        return new LET_statement(exp);
    }else if(token == "GOTO"){
        if(!scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        token = scanner.nextToken();token_type = scanner.getTokenType(token);
        if(token_type != NUMBER){error("[Warning] SYNTAX ERROR");}
        if(scanner.hasMoreTokens()){error("[Warning] SYNTAX ERROR");}
        int toLineNumber;
        try{
            toLineNumber = stringToInteger(token);
        } catch (...) {
            error("[Warning] SYNTAX ERROR");
        }
        return new GOTO_statement(toLineNumber);
    }else if(token == "IF"){
        if (!scanner.hasMoreTokens()) { error("[Warning] SYNTAX ERROR"); }
        string op;
        Expression *first;
        Expression *second;
        GOTO_statement *go_to;
        if(line.find('=') == string::npos) {
            first = readE(scanner);
            op = scanner.nextToken();
            if (op != "<" && op != ">" && op != "=") {delete first;error("[Warning] SYNTAX ERROR");}
            try {
                second = readE(scanner);
            } catch (...) {
                delete first; error("[Warning] SYNTAX ERROR");
            }
            token = scanner.nextToken();
            if (token != "THEN") {delete first;delete second;error("[Warning] SYNTAX ERROR");}
            token = scanner.nextToken();
            token_type = scanner.getTokenType(token);
            if (token_type != NUMBER) {delete first;delete second;error("[Warning] SYNTAX ERROR");}
            if (scanner.hasMoreTokens()) {delete first;delete second;error("[Warning] SYNTAX ERROR");}
            int toLineNumber;
            try {
                toLineNumber = stringToInteger(token);
            } catch (...) {
                delete first;delete second;
                error("[Warning] SYNTAX ERROR");
            }
            go_to = new GOTO_statement(toLineNumber);
            return new IF_statement(op, first, second, go_to);
        }else{
            if(line.find_first_of('=') != line.find_last_of('=')){error("[Warning] SYNTAX ERROR");}
            op = "=";
            string sonString;
            while(scanner.hasMoreTokens()){
                token = scanner.nextToken();
                if(token == "=") break;
                sonString += (token + " ");
            }
            second = readE(scanner);
            token = scanner.nextToken();
            if(token != "THEN"){delete second;error("[Warning] SYNTAX ERROR");}
            token = scanner.nextToken();token_type = scanner.getTokenType(token);
            if(token_type != NUMBER){delete second;error("[Warning] SYNTAX ERROR");}
            if (scanner.hasMoreTokens()) {delete second;error("[Warning] SYNTAX ERROR");}
            int toLineNumber;
            try {
                toLineNumber = stringToInteger(token);
            } catch (...) {
                delete second;
                error("[Warning] SYNTAX ERROR");
            }
            scanner.setInput(sonString);
            try{
                first = readE(scanner);
            } catch (...) {
                delete second;
                error("[Warning] SYNTAX ERROR");
            }
            go_to = new GOTO_statement(toLineNumber);
            return new IF_statement(op, first, second, go_to);
        }
    }
    error("[Warning] SYNTAX ERROR");
}

bool checkLegal(string token){
    if(token == "LET" || token == "REM" || token == "PRINT" || token == "END" || token == "IF" ||
    token == "THEN" || token == "GOTO" || token == "RUN" || token == "LIST" || token == "CLEAR" ||
    token == "QUIT" || token == "HELP"){return false;}
    else return true;
}