#include "SA.h"
// ASSIGN 3
//=================================================
int Mem_addr = 5000; //incremented by 1
Symbol_Table* symbolTable = new Symbol_Table[300];// max number of var
OPP_Table* instr_table = new OPP_Table[300];
stack<int> backTrack;
int instr_address = 0;

bool inSymTable(string id) {
    for (int i = 0; i < Mem_addr - 5000; i++) {
        if (symbolTable[i].id == id) {
            return true;
        }
    }
    return false;
}

int getAddress(string id) {
    for (int i = 0; i < Mem_addr - 5000; i++) {
        if (symbolTable[i].id == id)
            return symbolTable[i].address;
    }
    return 0;
}

void insertSym(string id, string type) {

    if (inSymTable(id)) { 
        cout << "\nError: Cant have two initializations of variable\n";
        exit(0);
    }
    symbolTable[Mem_addr - 5000].address = Mem_addr;
    symbolTable[Mem_addr - 5000].id = id;
    symbolTable[Mem_addr - 5000].type = type;
    Mem_addr++;

} // this is where we check type and Symbol

void printSym(Symbol_Table* symTable) {
    cout << "\nSymbol Table: ";
    for (int i = 0; i < Mem_addr - 5000; i++)
    {
        cout << endl << symTable[i].id << " " << " " << symTable[i].type << " " << symTable[i].address;
    }
    cout << endl;
}

//==================================================
//PART 2

void gen_instr(string operation, int oprnd) {
    //  cout << "goes through gen_instr" ;
    instr_table[instr_address].address = instr_address + 1;
    instr_table[instr_address].op = operation;
    instr_table[instr_address].oprnd = oprnd;
    instr_address++;
}
void gen_instr(string operation, string oprnd) {
    instr_table[instr_address].address = instr_address + 1;
    instr_table[instr_address].op = operation;
    //instr_table[instr_address].oprnd = oprnd;
    instr_address++;
}
void print_instr(OPP_Table* table) {
    cout << "\n";
    for (int i = 0; i < instr_address; i++)
    {   //added this in replacement of "nil" since I changed the oprnd to int instead of string
        string  instr[] = { "STDOUT", "STDIN","ADD","SUB","MUL","DIV","GRT","LES","EQU","LABEL" };
        if (table[i].op == instr[0] || table[i].op == instr[1] || table[i].op == instr[2] || table[i].op == instr[3] || table[i].op == instr[4] || table[i].op == instr[5] || table[i].op == instr[6]
            || table[i].op == instr[7] || table[i].op == instr[7] || table[i].op == instr[9])
            cout << table[i].address << " " << table[i].op << endl;
        else
            cout << table[i].address << " " << table[i].op << " " << table[i].oprnd << endl;
    }
}


// backpatching is a process in which the operand field of an instruction containing a forward reference
// is left blank initially. the address of the forward reference symbol is put into this field when
// its definition is encountered in the program
int pop_stack() {
    int ret = 0;
    if (!backTrack.empty())
    {
        ret = backTrack.top();
        backTrack.pop();  
        return ret;
    }
    else return ret;
}
void push_stack(int address) {
    backTrack.push(address); //backTrack is a stack
    return;
}

void back_patch(int jump_addr) {
    int addr = pop_stack(); //pop_jumpstack needs definition
    instr_table[addr].oprnd = jump_addr + 1; //changed the opprd to int to be able to do this ..
    //its 12:25 am kinda sleepy but rewrote this ill check back later:w
}

// for LA not for SA
void SA::exiting() {
    cout << "\n\nError please go to out_SA.txt for more details\n";
    system("pause"); // we might need this again for ASIG3
    exit(0);
}
void SA::printTokens() {
    // print tokens to the output file
    ofstream Outputfile;
    Outputfile.open("cmd.txt"); // put a file path here
}

void SA::printAssembly() {
    outAssembly.open("outAssembly.txt");
    outAssembly << "\nSymbol Table: ";
    for (int i = 0; i < Mem_addr - 5000; i++)
    {
        outAssembly << endl << symbolTable[i].id << " " << " " << symbolTable[i].type << " " << symbolTable[i].address;
    }

    outAssembly << "\n\n"; 

    for (int i = 0; i < instr_address; i++)
    {   //added this in replacement of "nil" since I changed the oprnd to int instead of string
        string  instr[] = { "STDOUT", "STDIN","ADD","SUB","MUL","DIV","GRT","LES","EQU","LABEL" };
        if (instr_table[i].op == instr[0] || instr_table[i].op == instr[1] || instr_table[i].op == instr[2] || instr_table[i].op == instr[3] ||
            instr_table[i].op == instr[4] || instr_table[i].op == instr[5] || instr_table[i].op == instr[6]
            || instr_table[i].op == instr[7] || instr_table[i].op == instr[7] || instr_table[i].op == instr[9])
            outAssembly << instr_table[i].address << " " << instr_table[i].op << endl;
        else
            outAssembly << instr_table[i].address << " " << instr_table[i].op << " " << instr_table[i].oprnd << endl;
    }
}

