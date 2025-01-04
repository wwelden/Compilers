import json

def printDict(d):
    for k,vals in d.items():
        print(f"  {k if k else 'EOF'}: ", end="")
        if(vals):
            print({v if v else "EOF" for v in vals})
        else:
            print("{}")
    print("    " + '-'*30)

def printGrammar(g):
    _epsilon = 'ε'
    print("Productions:")
    for a,rhs,index in g.productions:
        print(f'\t{index}: {a} -> {" ".join(rhs) if rhs else _epsilon}')



def humanReadable(t):

    print("First Table: ")
    printDict(t.firstTable)

    print("Follow Table: ")
    printDict(t.followTable)

    print("Next Table: ")
    printDict(t.nextTable)

    return
