def compare(a, b):
    if type(a) is float: return True
    if type(b) is float: return False
    # print(f"Comparing {a} vs {b}")
    for i in range(max(len(a), len(b))):
        # print(f"Comparing {a} vs {b}")
        if i == len(a): return True
        if i == len(b): return False
        x, y = a[i], b[i]

        if type(x) is int and type(y) is int:
            if y > x: return True
            if x > y: return False
        if type(x) is list and type(y) is list:
            c = compare(x, y)
            if c is not None: return c
        if type(x) is int and type(y) is list:
            x = [x]
            c = compare(x, y)
            if c is not None: return c
        if type(x) is list and type(y) is int:
            y = [y]
            c = compare(x, y)
            if c is not None: return c
    return None

def merge(a, b):
    result = []
    a.append(float('inf'))
    b.append(float('inf'))

    while len(a) > 1 or len(b) > 1:
        if type(a[0]) is float:
            result.append(b.pop(0))
        elif type(b[0]) is float:
            result.append(a.pop(0))
        elif compare(a[0], b[0]):
            result.append(a.pop(0))
        else:
            result.append(b.pop(0))
            
    return result

def merge_sort(arr):
    """
    Merge sort just like you remember it from your college classes
    """
    if len(arr) == 1:
        return arr

    a = merge_sort(arr[:len(arr)//2])
    b = merge_sort(arr[len(arr)//2:])

    return merge(a,b)

def main():
    with open("steinar.txt") as f:
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
        c = compare(a, b)
        if (c is None or c == True):
            # print("True")
            ordered += pos
        # else:
            # print("False")
        pos += 1
        # print()

    print(ordered)

    longlist = [[[2]], [[6]]]
    for i in data:
        longlist.append(eval(i))
    
    longlist = merge_sort(longlist)
    i_1 = longlist.index([[2]])+1
    i_2 = longlist.index([[6]])+1
    print(i_1*i_2)

if __name__ == "__main__":
    main()