void SA::copyTokensInit(vector<Token> original) {
   // outFile.open("out_SA.txt");

    for (int i = 0; i < original.size(); i++)
    {
        int temp = original[i].line;
        if (i < original.size() - 1)
            original[i + 1].prev_Line = temp;
        SA::tokens.push_back(original[i]);
        if (original[i].type == "unknown")
        {
            cout << "\n*********\nError: Unknown token not allowed on line: " << original[i].line << "\n*********";
            exiting();
        }
    }
    current = tokens[0];
    //outFile << "\n\nToken = " << current.type << "\tLexeme: " << current.lexeme;
}

Token SA::lexer_get() {
    Token temp;
    if (!tokens.empty())
    {
        tokens.erase(tokens.begin());
        if (!tokens.empty()) {
            temp = tokens[0];

            outFile << "\n\nToken = " << temp.type << "\tLexeme: " << temp.lexeme;
            return temp;
        }
    }
    return temp;
}

// start of Syntax Analayis

void SA::Rat20SU() {
    //outFile << "\n<Rat20SU>";
    //<Rat20SU> =  $$ <Opt Declaration List>  <Statement List> $$
    if (current.lexeme == "$$")
    {
        current = lexer_get(); //grab next token
        OptDeclarationList(); // call opt declaration list
        StatementList();  //statement list
        if (current.lexeme != "$$")
        {
            cout << "\nError missing: '$$' in line: " << current.line;
            exiting();
        }
        else { printSym(symbolTable); print_instr(instr_table); }
    }
    else
    {
        cout << "\nError missing $$ at starting point line number = " << current.prev_Line;
        exiting();
    }
}

void SA::OptDeclarationList() {
    //outFile << "\n<OptDeclarationList>";
    //declaration list | empty
    DeclarationList();

    Empty();
}
void SA::DeclarationList() {
    //outFile << "\n<DeclarationList>";
    Declaration();
    if (current.lexeme == ";") {
        current = lexer_get();
        DeclarationListPrime();
    }
    else {
        cout << "\nError: missing ; in line: " << current.prev_Line;
        exiting();
    }
}
void SA::DeclarationListPrime() {
    //outFile << "\n<DeclarationListPrime>";
    if (current.lexeme == "integer" || current.lexeme == "boolean") {
        DeclarationList();
    }
    else;
}

void SA::Declaration() {
    //outFile << "\n<Declaration>";
    Qualifier();
    if ((current = lexer_get()).type == "identifier")
    {
        //outFile << "\n<identifier>";
        insertSym(current.lexeme, pre_type); //we are putting integer or boolean as the type asking if we should be doing identifier as type on wednesday
        current = lexer_get();
    }
}


void SA::Qualifier() {
    //outFile << "\n<Qualifier>";
    if (current.lexeme == "integer")
    {
        //outFile << "-> integer";
        pre_type = current.lexeme;
    }
    else if (current.lexeme == "boolean")
    {
        //outFile << "-> boolean";
        pre_type = current.lexeme;
    }
    else {
        cout << "\nError: missing qualifier integer or boolean in line:" << current.prev_Line;
        exiting();
    }
}
void SA::StatementList() {
    //outFile << "\n<StatementList>";
    Statement();
    StatementListPrime();
}

void SA::StatementListPrime() {
    //outFile << "\n<StatementListPrime>";
    if (current.lexeme == "{" || current.type == "identifier" ||
        current.lexeme == "if" || current.lexeme == "get" ||
        current.lexeme == "put" || current.lexeme == "while")
        StatementList();
    else if (current.lexeme == "integer" || current.lexeme == "boolean")outFile << "\nError: unable to declare after initilizing";
    else;
}
void SA::Statement() {
    //outFile << "\n<Statement>";
    Compound();
    Assign();
    If();
    Get();
    Put();
    While();
}

