with open("input.txt") as f:
    L = f.read().split("\n")

def c2n(c):
    return ord(c) - (38 if ord(c) < 91 else 96)

def alphaR(P):
    if type(P) is list:
        for c in P[0]:
            if c in P[1] and c in P[2]:
                return c2n(c)
    else:
        T = P[:len(P)//2]
        D = P[len(P)//2:]
        for c in T:
            if c in D:
                return c2n(c)
    return 0

if __name__=="__main__":
    while "" in L: L.remove("")
    # P1
    dPdDsave = 0
    for l in L:
        dPdDsave += alphaR(l)
    print(dPdDsave)

    #P2
    dPdDsave = 0
    for i in range(0, len(L), 3):
        dPdDsave += alphaR(L[i:i+3])
    print(dPdDsave)
