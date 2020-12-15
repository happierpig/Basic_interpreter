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
   if (scanner.hasMoreTokens()) {
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
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
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

Statement *parseStmt(TokenScanner & scanner){
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
        return new LET_statement(exp);
    }else if(token == "GOTO"){
        //todo:
    }else if(token == "IF"){
        //todo:
    }
    error("[Warning] SYNTAX ERROR");
}
