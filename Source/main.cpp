//============================================================================
// Name        : main.cpp
// Author      : Kemal Berk Kocabagli
// Version     :
// Copyright   : BOGAZICI UNIVERSITY CMPE DEPARTMENT
// Description : Mylang_COMPILER Implementation, Ansi-style
//============================================================================
#include <iostream>
#include <fstream>
#include <set>


using namespace std;

string current;
ifstream input;
ofstream output;
bool reachedend = 0;
bool check =1;
int stNum=1;
string label = "LABL";
int counter = 1;


set <string> variables;

bool var (string s);
int stm ();
int isID (string s);
int isNum (string s);
void myprint();
void myread();
void separator();
int main(int argc, char *argv[]) {
    if (argc<3) {cout << "Please run the program with one input and one output file." << endl; return 0;}
    
    input.open(argv [1]);
    output.open(argv[2]);
    if (!input.good()) check = 0;
    else output << "code segment" << endl;
    input >> current;
    
    bool valid;
        valid = stm(); // stm - starting symbol
    
        //cout << "statement valid?" << valid << endl;
        if(!valid || !reachedend) {
            check =0;
            ofstream output (argv[2]);
            cout << "SYNTAX ERROR AROUND " << current << ". CANNOT COMPILE." << endl;
            return 0;
        }
        
    
    if (check) {
        output << "  int 20h \t ; exit to dos" << endl;
        separator();
        myread();
        separator();
        myprint();
        separator();

        while (!variables.empty()) {
            output << "v" << *variables.begin();
            output << "\tdw ?" << endl;

            variables.erase(variables.begin());
        }
        output << "code ends" << endl;
       
        cout << "COMPILED" << endl;
    }
    
    return 0;
}

// Checks if the string is a valid ID in the language Mylang
int isID (string s){

    if (s[0]>47 && s[0]<58) {
        //cout << "NOT ID" << endl;
        return 0;

    }

    if (s.compare("=")==0) {
        return 0;
    }
    else if (s.compare("print")==0) {
        return 0;
    }
    else if (s.compare("read")==0) {
        return 0;
    }
    else if (s.compare("if")==0) {
        return 0;
    }
    else if (s.compare("then")==0) {
        return 0;
    }
    else if (s.compare("while")==0) {
        return 0;
    }
    else if (s.compare("do")==0) {
        return 0;
    }
    else if (s.compare("begin")==0) {
        return 0;
    }
    else if (s.compare("end")==0) {
        return 0;
    }
    else if (s[s.size()-1] == ';') {
        return 0;
    }


    //cout << "ID" << endl;
        return 1;
}

// Checks if the string is a valid number in the language mylang
int isNum (string s) {
    for (int i=0 ; i< s.size(); i++) {
        if (!(s[i]>47 && s[i]<58)) {
            //cout << "IS NOT A NUMBER" << endl;
            return 0;
        }
    }
    //cout << "IS NUMBER" << endl;
        return 1;
}

