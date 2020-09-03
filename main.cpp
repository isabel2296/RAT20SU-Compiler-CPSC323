//CPSC 323 Lexer Assigment
#include "SA.h"

using namespace std;

// global variables
vector <Token> token; // holds our tokens
ifstream file;
int line = 1;


bool isSeparator(char c) {
    return c == ',' || c == ';' || c == '(' || c == ')' ||
        c == '[' || c == ']' || c == '{' || c == '}' || c == '$';
} //checking for separator
bool isword(char c) {
    c = tolower(c);
    return c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
        c == 'e' || c == 'f' || c == 'g' || c == 'h' ||
        c == 'i' || c == 'j' || c == 'k' || c == 'l' ||
        c == 'm' || c == 'n' || c == 'o' ||
        c == 'p' || c == 'q' || c == 'r' || c == 's' ||
        c == 't' || c == 'u' || c == 'v' || c == 'w' ||
        c == 'x' || c == 'y' || c == 'z' || c == '_';
}//checking for characters
bool isInteger(char c) {
    return c == '0' || c == '1' || c == '2' || c == '3' ||
        c == '4' || c == '5' || c == '6' || c == '7' ||
        c == '8' || c == '9' || c == '.';
} //if a number return true
bool iskeyword(string t) {
    return t == "int" || t == "if" || t == "otherwise" ||
        t == "fi" || t == "while" || t == "return" ||
        t == "get" || t == "put" || t == "endl" ||
        t == "true" || t == "false" || t == "print" ||
        t == "scan" || t == "get" ||
        t == "integer" || t == "boolean";
} // checking to see if its a keyword
bool isOperator(char c) {
    return c == '=' || c == '>' || c == '<' || c == '*' ||
        c == '-' || c == '+' || c == '/';
}// if char operator return true

//FSM
void identifierFSM(char c) {
    bool valid = true;
    string total = "";
    enum states {
        firsttime, check, addingletter, addingunderscore, complete
    };

    enum states state = firsttime;

    if (c == '_') {
        valid = false;
    }
    while (!file.eof()) {
        switch (state) {
        case firsttime:
            if (c == '_') {
                valid = false;
            }
            total += c;
            state = check;
            break;
        case check:
            file.get(c);

            if (isword(c) && c != '_') {
                state = addingletter;
            }
            else if (c == '_') {
                state = addingunderscore;
            }
            else if (isInteger(c)) {
                valid = false;
                state = check;
            }
            else {
                file.unget();
                state = complete;
            }
            break;
        case addingletter:
            total += c;
            state = check;
            break;
        case addingunderscore:
            total += c;
            state = check;
            break;
        case complete:
            if (iskeyword(total)) {
                Token current;
                current.type = "keyword";
                current.lexeme = total;
                current.line = line;
                token.push_back(current);
            }
            else if (valid == true) {
                Token current;
                current.type = "identifier";
                current.lexeme = total;
                current.line = line;
                token.push_back(current);
            }
            else {
                Token current;
                current.type = "unknown";
                current.lexeme = total;
                current.line = line;
                token.push_back(current);
            }
            return;
        }
    }
}

void integerFSM(char c) {
    bool valid = true;
    enum states {
        check, add, complete
    };

    enum states state = check;
    string total = "";
    total += c;

    while (true) {
        switch (state) {
        case check:
            file.get(c);
            if (isInteger(c)) {
                state = add;
            }
            else if (isword(c)) {
                valid = false;
                state = add;
                break;
            }
            else {
                file.unget();
                state = complete;
                break;
            }
        case add:
            total += c;
            state = check;
            break;
        case complete:
            if (valid) {
                Token current;
                current.type = "integer";
                current.lexeme = total;
                current.line = line;
                token.push_back(current);
            }
            else {
                Token current;
                current.type = "unknown";
                current.lexeme = total;
                current.line = line;
                token.push_back(current);
            }
            return;
        }
    }
}


void lexer(char c) {
    Token current;

    if (isSeparator(c)) {

        string doubledollarcheck;
        doubledollarcheck = c;
        if (c == '$') {
            file.get(c);
            if (c == '$') { doubledollarcheck = "$$"; }
            else { file.unget(); }
        }

        current.type = "separator";
        current.lexeme = doubledollarcheck;
        current.line = line;
        token.push_back(current);
    }
    else if (isOperator(c)) {
        string doubleequalcheck;
        doubleequalcheck += c;
        if (c == '=') {
            file.get(c);
            if (c == '=') { doubleequalcheck = "=="; }
            else { file.unget(); }
        }
        current.type = "operator";
        current.lexeme = doubleequalcheck;
        current.line = line;
        token.push_back(current);
    }
    else if (isword(c)) {
        identifierFSM(c);
    }
    else if (isInteger(c)) {
        integerFSM(c);
    }
}


int main() {
    char c;
    bool commenting = false;

    string filename;
    cout << "Please enter your text file name: ";
    cin >> filename;

    file.open(filename);
    while (!file.is_open()) {
        cout << "File not found. Please try again\nFile Name (0 to exit): ";
        cin >> filename;
        file.open(filename);
        if (filename == "0") {
            exit(0);
        }
    }// can make this end at the end so it can continue asking for a new file
    // untill the user wishes to end the execution

    while (!file.eof()) {
        file.get(c);
        if (c == '\n') { line++; }


        //ignores comments
        if (c == '[') {
            if ((file.get()) == '*') { commenting = true; }
            else { file.unget(); }
        }
        else if (commenting == true) {
            if (c == '*') {
                if ((file.get()) == ']') {
                    commenting = false;
                    file.get(c);
                    if (c == '\n') { line++; }
                }
                else {
                    file.unget();
                }
            }
        }

        //starts to iterate through the lexer
        if (c != '\n' && !commenting) { lexer(c); }
    }

    file.close();

    //will start SA
    SA n;
    n.copyTokensInit(token);
    n.Rat20SU();
    n.printAssembly();
    return 0;
}




