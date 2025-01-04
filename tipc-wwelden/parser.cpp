#include <iomanip>
#include <stdlib.h>
#include <optional>
#include <iostream>

#include "compilerUtils.h"
#include "parser.h"
#include "Allocator.h"

//const int Infinity = std::numeric_limits<int>::max();

// assisted by copilot

Line Parse (ifstream &input){
  Token inst = nextToken(input);
  assertCondition(inst.type == INST, "Failure in parser.Parse: expected inst");
  switch(inst.val){
    case LOAD:
      return parseLoad(input);
    case LOADI:
      return parseLoadi(input);
    case STORE:
      return parseStore(input);
    case ADD:
      return parseArithop(OpCode (inst.val), input);
    case SUB:
      return parseArithop(OpCode (inst.val), input);
    case MULT:
      return parseArithop(OpCode (inst.val), input);
    case LSHIFT:
      return parseArithop(OpCode (inst.val), input);
    case RSHIFT:
      return parseArithop(OpCode (inst.val), input);
    case OUTPUT:
      return parseOutput(input);
    case NOP:
      return parseNop(input); 
      
    default:
      error("Failure in parser.Parse: invalid inst");
  }

}

Operand makeOpperand (bool isReg, int sr, int constant, int vr, int pr, int nu){
  Operand o;
  o.isReg = isReg;
  o.SR  = sr;
  o.constant = constant;
  o.VR = vr;
  o.PR = pr;
  o.NU = nu;
  return o;
}

void parseArrow(ifstream &input){
  Token t = nextToken(input);
  assertCondition(t.type == ARROW, "Failure in parser.parseArrow: expected arrow");
}
void parseComma(ifstream &input){
  Token t = nextToken(input);
  assertCondition(t.type == COMMA, "Failure in parser.parseComma: expected comma");
}


Operand parseConst(Token t){
  Operand o;
  if(t.type == CONST){
    o.isReg = false;
    o.constant = t.val;
    return o;
  }
  else{
    error("Failure in parser.parseConst: invalid Operand");
  }
}
Operand parseReg(Token t){
  Operand o;
  if(t.type == REG){
    o.isReg = true;
    o.SR = t.val;
    return o;
  }
  else{
    error("Failure in parser.parseReg: invalid Operand");
  }
}
Operand parseOpperand(ifstream &input){
  Token t = nextToken(input);
  Operand o;
  if(t.type == REG){
    o.isReg = true;
    o.SR = t.val;
    return parseReg(t);
  }
  else if(t.type == CONST){
    o.isReg = false;
    o.constant = t.val;
    return parseConst(t);
  }
  else{
    error("Failure in parser.parseOpperand: invalid Operand");
  }
}

Line parseArithop(OpCode opc, ifstream &input){
  Line l;
  l.opcode = opc;
  l.op1 = parseOpperand(input);
  parseComma(input);
  l.op2 = parseOpperand(input);
  parseArrow(input);
  l.dest = parseOpperand(input);
  return l;
}

Line parseLoad(ifstream &input){
  Line l;
  l.opcode = LOAD;
  l.op1 = parseOpperand(input);
  parseArrow(input);
  l.dest = parseOpperand(input);
  return l;
}
Line parseLoadi(ifstream &input){
  Line l;
  l.opcode = LOADI;
  l.op1 = parseOpperand(input);
  parseArrow(input);
  l.dest = parseOpperand(input);
  return l;
}
Line parseStore(ifstream &input){
  Line l;
  l.opcode = STORE;
  l.op1 = parseOpperand(input);
  parseArrow(input);
  l.op2 = parseOpperand(input);
  //l.dest = parseOpperand(input);
  return l;
}
Line parseOutput(ifstream &input){
  Line l;
  l.opcode = OUTPUT;
  l.op1 = parseOpperand(input);
  return l;
}
Line parseNop(ifstream &input){
  Line l;
  l.opcode = NOP;
  return l;
}

