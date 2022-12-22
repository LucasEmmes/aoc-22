monkeys = [
    [52, 78, 79, 63, 51, 94],
    [77, 94, 70, 83, 53],
    [98, 50, 76],
    [92, 91, 61, 75, 99, 63, 84, 69],
    [51, 53, 83, 52],
    [76, 76],
    [75, 59, 93, 69, 76, 96, 65],
    [89]
    ]

monkey_business = [0, 0, 0, 0, 0, 0, 0, 0]

for i in range(10000):
    for item in monkeys[0]:
        item = item % 9699690
        item = item * 13
        #item = item // 3
        if (item%5==0):
            monkeys[1].append(item)
        else:
            monkeys[6].append(item)
        monkey_business[0]+=1
    monkeys[0] = []

    for item in monkeys[1]:
        item = item % 9699690
        item = item + 3
        #item = item // 3
        if (item%7==0):
            monkeys[5].append(item)
        else:
            monkeys[3].append(item)
        monkey_business[1]+=1
    monkeys[1] = []

    for item in monkeys[2]:
        item = item % 9699690
        item = item * item
        #item = item // 3
        if (item%13==0):
            monkeys[0].append(item)
        else:
            monkeys[6].append(item)
        monkey_business[2]+=1
    monkeys[2] = []

    for item in monkeys[3]:
        item = item % 9699690
        item = item + 5
        #item = item // 3
        if (item%11==0):
            monkeys[5].append(item)
        else:
            monkeys[7].append(item)
        monkey_business[3]+=1
    monkeys[3] = []

    for item in monkeys[4]:
        item = item % 9699690
        item = item + 7
        #item = item // 3
        if (item%3==0):
            monkeys[2].append(item)
        else:
            monkeys[0].append(item)
        monkey_business[4]+=1
    monkeys[4] = []

    for item in monkeys[5]:
        item = item % 9699690
        item = item + 4
        #item = item // 3
        if (item%2==0):
            monkeys[4].append(item)
        else:
            monkeys[7].append(item)
        monkey_business[5]+=1
    monkeys[5] = []

    for item in monkeys[6]:
        item = item % 9699690
        item = item * 19
        #item = item // 3
        if (item%17==0):
            monkeys[1].append(item)
        else:
            monkeys[3].append(item)
        monkey_business[6]+=1
    monkeys[6] = []

    for item in monkeys[7]:
        item = item % 9699690
        item = item + 2
        #item = item // 3
        if (item%19==0):
            monkeys[2].append(item)
        else:
            monkeys[4].append(item)
        monkey_business[7]+=1
    monkeys[7] = []
    if (i%100 == 0): print(i)

k = list(sorted(monkey_business, reverse=True))
print(k[0] * k[1])