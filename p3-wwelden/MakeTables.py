from typing import Dict, List
from MBNF import Grammar

class Tables:
    # symbol -> [terminals / non-terminals that follow]
    firstTable: Dict[str, set[str]] = {}
    followTable: Dict[str, set[str]] = {}
    nextTable: List[set[str]] = {}
    next_tbl = {}


_epsilon = 'ε'
_eof = ""

def computeFirsts(g: Grammar, t: Tables):
    for terminal in g.terminals:
            t.firstTable[terminal] = set((terminal,))

    t.firstTable[_epsilon] = set((_epsilon,))
    t.firstTable[_eof] = set((_eof,))

    for nt in g.nonterminals:
        t.firstTable[nt] = set()

    changed = False
    while True:
        for [A, Bs, _] in g.productions:
            if len(Bs) != 0:
                rhs: set = t.firstTable[Bs[0]].difference(set((_epsilon,)))
                i = 1
                k = len(Bs) - 1
                if i < len(Bs):
                    while _epsilon in t.firstTable[Bs[i]] and i < k:
                        i += 1
                        rhs.update(t.firstTable[Bs[i]].difference(set((_epsilon,))))
                    if i == k and _epsilon in t.firstTable[Bs[k]]:
                        rhs.add(_epsilon)
                if len(rhs.difference(t.firstTable[A])) > 0:
                    changed = True
                t.firstTable[A].update(rhs)
        if changed:
            changed = False
        else:
            break
    return t

def computeFollows(g: Grammar, t: Tables):
    # t or t.followTable = Follow
    # nt = A
    # g.nonterminals[0] = S
    # ∈ NT = in g.nonterminals

    # foreach A ∈ NT do
    for nt in g.nonterminals:
        # Follow(A) <- 0
        t.followTable[nt] = set()
    # Follow(S) <- eof
    t.followTable[g.nonterminals[0]].add(_eof)
    # while Follow keeps changing do
    changed = False
    while True:
        # foreach Production A -> B1B2...Bk in P do
        for [A, Bs, _] in g.productions:
            k = len(Bs) - 1
            # trailer <- Follow(A)
            trailer: set[str] = t.followTable[A]
            # for i <- k to 1 by -1 do
            for i in range(k, 0, -1):
                # if Bi ∈ NT then
                if Bs[i] in g.nonterminals:
                    # Follow(Bi) U+ trailer
                    if len(trailer.difference(t.followTable[Bs[i]])) > 0:
                        t.followTable[Bs[i]].update(trailer)
                        changed = True
                # if ε ∈ First(Bi) then
                if _epsilon in t.firstTable[Bs[i]]:
                    # trailer U+ First(Bi) \ {ε}
                    trailer.update(t.firstTable[Bs[i]].difference(set((_epsilon,))))
                # else
                else:
                    # trailer <- First(Bi)
                    trailer = t.firstTable[Bs[i]]
        if changed:
            changed = False
        else:
            break
    # return Follow
    return t


def computeNext(g: Grammar,t: Tables):
    for [A, Bs, i] in g.productions:
        t.nextTable[i] = set()
        firstNotEpsilon = -1
        for [j, B] in enumerate(Bs):
            if _epsilon not in t.firstTable[B]:
                firstNotEpsilon = j
                break
        if firstNotEpsilon == -1:
            t.nextTable[i] = t.followTable[A]
            for B in Bs:
                t.nextTable[i].update(t.firstTable[B])
        else:
            for j in range(0, firstNotEpsilon + 1):
                t.nextTable[i].update(t.firstTable[Bs[j]])
    return t