vector<Line> parseLines(ifstream &input){
  vector<Line> lines;
  int lineNum = 1;
  while(input.good() && !(input.peek() == EOF)){
    Line l = Parse(input);
    l.lineNum = lineNum;
    lines.push_back(l);
    lineNum++;
  }
  return lines;
}
void Line::prettyPrint(int code){
  int regWidth = 6;
  if (code == 1){
    if(opcode != NOP){
    cout << showOpCode(opcode) << "\t";
    if(opcode == OUTPUT || opcode == LOADI) {
      cout << op1.constant;
    }
    else if (op1.isReg){
      cout << "r" << op1.SR;
    }
    if (op2.isReg && opcode != STORE){
      cout << setw(regWidth) <<", " << "r" << op2.SR  << setw(regWidth* 0.5) << "\t";
    }else{
      cout << setw(regWidth*2)<< "\t";
    }
    if (dest.isReg){
      cout << "=> ";
      cout << "r" << dest.SR  << " ";
    }
    else if (opcode == STORE){
      cout << "=> ";
      cout << "r" << op2.SR  << " ";
    }
    cout << endl;
    }
  }else if (code == 2){
    if(opcode != NOP){
    cout << showOpCode(opcode) << "\t";
    if(opcode == OUTPUT || opcode == LOADI) {
      cout << op1.constant;
    }
    else if (op1.isReg){
      cout << "r" << op1.VR;
    }
    if (op2.isReg && opcode != STORE){
      cout << setw(regWidth) <<", " << "r" << op2.VR  << setw(regWidth* 0.5) << "\t";
    }else{
      cout << setw(regWidth*2)<< "\t";
    }
    if (dest.isReg){
      cout << "=> ";
      cout << "r" << dest.SR  << " ";
    } else if (opcode == STORE){
      cout << "=> ";
      cout << "r" << op2.SR  << " ";
    }
    cout << endl;
    }
  }else if (code == 3){
    if(opcode != NOP){
    cout << showOpCode(opcode) << "\t";
    if(opcode == OUTPUT || opcode == LOADI) {
      cout << op1.constant;
    }
    else if (op1.isReg){
      cout << "r" << op1.PR;
    }
    if (op2.isReg && opcode != STORE){
      cout << setw(regWidth) <<", " << "r" << op2.PR  << setw(regWidth* 0.5) << "\t";
    }else{
      cout << setw(regWidth*2)<< "\t";
    }
    if (dest.isReg){
      cout << "=> ";
      cout << "r" << dest.PR  << " ";
    } else if (opcode == STORE){
      cout << "=> ";
      cout << "r" << op2.PR  << " ";
    }  
   cout << endl;
    }
  }

}

void Operand::tablePrint(){ 
  int regWidth = 4;
  int reg = 0;
  if(isReg) {
        cout << "sr" << left << setw(regWidth) << SR << " |";
        cout << "vr" << left << setw(regWidth) << VR << " |";
        cout << "pr" << left << setw(regWidth) << PR << " |";
        if (NU == -100){
          cout << left << setw(regWidth) << "inf" << " || ";
        } else {
          cout << left << setw(regWidth) << NU << " || ";
        }
  } else {
    if (constant == -1){
      cout << left << setw(regWidth+2) << "-" << " | ";
    } else {
       cout << left << setw(regWidth+2) << constant << " | ";
    }
    cout << left << setw(regWidth+1) << "-" << " | ";
    cout << left << setw(regWidth+1) << "-" << " | ";
    cout << left << setw(regWidth-1) << "-" << " || ";
  }
 return;
}

// void Line::tablePrint(int code){
//   if(opcode != NOP){
//   cout << "//| ";
//   cout << lineNum << "\t";
//   cout << showOpCode(opcode) << "\t";
//   op1.tablePrint(code);
//   op2.tablePrint(code);
//   dest.tablePrint(code);
//   cout << endl;
//   }
// }
void Line::tablePrint(){
  if(opcode != NOP){
  cout << "//| ";
  cout << lineNum << "\t";
  cout << showOpCode(opcode) << "\t|";
  op1.tablePrint();
  op2.tablePrint();
  dest.tablePrint();
  cout << endl;
  }
}
//remove spilling comments and stuff

void prettyPrint(vector<Line> &lines){
  for (int i = 0; i < lines.size(); i++){
    lines[i].prettyPrint(3);
  }
}
void tablePrint(vector<Line> &lines){
  cout << "//Line\t" << "OpCode\t\t" << "Op1\t\t\t\t" << "Op2\t\t\t\t" << "Dest\t\t\t" << endl;
  cout << "//\t\tSR\tVR\tPR\tNU\tSR\tVR\tPR\tNU\tSR\tVR\tPR\tNU\t"<< endl;
  for (int i = 0; i < lines.size(); i++){
    lines[i].tablePrint();
  }
}

// make a functions that loops through a vector of lines and adds line nums to each line
