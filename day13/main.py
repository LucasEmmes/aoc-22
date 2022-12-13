def compare(a, b):
    correct = True
    for x, y in zip(a, b):
        if type(x) is int and type(y) is int:
            if x > y: return False
        elif type(x) is list and type(y) is list:
            if len(x) > len(y): return False
            if len(x) < len(y): x += [0]*(len(y)-len(x))
            correct = compare(x, y)
        else:
            if type(x) is int:
                x = [x]
            else:
                y = [y]
            correct = compare(x, y)
    return correct


def main():
    with open("input.txt") as f:
        data = f.read().split("\n")

    while "" in data:
        data.remove("")

    pairs = []

    for i in range(0, len(data), 2):
        a = eval(data[i])
        b = eval(data[i+1])
        pairs.append((a, b))

    ordered = 0
    pos = 1
    for a, b in pairs:
        if (compare(a, b)):
            print(pos)
            print(a)
            print(b)
            print()
            ordered += pos
        pos += 1

    print(ordered)

if __name__ == "__main__":
    main()