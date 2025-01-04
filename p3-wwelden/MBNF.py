from enum import Enum
from queue import Empty
from typing import List, Tuple
import sys

class TokenCat(Enum):
    SEMICOLON = 1
    DERIVES = 2
    ALSODERIVES = 3
    EPSILON = 4
    SYMBOL = 5


def scanGrammar(contents: str):
    dict = {
        ";": TokenCat.SEMICOLON,
        ":": TokenCat.DERIVES,
        "|": TokenCat.ALSODERIVES,
        "Epsilon": TokenCat.EPSILON,
        "epsilon": TokenCat.EPSILON,
        "EPSILON": TokenCat.EPSILON
    }

    words = contents.split()
    tokens: List[Tuple[TokenCat, str]] = []
    symbols: set[str] = set()

    for word in words:
        if word in dict:
            cat = dict[word]
            tokens.append((cat,word));
        else:
            tokens.append((TokenCat.SYMBOL, word));

    return (tokens, list(symbols))

class Grammar:
    productions: List[Tuple[str, List[str], int]] = [] # The Production is the thing that derives
    nonterminals: List[str] = [] # The Non-terminal calls productions
    terminals: List[str] = [] # The terminals simply are

    def __init__(self) -> None:
        pass

    def printIt(self) -> None:
        print('Grammar: ')
        for production in self.productions:
            i = '{'
            i += production[0]
            i += ', ['
            for terminal in production[1]:
                i += ' '+terminal
            i += ' ], '
            i += str(production[2])
            i+= '} '
            print(i)
        print(self.nonterminals)
        print(self.terminals)

grammar: Grammar = Grammar()
def parseGrammar(tokensList, symbolList):
    ProductionList(tokensList)
    # Add terminals and nonterminals to the grammar elements here!
    for production in grammar.productions:
            if production[0] not in grammar.nonterminals:
                grammar.nonterminals.append(production[0])
    for production in grammar.productions:
            for terminal in production[1]:
                if terminal not in grammar.nonterminals and terminal not in grammar.terminals:
                    grammar.terminals.append(terminal)
    return grammar

def ProductionList(tokensList):
    prodList = []
    index = 1
    while (tokensList):
        head = ProductionSet(tokensList, index)
        while (tokensList[0][0] == TokenCat.ALSODERIVES):
            index += 1
            prodSet = ProductionSetPrime(tokensList, head, index)
            prodList.append(prodSet)
        if (tokensList[0][0] != TokenCat.SEMICOLON):
            print(tokensList[0][1], file=sys.stderr)
            Die('not semicolon')
        tokensList.remove(tokensList[0])
        index += 1

def ProductionSet(tokensList, index):
    token = tokensList[0]
    head = token
    if (token[0] != TokenCat.SYMBOL) :
        print(token[1], file=sys.stderr)
        Die('not symbol')
    tokensList.remove(tokensList[0])

    if (tokensList[0][0] != TokenCat.DERIVES):
        Die('not derives')
    tokensList.remove(tokensList[0])
    rightHandSide = RightHandSide(tokensList)
    Add((head[1], rightHandSide, index), grammar.productions)
    return head

def ProductionSetPrime(tokensList, head, index):
    if (tokensList[0][0] != TokenCat.ALSODERIVES):
        Die('not alsoderives')
    tokensList.remove(tokensList[0])
    rightHandSide = RightHandSide(tokensList)
    Add((head[1], rightHandSide, index), grammar.productions)

def RightHandSide(tokensList):
    rightHandList = []
    while (tokensList[0][0] == TokenCat.SYMBOL):
        rightHandList.append(tokensList[0][1])
        tokensList.remove(tokensList[0]);
    if (tokensList[0][0] == TokenCat.EPSILON):
        tokensList.remove(tokensList[0]);
    return rightHandList

def Add(term, list):
    alreadyHere = False
    for item in list:
        if (term == item):
            alreadyHere = True
    if (not alreadyHere):
        list.append(term)

def Die(place):
    print('Comitting Die', file=sys.stderr)
    print(place, file=sys.stderr)
    quit()