void SA::Compound() {
    if (current.lexeme == "{")
    {
        //outFile << "\n<Compound>";
        current = lexer_get();
        StatementList();
        if (current.lexeme != "}")
        {
            cout << "\nError: missing } at line: " << current.prev_Line;
            exiting();
        }
        else;//current = lexer_get(); //idk if we should keep this
    }
    else;
}

void SA::Assign() {
    if (current.type == "identifier")
    {
        save = current; // save = token
        //outFile << "\n<Assign>";
        current = lexer_get(); // lexer()
        if (current.lexeme == "=")
        {
            current = lexer_get();
            Expression();
            gen_instr("POPM", getAddress(save.lexeme));
            if (current.lexeme != ";") {
                cout << "\nError: missing ';' in line: " << current.prev_Line;
                exiting();
            }
            else current = lexer_get(); //not sure if this is right
        }
        else {
            cout << "\nError: Missing = in assignment line:  " << current.prev_Line;
            exiting();
        }
    }
    else;
}

void SA::If() {
    if (current.lexeme == "if")
    {
        int addr = instr_address;
        //outFile << "\n<If>";
        current = lexer_get();
        if (current.lexeme == "(") {
            current = lexer_get();
            Condition();
            if (current.lexeme == ")")
            {
                current = lexer_get();
                Statement();
                IfPrime();
            }
            else {
                cout << "\nError: Missing ) in if() line: " << current.prev_Line;
                exiting();
            }
        }
        else {
            cout << "\nError: Missing ( in if() line: " << current.prev_Line;
            exiting();
        }
    }
    else;
}

void SA::IfPrime() {
    //outFile << "\n<IfPrime>";
    back_patch(instr_address);
    //placing this in IF maybe
    if (current.lexeme == "fi") {
        gen_instr("LABEL", "nil");
        current = lexer_get();//idk if needed
    }
    else if (current.lexeme == "otherwise") {
        current = lexer_get();

        Statement();
        int addr = instr_address;
        //back_patch(instr_address);
        if (current.lexeme != "fi")
        {
            cout << "\nError: Missing fi in otherwise if() line: " << current.prev_Line;
            exiting();
        }
        else {
            gen_instr("LABEL", "nil");
            current = lexer_get(); //idk if needed
        }
    }
    else {
        cout << "\nError: Missing fi or otherwise in if() line: " << current.prev_Line;
        exiting();
    }
}

void SA::Put() {
    if (current.lexeme == "put")
    {
        //outFile << "\n<Put> ";
        if ((current = lexer_get()).lexeme == "(")
        {
            current = lexer_get();
            //gen_instr("STDOUT","nil"); //--here? nope needs to be at the end

            if (current.type != "identifier")
            {
                cout << "\nError: Missing identifer in put() in line: " << current.prev_Line;
                exiting();
            }
            else {
                //forgot to add this part !
                gen_instr("PUSHM", getAddress(current.lexeme));
                current = lexer_get(); // here gets the next one idk if right
            }
            if (current.lexeme != ")")
            {
                cout << "\nError: Missing ) in put() in line: " << current.prev_Line;
                exiting();
            }
            else {
                //goes here
                gen_instr("STDOUT", "nil"); //  this is the end
                current = lexer_get(); //need to fix
            }
            if (current.lexeme != ";")
            {
                cout << "\nError: missing ';' in put() in line: " << current.prev_Line;
                exiting();
            }
            else current = lexer_get(); // need to fix
        }
    }
}
void SA::Get() {
    if (current.lexeme == "get")
    {
        //outFile << "\n <Get>";
        current = lexer_get();
        if (current.lexeme == "(")
        {
            current = lexer_get();
            if (current.type == "identifier")
            {
                gen_instr("STDIN", "nil");
                gen_instr("POPM", getAddress(current.lexeme)); // shouldnt this be on line 401 so if error it wont print these out
                //outFile << "\n <identifier>";
                current = lexer_get();
                if (current.lexeme == ")")
                {
                    current = lexer_get();
                    if (current.lexeme != ";")
                    {
                        cout << "\nError missing ';' at the end of get() line: " << current.prev_Line;
                        exiting();
                    }
                    else current = lexer_get();
                }
                else {
                    cout << "\nError:missing closing ')' in get() line: " << current.prev_Line;
                    exiting();
                }
            }
            else {
                cout << "\nError: missing identifier in get() line " << current.prev_Line;
                exiting();
            }
        }
        else {
            cout << "\nError: missing '(' in get() line: " << current.prev_Line;
            exiting();
        }
    }
}
void SA::While() {
    if (current.lexeme == "while")
    {
        int addr = instr_address + 1;
        gen_instr("LABEL", "nil");

        //outFile << "\n<While>";
        current = lexer_get();

        if (current.lexeme == "(")
        {
            current = lexer_get();
            Condition();
            if (current.lexeme != ")")
            {
                cout << "Error: Missing ) in while() in line: " << current.prev_Line;
                exiting();
            }
            else {
                current = lexer_get();

                Statement();
                gen_instr("JUMP", addr);
                back_patch(instr_address);
                current = lexer_get(); //added this but removed on this on line 256! for replacement
            }
        }
        else {
            cout << "Error: Missing ( in while() in line: " << current.prev_Line;
            exiting();
        }
    }
}

