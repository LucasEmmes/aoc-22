with open("input.txt") as f:
    data = f.read().split("\n")

while "" in data:
    data.remove("")

nodes = []
connections = {}

for i, r in enumerate(data):
    k = r.split(" ")
    nodes.append(k[1])
    g = []
    for c in k[9:]:
        c = c[:2]
        g.append(c)
    connections[k[1]] = g

# for node in nodes:
#     print("\t\t{id:\"",node,"\"},")

for k, v in connections.items():
    for x in v:
        print("\t{from: \"",k,"\", to: \"",x,"\"},")