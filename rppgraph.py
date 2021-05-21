class RPP_Graph:

    def __init__(self, nvertices, nrequired, edges):
        self.nb_vertices = nvertices
        self.nb_required = nrequired
        self.make_graph(edges)

    def make_graph(self, edges):
        self.adj_dic = {}
        for e in edges:
            if e[0] in self.adj_dic:
                self.adj_dic[e[0]].append((e[1], e[2], e[3]))
            else:
                self.adj_dic[e[0]] = [(e[1], e[2], e[3])]
            if e[1] in self.adj_dic:
                self.adj_dic[e[1]].append((e[0], e[2], e[3]))
            else:
                self.adj_dic[e[1]] = [(e[0], e[2], e[3])]       
        
