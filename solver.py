import numpy as np
import numpy.ctypeslib as npct
from ctypes import *


def load_lib():
    lib = npct.load_library("build/solver.so", '.')
    lib.create_solver.argtypes = []
    lib.set_number_players.argtypes = [c_void_p, c_int]
    lib.get_prob.argtypes = [c_void_p, npct.ndpointer(dtype=np.uint8, ndim=1, flags='CONTIGUOUS'), c_int]
    lib.get_prob.restype = c_double
    lib.card2id.argtypes = [c_void_p, c_char_p]
    lib.card2id.restype = c_uint8

    return lib


LIB = load_lib()


class TexasSolver:
    def __init__(self) -> None:
        self.solver = LIB.create_solver()

    def card2id(self, cards):
        ids = np.empty(len(cards), dtype=np.uint8)
        for i, c in enumerate(cards):
            ids[i] = LIB.card2id(self.solver, c.encode('utf-8'))
        return ids

    def get_prob(self, num_players: int, cards: np.ndarray):
        n = len(cards)
        if n in [2, 5, 6, 7]:
            if cards.max() > 51:
                print("Invalid card id!")
            else:
                LIB.set_number_players(self.solver, num_players)
                return LIB.get_prob(self.solver, cards, n)


if __name__ == '__main__':
    m = TexasSolver()

    while (True):
        n_player = int(input("Input the number of players: "))
        cards = input("Input your cards one by one: ")
        cards = cards.strip().split(" ")
        ids = m.card2id(cards)
        prob = m.get_prob(n_player, ids)
        print("Your probability to win is:  {:.2f}%".format(100 * prob))
        print()

