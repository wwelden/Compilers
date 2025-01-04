#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include "compilerUtils.h"

using namespace std;

// Framework
// These functions are provided for you, and do not need modified for project 0.
// They will also serve as a template for project 1.
//
enum RelType {
  SUM, PRODUCT, NEITHER
};

string showRelType(RelType rel);

struct Relation {
  RelType cat = (RelType)-1;
  int location = -1;
  void prettyPrint();
  string showLocation();
  void tablePrint();
};

// Project 0
// These functions will need to be completed for project 0.
//Line readLine(ifstream &input, bool &valid);
//vector<Line> readFile(ifstream &input, vector<int> &summary);
