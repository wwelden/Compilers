#include <iomanip>
#include <stdlib.h>
#include <optional>
#include <iostream>

#include "compilerUtils.h"
#include "internalRep.h"
#include "scanner.h"

// assisted by copilot

Token nextToken(ifstream &input){
    skipUseless(input);
    char c = input.peek();
    switch (c){
        case 's': 
            input.get(); 
            return lexS1(input); //add other cases + comma, and =
        case 'l': 
            input.get(); 
            return lexS8(input);
        case 'r':
            input.get(); 
            return lexS13(input);
        case 'm':
            input.get(); 
            return lexS19(input);
        case 'a':
            input.get(); 
            return lexS22(input);
        case 'n':
            input.get(); 
            return lexS25(input);
        case 'o':
            input.get(); 
            return lexS28(input);
        case ',':
            input.get(); 
            return Token{COMMA, 0};
        case '=':
            input.get(); 
            if (input.get() == '>'){
                return Token{ARROW, 0};
            }
            else error("Falure after reading =");
        case EOF:
            return Token{INST, NOP};
        default : 
            if(isdigit(c)) //build token with read int const token 
                return Token{CONST, readInt(input)};
            else {
                string line;
                getline(input,line);
                cout << line << endl;
                error ("Falure in scanner.next: " + to_string(c));
            }
    }
};
void skipUseless(ifstream &input){
    skipWhitespace(input);
    char c = input.peek();
    while (c == '/') {
        ignoreComment(input);
        skipWhitespace(input);
        c = input.peek();
    }
};

string showOpCode(OpCode op) {
  switch(op) { 
    case LOAD:
      return "load";
      break;
    case LOADI:
      return "loadI";
      break;
    case STORE:
      return "store";
      break;
    case ADD:
      return "add";
      break;
    case SUB:
      return "sub";
      break;
    case MULT:
      return "mult";
      break;
    case LSHIFT:
      return "lshift";
      break;
    case RSHIFT:
      return "rshift";
      break;
    case OUTPUT:
      return "output";
      break;
    case NOP:
      return "nop";
      break;
    default:
      error("Improperly initialized OpCode"); 
      return "";
      break;
  }
};
string showTokenType(TokenType tt){
    switch(tt){
        case INST:
            return "inst";
            break;
        case REG:
            return "reg";
            break;
        case CONST:
            return "const";
            break;
        case COMMA:
            return "comma";
            break;
        case ARROW:
            return "arrow";
            break;
        default:
            error("Improperly initialized TokenType"); 
            return "";
            break;
    }
}
Token lexS1(ifstream &input){
    char c = input.get();
    switch (c){
        case 't': 
            if (input.get() == 'o'&& input.get() == 'r'&& input.get() == 'e')
                return Token{INST, STORE};
            else error("Falure after reading ST");
        case 'u':
            if (input.get() == 'b')
                return Token{INST, SUB};
            else error("Falure after reading SU");
            break;
        default : error("Falure in lexS1: " + to_string(c)); 
    }
};
Token lexS8(ifstream &input){
    char c = input.get();
    switch (c){
        case 'o': 
            if (input.get() == 'a'&& input.get() == 'd')
                if (input.get() == 'I') //might need to change this for load
                    return Token{INST, LOADI};
                else
                    return Token{INST, LOAD};
            else error("Falure after reading LO");
        case 's':
            if (input.get() == 'h'&& input.get() == 'i'&& input.get() == 'f'&& input.get() == 't')
                return Token{INST, LSHIFT};
            else error("Falure after reading LS");
            break;
        default : error("Falure in lexS8: " + to_string(c));
    }
};
Token lexS13(ifstream &input){
    char c = input.peek();
    switch (c){ //check if reg
        case 's': 
            input.get();
            if (input.get() == 'h' && input.get() == 'i'&& input.get() == 'f'&& input.get() == 't')
                return Token{INST, RSHIFT};
            else error("Falure after reading RS");
            break;
        default : 
            if(isdigit(c))
                return Token{REG, readInt(input)};
            else 
                 error("Falure in lexS13: " + to_string(c));
    }
};
Token lexS19(ifstream &input){ 
            if (input.get() == 'u' && input.get() == 'l' && input.get() == 't')
                return Token{INST, MULT};
            else error("Falure after reading MU"); 
};
Token lexS22(ifstream &input){
            if (input.get() == 'd'&& input.get() == 'd')
                return Token{INST, ADD};
            else error("Falure after reading AD");
};
Token lexS25(ifstream &input){
            if (input.get() == 'o' && input.get() == 'p')
                return Token{INST, NOP};
            else error("Falure after reading NO");
};
Token lexS28(ifstream &input){
            if (input.get() == 'u' && input.get() == 't'&& input.get() == 'p'&& input.get() == 'u'&& input.get() == 't')
                return Token{INST, OUTPUT};
            else error("Falure after reading OU");
};

void Token::prettyPrint(){
    cout << "<" << showTokenType(type) << ", ";
    if (type == INST)
        cout << showOpCode(static_cast<OpCode>(val)) << ">" << endl;
    else if (type == REG)
        cout << "r" << val << ">" << endl;
    else if (type == CONST)
        cout << val << ">" << endl;
    else if (type == COMMA)
        cout << "," << ">" << endl;
    else if (type == ARROW)
        cout << "=>" << ">" << endl;
    else 
        error("Improperly initialized TokenType"); 
};