//STM
int isNum();
int expr();
int opt_stms();
int stm () {
    
    if (isNum(current)) {
        //cout << current << endl;
        return 0;
    }
    if (isID(current)) {
        //cout << "current:" << current << endl;
        string c = current;
        //cout << "  push " << "v" << current << endl;
        output << "  push offset " << "v" << current << endl;
        //cout << "push-addr-var " << current << endl;
        variables.insert(current);
        input >> current;
        if (current.compare("=")==0) {
            //cout << "current:" << current << endl;
            input >> current;
            bool c1 = expr();
            output << "  pop ax" << endl;
            output << "  pop bx" << endl;
            output << "  mov [bx],ax" << endl;
            //cout << "  mov [v" << c << "],ax" << endl;
            //cout << "assign" << endl;
            return c1;
        }
        return 0;
        //cout << "found if"  << endl;
    }
    else if (current.compare("print")==0) {
        //cout << "current:" << current << endl;

        input >> current;
        bool b1 = expr();
        output << "  pop ax"<< endl;
        output << "  call myprint"<< endl;
        //cout << "print " << current << endl;
        return b1;
    }
    else if (current.compare("read")==0) {
        //cout << "current:" << current << endl;
        output << "  call myread"<< endl;
        input >> current;
        output << "  mov v"<< current << ",cx " << endl;
        if (isID(current)) {
            //cout << "current:" << current << endl;
            variables.insert(current);
            input >> current;
            if (!input.good()) reachedend = 1;
            return 1;
        }
        return 0;
    }
    else if (current.compare("if")==0) {
        //cout << "current:" << current << endl;
        input >> current;
        bool c1 = expr();
        int c = counter;
        output << "  pop ax"<< endl;
        output << "  cmp ax,0"<< endl;
        output << " if z jmp " << label << c << endl;
        //cout << "jump-if-false " << label << c << endl;
        counter++;
        if (current.compare("then")==0) {
            //cout << "current:" << current << endl;
            input >> current;
            if (!input.good()) reachedend = 1;
            bool c2 = stm();
            output << label << c  << ":" << endl;

            return (c1 && c2);
        }
        return 0;
    }
    else if (current.compare("while")==0) {
        //cout << current << endl;
        input >> current;
        int c = counter;
        output << label << c << ":" << endl;
        counter++;
        bool c1 = expr();
        output << "  pop ax"<< endl;
        output << "  cmp ax,0"<< endl;
        output << "  if z jmp " << label << counter << endl;
        //cout << "jump-if-false " << label << counter << endl;
        int co = counter;
        counter++;
        if (current.compare("do")==0) {
            //cout << current << endl;
            input >> current;
            if (!input.good()) reachedend = 1;
            bool c2 = stm();
            output << "  jmp " << label << c << endl;
            //cout << "jump " << label << c << endl;
            output << label << co << ":" << endl;
            return (c1 && c2);
        }
        return 0;
    }
    else if (current.compare("begin")==0) {
        //cout << current << endl;
        input >> current;
        bool c1 = opt_stms();
        if (current.compare("end")==0) {
            //cout << current << endl;
            input >> current;
            if (!input.good()) reachedend = 1;
            return c1;
        }
        return 0;
    }
    else if (current.compare(";")==0){
        input >> current;
        return 0;
    }
    
    
    
    return 1;
}



//OPT_STMS
int stmt_list();
int opt_stms() {
    return stmt_list();
}

//STMT_LIST
int stmt_list(){
    bool b = stm();
    if (current.compare(";")==0) {
        //cout << "current:" << current << endl;
        input >> current;
        return stmt_list();
    }
    return b;


}

//EXPR
int term();
int moreterms();
int expr(){
    bool c1= term();
    bool c2= moreterms();
    return (c1 || c2);
}

//MORETERMS
int expr();
int moreterms(){
    if (current.compare("+")==0 || current.compare("-")==0 ) {
        //cout << "current:" << current << endl;
        string operation = current;
        input >> current;
        bool c1= term();
        if (operation.compare("+")==0) {
            output << "  pop cx" << endl;
            output << "  pop ax" << endl;
            output << "  add ax,cx" << endl;
            output << "  push ax" << endl;
            //cout << "+" << endl;
        }
        else {
            output << "  pop cx" << endl;
            output << "  pop ax" << endl;
            output << "  sub ax,cx" << endl;
            output << "  push ax" << endl;
            //cout << "-" << endl;
        }
        bool c2= moreterms();
        return (c1 && c2);
    }
    
    return 1;
}

//TERM
int factor();
int morefactors();
int term(){
    bool c1= factor();
    bool c2= morefactors();
    return (c1 || c2);
}

