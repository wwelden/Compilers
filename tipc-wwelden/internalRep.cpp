#include <iomanip>
#include <stdlib.h>
#include <optional>

#include "internalRep.h"
#include "compilerUtils.h"

// Framework
// These functions are provided for you, and do not need modified for project 0.
// They will also serve as a template for project 1.
string showRelType(RelType rel) {
  switch(rel) {
    case SUM:
      return "sum";
      break;
    case PRODUCT:
      return "product";
      break;
    case NEITHER:
      return "neither";
      break;
    default:
      error("Improperly initialized RelType"); 
      return "";
      break;
  }
}

string Relation::showLocation() {
  switch(location) {
    case 0:
      return "0th";
      break;
    case 1:
      return "1st";
      break;
    case 2:
      return "2nd";
      break;
    default:
      error("Improperly initialized location.");
      return "";
      break;
  }
}

void Relation::prettyPrint() {
  switch(cat) {
    case SUM:
      cout << "//The " << showLocation() << " location is the " << showRelType(cat) << " of the other two." << endl;
      break;
    case PRODUCT:
      cout << "//The " << showLocation() << " location is the " << showRelType(cat) << " of the other two." << endl;
      break;
    case NEITHER:
      cout << "//The " << showLocation() << " location is neither the sum nor the product of the other two." << endl;
      break;
  }
  return;
}

void Relation::tablePrint() {
  cout << left << setw(7) << showRelType(cat) << " | " << setw(4) << location;
  return;
}

//Project 0

//This function can only read a single digit. Reading a single character at a time, modify the
//function to read integers of arbitrary length. You can assume that integers are always ended by a
//blank ' ' or '\t'
int readInt(ifstream &input) {
  char c;
  int ret = 0;
  skipBlanks(input);
  input.get(c);
  while( '0' <= c && c <= '9' ) {
    ret = ret * 10 + c-'0';
    input.get(c);
  }
  input.unget(); 
  if(debugLevel > 1) {
    debug("Final integer from readInt: " + to_string(ret)); // use the -v 1 flag to test this
  }
  return ret;
}
//Copilot assisted function
Relation setRelation (int a, int b, int c) {
  if (a+b == c){
    return {SUM, 2};
  } else if (a*b == c){
    return {PRODUCT, 2};
  } else if (a == b+c){
    return {SUM, 0};
  } else if (a == b*c){
    return {PRODUCT, 0};
  } else if (a+b == c){
    return {SUM, 1};
  } else if (a*b == c){
    return {PRODUCT, 1};
  } else {
    return {NEITHER, 0};
  }
}
