from os import terminal_size
import yaml
import sys
from MBNF import Grammar
from MakeTables import Tables
# from itertools

_epsilon = 'ε'
def makeNextTable(t: Tables, g: Grammar):
    for [A, Bs, i] in g.productions:
        bs1 = set(Bs)
        bs2 = bs1
        for b1 in bs1:
            bs2.remove(b1)
            for b2 in bs2:
                if len(t.firstTable[b1].intersection(t.firstTable[b2])) > 0:
                    print("error: input grammar is not LL(1) and yaml print was specified.")
                    sys.exit(2)
                if b1 == _epsilon:
                    if len(t.firstTable[b2].intersection(t.followTable[A])) > 0:
                        print("error: input grammar is not LL(1) and yaml print was specified.")
                        sys.exit(2)
    for nonterminal in g.nonterminals:
        t.next_tbl[nonterminal] = {}
    for [i, next] in enumerate(t.nextTable):
        for terminal in next:
            t.next_tbl[g.productions[i][0]][terminal] = i
    return 
    

def yaml_print(t: Tables, g: Grammar):
    print("terminals: [" + ", ".join(g.terminals) + "]")
    print("nonterminals: [" + ", ".join(g.nonterminals) + "]")
    print("eof-marker: <EOF>")
    print("error-marker: --")
    print("start-symbol: " + g.nonterminals[0])
    print("productions:")
    index = 0
    for [A, Bs, _] in g.productions:
        print(str(index) + ": {" + A + ": [" + ", ".join(Bs) + "]}")
    print("table:")
    ind = 0
    for [nt, row] in t.next_tbl:
        s = str(nt) + ": {"
        for [t, i] in row:
            s += t + ": " + str(i)
        print(str(nt) + ": {" + ", ".join([t + ": " + str(i) for [t, i] in row]) + "]}")
    return

    worklist()
    print("terminals: ")
    print("non-terminals: ")
    print("eof-marker: ")
    print("terminals: ")
    print("terminals: ")
    tables = make_tables(ir, worklist)

# terminals: [a, b]
# non-terminals: [B]
# eof-marker: <EOF>
# error-marker: --
# start-symbol: B
# productions:
#   0: {B: [a, B, b]}
#   1: {B: []}
# table:
#   B: {b: 1, a: 0, <EOF>: 1}

#
    print("First Table: ")
    printDict(t.firstTable)

    print("Follow Table: ")
    printDict(t.followTable)

    print("Next Table: ")
    printDict(t.nextTable)

    return
#