import random

class Game:

    def __init__(self, initial, pa, m, it):
        self.initial_state = initial
        self.possible_actions = pa
        self.move = m
        self.is_terminal = it


def monte_carlo(Game):
    current_state = Game.initial_state()
    while not Game.is_terminal(current_state):
        action = random.choice(Game.possible_actions(current_state))
        current_state = Game.move(current_state, action)

    return current_state