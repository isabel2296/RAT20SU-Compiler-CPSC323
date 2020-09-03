#ifndef COMPILER1STASSIGNMENT_SA_H
#define COMPILER1STASSIGNMENT_SA_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <sstream>

using namespace std;
//ofstream outFileA3;


struct Token {// created a struct to keep track of the tokens and their values
    string type; //aka identifier, keywords, integers,operators,seperators
    string lexeme; // the value of the token
    int line;
    int prev_Line;
};// token object
/*=================================================================== */
//part1

class Symbol_Table {
public:
    string id; // identifer
    int address;
    string type; // lexer type
};

bool inSymTable(string id); // check if symbol is in table
int getAddress(string id);
void insertSym(string id, string type); // this is where we check type and Symbol
void printSym(Symbol_Table symTable);
/*=================================================================== */

/*=================================================================== */
//part 2
// may not need this / just fibbing a little

//template <typename T>
class OPP_Table {
public:
    int address;
    int oprnd; // changed this to int
    string op;
};


void gen_instr(string op, string oprnd);
void gen_instr(string op, int oprnd);
void back_patch(int jump_addr);
void print_instr(OPP_Table table);



/*=================================================================== */


class SA {
private:
    vector<Token> tokens;
    Token current;
    ofstream outFile;
    ofstream outAssembly;
    string pre_type;
    Token save;
public:
    void copyTokensInit(vector<Token> original);
    Token lexer_get();
    void exiting();
    void printTokens();

    void Rat20SU();
    void printAssembly();

    void OptDeclarationList();
    void DeclarationList();
    void DeclarationListPrime();
    void Declaration();
    void Qualifier();
    void StatementList();
    void StatementListPrime();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void IfPrime();
    void Put();
    void Get();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void Expression_Prime();
    void Term();
    void Term_Prime();
    void Factor();
    void Primary();
    void Empty();
};

#endif //COMPILER1STASSIGNMENT_SA_H