// MOREFACTORS
int factor();
int morefactors() {
    if (current.compare("*")==0 || current.compare("/")==0 ||
            current.compare("%")==0) {
        //cout << "current:" << current << endl;
        string operation = current;
        input >> current;
        bool c1= factor();
        if (operation.compare("*")==0) {
            output << "  pop cx" << endl;
            output << "  pop ax" << endl;
            output << "  mul cx" << endl;
            output << "  push ax" << endl;
            //cout << "*" << endl;
        }
        else if (operation.compare("/")==0) {
            output << "  mov dx,0" << endl;
            output << "  pop cx" << endl;
            output << "  pop ax" << endl;
            output << "  div cx" << endl;
            output << "  push ax" << endl;


            //cout << "/" << endl;
        }
        else if (operation.compare("%")==0)  {
            output << "  mov dx, 0" << endl;
            output << "  pop cx" << endl;
            output << "  pop ax" << endl;
            output << "  div cx" << endl;
            output << "  push dx" << endl;
            //cout << "%" << endl;
        }
        bool c2= morefactors();
        return (c1 && c2);
    }
    if (current.compare(";")) return 0;
    return 1;
}

//FACTOR
int expr();
int factor() {
    if (current.compare("(")==0) {
        //cout << "current:" << current << endl;
        input >> current;
        bool c1 = expr();
        if (current.compare(")")==0){
            //cout << "current:" << current << endl;
            input >> current;
             if (!input.good()) reachedend = 1;
            return c1;
        }
        return 0;
    }
    else if (isID(current)|| isNum(current)) {
        //cout << "current:" << current << endl;
        if (isID(current)) {
            output << "  push " << "v" << current << " w" << endl;
            //cout << "push-val-var " << current << endl;
        }
        else {
            output << "  push " << current << endl;
            //cout << "push-num " << current << endl;
        }
        input >> current;
        if (!input.good()) reachedend = 1;
        return 1;
    }
    return 0;
}

//prints the Assembly code for printing the number in the AX register
void myprint () {
    output << "myprint:"<< endl;
    output << "  mov \t si, 10d" << endl;
    output << "  xor \t dx,dx" << endl;
    output << "  push \t 0Ah" << endl;
    output << "  mov \t cx, 1d" << endl;
    output << "nonzero:"<< endl;
    output << "  div \t si" << endl;
    output << "  add \t dx,48d" << endl;
    output << "  push \t dx" << endl;
    output << "  inc \t cx" << endl;
    output << "  xor \t dx,dx" << endl;
    output << "  cmp \t ax,0h" << endl;
    output << "  jne \t nonzero" << endl;
    output << "  writeloop:" << endl;
    output << "  pop \t dx" << endl;
    output << "  mov \t ah,02h" << endl;
    output << "  int \t 21h" << endl;
    output << "  dec \t cx" << endl;
    output << "  jnz \t writeloop" << endl;
    output << "  ret"<< endl;
}

//prints the Assembly code for reading a nonnegative integer
void myread() {
    output << "myread:"<< endl;
    output << "  MOV \t CX,0" << endl;
    output << "morechar:"<< endl;
    output << "  mov \t ah,01h" << endl;
    output << "  int \t 21H" << endl;
    output << "  mov \t dx,0" << endl;
    output << "  mov \t dl,al" << endl;
    output << "  mov \t ax,cx" << endl;
    output << "  cmp \t dl,0D" << endl;
    output << "  je \t myret" << endl;
    output << "  sub \t dx,48d" << endl;
    output << "  mov \t bp,dx" << endl;
    output << "  mov \t ax,cx" << endl;
    output << "  mov \t cx,10d" << endl;
    output << "  mul \t cx" << endl;
    output << "  add \t ax,bp" << endl;
    output << "  mov \t cx,ax" << endl;
    output << "  jmp \t morechar" << endl;
    output << "myret:"<< endl;
    output << "  ret" << endl;
}

//Assembly code separator
void separator() {
    for (int i=0; i<25;i++) output << ";";
    output << endl;
}