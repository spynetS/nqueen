import random

def print_state(state):
    n = len(state)
    print("Fitness:", fitness(state))
    #print(state)
    for i in range(0,n):
        y = n-1 - i
        print(y,end="")
        for x in range(n):
            if state[x] == y:
                print('👸', end="")
                continue
            if x+y % 2 == 0 or (y+x) % 2 == 0:
                print('⬛', end="")
            else:
                print("⬜", end="")
                
        print("")
    print(" ",end="")
    for x in range(n):
        print(x, end=" ")
    print("")


def random_state(n: int):
    state = list(range(n))
    random.shuffle(state)
    return state

def populate(n: int, pop_len: int):
    population = []
    for i in range(pop_len):
        population.append(random_state(n))

    return population

def fitness(state):
    # FIXME BUG This only checks the diagonal over us not under
    # Here we sum the number of queens on the same diagonal
    n = len(state)
    diag_sum = 0
    for i in range(0,n):
        col = state[i]
        # here we check all the squares
        # diagonaly to our cell
        for j in range(i+1,n):
            # if the row above us 
            if abs(state[j] - state[i]) == abs(j - i):
                diag_sum += 1
    return diag_sum

def selection(population: list, amnt: int) -> list:
    population.sort(key=fitness)
    return population[:amnt]
    
def crossover(parents: list, mutation_rate: int = 0.05) -> list:
    # here we combine 2 parents into one child
    # we take one half and fill in the rest
    # from the other parent
    n = len(parents[0])
    half = (n+1) // 2

    child = parents[0][:half]
    for v in parents[1]:
        if v not in child:
            child.append(v)

    if random.random() < mutation_rate:
        i, j = random.sample(range(len(child)), 2)
        child[i], child[j] = child[j], child[i]

    return child

# ===== main program =====
def nqueen(n: int,
           pop_len: int,
           amnt_parents: int = 2,
           mutation_rate: int = 0.05) -> int:
    # solutions only exists for n >= 4
    assert(n >= 4)
    # to be able to select amnt_parents, population needs to be bigger
    assert(pop_len >= amnt_parents)
    population = populate(n,pop_len)
    found = False
    # play it out over generations
    generations = 10000
    for i in range(generations):
        parents = selection(population, amnt_parents)
        population = []
        for j in range(pop_len):
            child = crossover(parents, mutation_rate)
            population.append(child)
            if fitness(child) == 0:
                found = True
                break
        if found:
            return (i, child)
    return (-1, {})

if __name__ == "__main__":
    nqueen(8,10)

