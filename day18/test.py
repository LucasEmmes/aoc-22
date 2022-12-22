for i in range(0,11,2):
    x = (i-1) if i < 3 else 0
    y = (i-5) if i > 3 and i < 7 else 0
    z = (i-9) if i > 7 and i < 11 else 0
    print(x, y, z)