void SA::Condition() {
    //outFile << "\n<Condition>";
    Expression();
    // changed this up a little to look like procedure c
    if (current.lexeme == "==" || current.lexeme == ">"
        || current.lexeme == "<") {
        Token temp = current;
        Token temp2;
        current = lexer_get();
        Expression();
        temp2 = current;
        current = temp;
        Relop(); // had to change the order so it can print out right
        current = temp2;
    }
    else {
        cout << "Error: missing RELOP"; exiting();
    }
    //before we just called Relop(); Expression()
}
void SA::Relop() {
    //outFile << "\n<Relop>";
    if (current.lexeme == "==") {
        //current = lexer_get();
        gen_instr("EQU", "nil");
        push_stack(instr_address);
        gen_instr("JUMPZ", "nil"); // number
    }
    else if (current.lexeme == ">") {
        //current = lexer_get();
        gen_instr("GRT", "nil");
        push_stack(instr_address);
        gen_instr("JUMPZ", "nil");
    }
    else if (current.lexeme == "<") {
        //current = lexer_get();
        gen_instr("LES", "nil");
        push_stack(instr_address);
        gen_instr("JUMPZ", "nil");
    }
}
void SA::Expression() {
    //outFile << "\n<Expression>";
    Term();
    Expression_Prime();
}
void SA::Expression_Prime() {
    if (current.lexeme == "+") {
        //outFile << "\n<Expression Prime> -> +";
        current = lexer_get();
        Term();
        gen_instr("ADD", "nil");
        Expression_Prime();
    }
    else if (current.lexeme == "-") {
        //outFile << "\n<Expression Prime> -> -";
        current = lexer_get();
        Term();
        gen_instr("SUB", "nil");
        Expression_Prime();
    }
    else;
}
void SA::Term() {
    //outFile << "\n<Term>";
    Factor();
    Term_Prime();
}
void SA::Term_Prime() {
    if (current.lexeme == "*") {
        //outFile << "\n<Term Prime> * ";
        current = lexer_get();
        Factor();
        gen_instr("MUL", "nil");
        Term_Prime();
    }
    else if (current.lexeme == "/") {
        //outFile << "\n<Term Prime> /";
        current = lexer_get();
        Factor();
        gen_instr("DIV", "nil");
        Term_Prime();
    }
    else;
}


void SA::Factor() {
    //outFile << "\n <Factor>";
    if (current.lexeme == "-") {
        current = lexer_get();

        Primary();
    }
    else {
        Primary();
    }
}
void SA::Primary() {
   // outFile << "\n <Primary> ";
    if (current.type == "identifier") {
       // outFile << "-> <identifier>";
        gen_instr("PUSHM", getAddress(current.lexeme));
        current = lexer_get();
    }
    else if (current.type == "integer")
    {
       // outFile << "-> <integer>";
        //need to convert current.lexeme to int..
        // hard coded this :/
        stringstream convert(current.lexeme);
        int temp = 0;
        convert >> temp;
        gen_instr("PUSHI", temp);
        current = lexer_get();
    }
    else if (current.lexeme == "(")
    {

        current = lexer_get();
        Expression();
        if (current.lexeme != ")")
        {
           // outFile << "Error: Primary Expression missing ')' in line: " << current.prev_Line;

            exiting();
        }
    }
    else if (current.lexeme == "true") {
       // outFile << "-> true";
        gen_instr("PUSHM", getAddress(current.lexeme));
        current = lexer_get();
    }
    else if (current.lexeme == "false")
    {
       // outFile << "-> false";
        gen_instr("PUSHM", getAddress(current.lexeme));
        current = lexer_get();

    }
    else;
}

void SA::Empty() {
    //outFile << "\n <Empty>";
}