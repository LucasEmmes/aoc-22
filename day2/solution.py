with open("input.txt") as f: print((wins := {"A Y", "B Z", "C X"},ties := {"A X", "B Y", "C Z"},to_win := {"A":"Y", "B":"Z", "C":"X"},to_tie := {"A":"X", "B":"Y", "C":"Z"},to_lose := {"A":"Z", "B":"X", "C":"Y"},values := {"X":1, "Y":2, "Z":3},matches := f.read().split("\n"),p1:=sum([6*(match in wins) + 3*(match in ties) + values[match[-1]] if match!="" else 0 for match in matches]),p2:=sum([6*(match[-1] == "Z")+values[to_win[match[0]]]*(match[-1] == "Z") + 3*(match[-1] == "Y")+values[to_tie[match[0]]]*(match[-1] == "Y") + (match[-1] == "X")*values[to_lose[match[0]]] if match!="" else 0 for match in matches]))[-2:])