#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

extern int debugLevel;

void error(string msg);
void assertCondition(bool condition, string errmsg);
void debug(string str, int level=1);

int readInt(ifstream &input);

//assumes that the first '/' of a comment has already been read.
void ignoreComment(ifstream &input);
bool isSpace(char c); 
void skipBlanks(ifstream &input);
void skipWhitespace(ifstream &input);
void printTable(vector<int> &table);
void printStack(stack<int> &s);
void printSummary(vector<int> &summary, int empty=-1, string name = "");
bool isblankspace(char c);
