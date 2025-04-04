#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <getopt.h>

#include <stdlib.h>
#include <getopt.h>
#include "internalRep.h"
#include "compilerUtils.h"
#include "scanner.h"
#include "parser.h"
#include "Allocator.h"
#include "scheduler.h"

using namespace std;

struct options {
  bool pretty_print = false;
  bool table_print = false;
  bool lex_print = false;
  bool allocate = false;
  bool schedule = true;  // Default to scheduling for this project
  int numRegs = 3;
  string filename = "";
};

const struct option longopts[] = {
  {"help", no_argument, NULL, 'h'},
  {"print", no_argument, NULL, 'p'},
  {"table", no_argument, NULL, 't'},
  {"lexer", no_argument, NULL, 'l'},
  {"alloc", no_argument, NULL, 'a'},
  {"sched", no_argument, NULL, 's'},
  {"debug", no_argument, NULL, 'd'},
  {0, 0, 0, 0}
};


void usage(char *exe_name) {
  cerr << "usage: " << exe_name << " [-hptalsd] [-k NUM] file" << endl;
  cerr << "\t-h, --help    print this menu" << endl;
  cerr << "\t-p, --print   pretty print the input, with added comments on relations." << endl;
  cerr << "\t-t, --table   print the input in tabular form." << endl;
  cerr << "\t-a, --alloc   run the allocator algorithm on the input block of code." << endl;
  cerr << "\t-s, --sched   compute the dependency graph and latency-weighted distance to root." << endl;
  cerr << "\t-k NUM        use NUM registers when allocating the code." << endl;
  cerr << "\t-l, --lexer   print a list of the tokens, one per line." << endl;
  cerr << "\t-d, --debug   print debug information at verbosity 1" << endl;
  cerr << "\t-v NUM        print debug information at verbosity NUM" << endl;
}

//Project 0
// Connect the executor to the components in internalRep. You will also have to modify other parts
// of the file for the last part!

void execute(char *exe_name, options &opts) {
  ifstream inputFile;
  if(!opts.pretty_print && !opts.table_print && !opts.lex_print && !opts.allocate && !opts.schedule) {
    opts.schedule = true;  // Default to scheduling if no other options are provided
  }

  inputFile.open(opts.filename);
  if(!inputFile) {
    cerr << "Invalid filename: " << opts.filename << "." << endl;
    usage(exe_name);
    exit(EXIT_FAILURE);
  }
  if(opts.lex_print) {
    // insert code to print the summary here.

    skipUseless(inputFile);
    while(inputFile.good() && !(inputFile.peek() == EOF)){
      Token t = nextToken(inputFile);
      skipUseless(inputFile);
      t.prettyPrint();
    }

    exit(EXIT_SUCCESS);
  }

  vector<Line> repres = parseLines(inputFile);
  if(opts.allocate) {
        // call functions I guess
        Allocator alloc(opts.numRegs);
        alloc.ComputeLastUse(repres);
        //tablePrint(repres);
        alloc.LocalRegisterAllocation(repres);
        repres = alloc.allocatedBlock;
        //tablePrint(repres);
  }

  if(opts.schedule) {
        // Create scheduler and build dependency graph
        Scheduler scheduler;
        scheduler.buildDependencyGraph(repres);
        scheduler.computeWeights();

        // If no other output options are specified, print the graph
        if(!opts.pretty_print && !opts.table_print) {
            scheduler.printGraph();
            exit(EXIT_SUCCESS);
        }
  }

  if(opts.pretty_print) {
    //insert code to print the representation here, using the pretty printer.
    prettyPrint(repres);
  }
  if(opts.table_print) {
    // insert code to print the represenstation in table form, using the table printer.

    tablePrint(repres);
  }
}

int main(int argc, char **argv) {
  char *exe_name = argv[0];
  int ch;

  options opts;

  debug("Entering options.");
  while((ch = getopt_long(argc, argv, "dhlpatsvk:", longopts, NULL)) != -1) {
    debug("Processing option " + string(1,(char)ch));
    switch(ch) {
      case 'h':
        usage(exe_name);
        exit(EXIT_SUCCESS);
        break;
      case 'a':
        opts.allocate = true;
        opts.schedule = false;  // Turn off scheduling by default when allocating
        break;
      case 's':
        opts.schedule = true;
        break;
      case 'p':
        opts.pretty_print = true;
        break;
      case 't':
        opts.table_print = true;
        break;
      case 'd':
        debugLevel = 1;
        break;
      case 'v':
        debugLevel = atoi(optarg);
        break;
      case 'k':
        opts.numRegs = atoi(optarg);
        opts.allocate = true;
        opts.schedule = false;  // Turn off scheduling by default when allocating
        break;
      case 'l':
        opts.lex_print = true;
        break;
      default:
        // unknown opt
        cerr << "Unknown opt: " << ch << endl;
        usage(exe_name);
        exit(EXIT_FAILURE);
        break;
    }
  }
  if(!opts.table_print && !opts.schedule) {
    opts.pretty_print=true;
  }
  debug("//Exiting options.");

  if(argc - optind != 1) {
    cerr << "Invalid command: filename required." << endl;
    usage(exe_name);
    exit(EXIT_FAILURE);
  } else {
    opts.filename = string (argv[optind]);
  }
  debug("//Found filename " + opts.filename);

  execute(exe_name, opts);
}
