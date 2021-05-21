from rppgraph import RPP_Graph
from parsing import *
import random
from rppgraph import *
from montecarlo import *


def random_tour(graph):
    required = set()
    cost = 0
    current_node = random.randint(1, graph.nb_vertices)
    tour = []

    while (len(required) < 2*graph.nb_required or tour[0][0] != tour[-1][1]) and len(tour) < 1000000:
        nb_adj = len(graph.adj_dic[current_node])
        next_node = graph.adj_dic[current_node][random.randint(0, nb_adj-1)]
        cost += next_node[1]
        if next_node[2]:
            required.add((next_node[0], current_node))
            required.add((current_node, next_node[0]))
        
        tour.append((current_node, next_node[0]))
        current_node = next_node[0]
    
    return tour, cost



# file path
s = "/Users/swarm/Documents/stage/URPP/UR132"

# parse graph
n,rq, l = read_graph(s)
g = RPP_Graph(n, rq, l)

# random tour
# tour, cost = random_tour(g)

# print(tour)
# print(len(tour), cost)

# general monte carlo for RPP

# state class for RPP
class RPP_GameState:

    def __init__(self, rpp_graph):
        self.graph = rpp_graph
        self.cost = 0
        self.required = set()
        self.current_node = random.randint(1, rpp_graph.nb_vertices)
        self.tour = []
        self.iterations = 0

# functions for RPP
initial_state = lambda : RPP_GameState(g) # state = graph, tour, cost, current_node, # set of required edges
possible_actions = lambda state : list(range(len(state.graph.adj_dic[state.current_node]))) # index of neighbors or current_node
def move(state, action):
    state.iterations += 1
    next_node = state.graph.adj_dic[state.current_node][action]
    state.cost += next_node[1]
    if next_node[2]:
        state.required.add((next_node[0], state.current_node))
        state.required.add((state.current_node, next_node[0]))
    state.tour.append((state.current_node, next_node[0]))
    state.current_node = next_node[0]
    return state
is_terminal = lambda state : len(state.required) == 2*state.graph.nb_required and state.tour[0][0] == state.tour[-1][1] or len(state.tour) >= 1000000


# TEST

rpp_game = Game(initial_state, possible_actions, move, is_terminal)
s = monte_carlo(rpp_game)
print(s.cost)






