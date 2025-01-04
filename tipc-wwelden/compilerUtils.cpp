#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>

#include "compilerUtils.h"
using namespace std;

int debugLevel = 0; 

void error(string msg) {
  cerr << "Error: " << msg << endl;
  exit(EXIT_FAILURE);
}

void assertCondition(bool condition, string errmsg) {
  if (!condition) {
    cerr << "Assertion failed: " << errmsg << endl;
    exit(EXIT_FAILURE);
  }
}

void debug(string str, int level) {
  if (debugLevel >= level)
    cout << "//" << str << endl;
}


//assumes that the first '/' of a comment has not already been read.
void ignoreComment(ifstream &input) {
  char c;
  if(input.good()) {
    input.get(c);
    if(input.peek() == '/') {
      while(input.good() && c!= '\n') {
        input.get(c);
      }
    } else {
      error("invalid comment: ");
    }
  }
}


bool isblankspace(char c) {
  return (c == ' ') || (c == '\t');
}

//spaces and tabs only
void skipBlanks(ifstream &input) {
  char c;
  while(input.good() && isblankspace(input.peek())) {
    input.get(c);
  }
  return;
}

//includes newlines
void skipWhitespace(ifstream &input) {
  char c;
  while(input.good() && isspace(input.peek())) {
    input.get(c);
  }
  return;
}

void printStack(stack<int> &s) {
{
    if(s.empty())
    {
        cout << endl;
        return;
    }
    int x = s.top();
    s.pop();
    cout << x << ", ";
    printStack(s);
    s.push(x);
 }
}

void printSummary(vector<int> &summary, int empty, string name) {
  cout << "//" << name << "| ";
  int i = 0;
  for(auto elem : summary) {
    if(elem != empty)
      cout << i << " : " << elem << " | ";
    i++;
  }
  cout << " | " << endl;
}
