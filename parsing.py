def read_graph(path):
    f = open(path, "r")
    s = f.read().split("\n")
    n = int(s[2].split()[2])
    nrequired = int(s[3].split()[2])
    nnotrequired = int(s[4].split()[2])
    nedges = nrequired + nnotrequired
    l = []
    for i in range(6, 7 + nedges):
        if i != 6+nrequired:
            e = s[i].split()
            a, b, c = int(e[1][:-1]), int(e[2][:-1]), int(e[4])
            l.append((a, b, c, i < 6+nrequired))
    
    return n, nrequired, l




