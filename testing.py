import nqueen
import math
import matplotlib.pyplot as plt

class Config:
    def __init__(self, n: int, pop_len: int, parents: int,  mutation_rate:float):
        assert(pop_len >= parents)
        self.n = n
        self.pop_len = pop_len
        self.parents = parents
        self.mutation_rate = mutation_rate


def get_average(config: Config, rounds: int = 10, verbose: bool = False):
    """ Runs the algorithm rounds times and calculates the averate and returns it """
    sum_ = 0
    for i in range(rounds):
        generations, state = nqueen.nqueen(config.n, config.pop_len, config.parents, config.mutation_rate)
        sum_ += generations
        if verbose: print(i,"/",rounds,"->", generations)
    return sum_/rounds

if __name__ == "__main__":
    x = []
    y = []
    for i in range(4,9):
        x.append(i)
        
        average = get_average(Config(i, i*7, math.ceil(i*1.5), 0.05), 100, True)
        y.append(average)
        print("n =",i)
        print("average:", average)

    plt.plot(x, y)
    plt.xlabel("N")
    plt.ylabel("Average generations")
    plt.title("N-Queens Evolution")
    plt.show()
