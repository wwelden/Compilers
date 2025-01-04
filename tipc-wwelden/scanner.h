#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include "compilerUtils.h"
#include "internalRep.h"

using namespace std;

// assisted by copilot


enum TokenType {
  INST, REG, CONST, COMMA, ARROW
};
enum OpCode {
  LOAD, LOADI, STORE, ADD, SUB, MULT, LSHIFT, RSHIFT, OUTPUT, NOP
};

struct Token {
  TokenType type;
  int val;
  void prettyPrint();
};


Token nextToken(ifstream &input);

string showOpCode(OpCode op);
string showTokenType(TokenType type);

Token lexS1(ifstream &input);
Token lexS8(ifstream &input);
Token lexS13(ifstream &input);
Token lexS19(ifstream &input);
Token lexS22(ifstream &input);
Token lexS25(ifstream &input);
Token lexS28(ifstream &input);



// Project 0
// These functions will need to be completed for project 0.
int readInt(ifstream &input);
void readFile(string& input, string filename);
void writeFile(string& output, string filename);
void skipUseless(ifstream &input);
//Token getToken(string& input, int& pos);
Token setToken(string& input, int& pos, int& lineNum, int& tokenNum);
Token makeToken();
vector<Token> getTokens(string& input, int& pos);
