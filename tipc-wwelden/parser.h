#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include "compilerUtils.h"
#include "scanner.h"

using namespace std;

// assisted by copilot
// Framework
// These functions are provided for you, and do not need modified for project 0.
// They will also serve as a template for project 1.
//
struct Operand{
  bool isReg = false;
  int SR = -1;
  int constant = -1;
  void tablePrint();
  //int sr = 0;
  int VR = -1;
  int PR = -1;
  int NU = -1;
};

struct Line {
  OpCode opcode; 
  Operand op1;
  Operand op2;
  Operand dest;
  int lineNum;
  void prettyPrint(int code);
  void tablePrint();
};

Line Parse (ifstream &input);
Operand makeOpperand (bool isReg, int sr, int constant, int vr = -1, int pr = -1, int nu = -1);
Operand parseOpperand (ifstream &input, bool isReg);
void parseArrow(ifstream &input);
void parseComma(ifstream &input);
Line parseArithop(OpCode op, ifstream &input);
Operand parseReg(ifstream &input);
Operand parseConst(ifstream &input);
Line parseLoad(ifstream &input);
Line parseLoadi(ifstream &input);
Line parseStore(ifstream &input);
Line parseOutput(ifstream &input);
Line parseNop(ifstream &input);
vector<Line> parseLines(ifstream &input);
void tablePrint(vector<Line> &lines);
void prettyPrint(vector<Line> &lines);

// Project 0
// These functions will need to be completed for project 0

