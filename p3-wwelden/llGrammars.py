from MBNF import *
from MakeTables import *
from ReadableOutput import *
from yaml import *

class Token:
    type = ""

def fixLL(ir):
    return ir

def make_tables(ir, worklist):
    print()
    if(worklist):
        sys.exit("Worklists not supported yet!")
    else:
        # TODO ?????
        # numberGrammar(ir)
        table = Tables()
        table = computeFirsts(ir, table)
        table = computeFollows(ir, table)
        table = computeNext(ir,table)
        # print("Make and return the appropriate tables")
        return(Tables())

def print_tables(tables):
    humanReadable(tables)
    # print("Print tables in human-readable format")

def print_yaml(tables, grammar):
    makeNextTable(tables, grammar)
    # print("Print tables in yaml format, or error if the involution of the next table fails")
    return yaml_print(tables, grammar)

def usage():
    print("usage: llgen.py grammar [-htrw]")
    print("positional arguments:")
    print("  grammar     grammar file")
    print("optional arguments:")
    print("  -h, --help      show this help message and exit")
    print("  -w, --worklist  use worklist algorithm")
    print("  -t, --table     print LL1 grammar as yaml")
    print("  -r, --revise    transform input grammar into LL1 grammar. (NOT IMPLEMENTED)")
