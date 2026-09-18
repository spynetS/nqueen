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
    
def crossover(parents: list) -> list:
    # here we combine 2 parents into one child
    # we take one half and fill in the rest
    # from the other parent
    n = len(parents[0])
    half = (n+1) // 2

    child = parents[0][:half]
    for v in parents[1]:
        if v not in child:
            child.append(v)

    mutation_rate = 0.05
    if random.random() < mutation_rate:
        i, j = random.sample(range(len(child)), 2)
        child[i], child[j] = child[j], child[i]

    return child

# ===== main program =====
def nqueen(n: int, pop_len: int):
    population = populate(n,pop_len)
    # for state in population:
    #     print_state(state)
    
    found = False
    # play it out over generations
    generations = 10000
    for i in range(generations):
        parents = selection(population, 2)
        population = []
        for j in range(pop_len):
            child = crossover(parents)
            population.append(child)
            if fitness(child) == 0:
                print("after",i, "generations")
                print(child)
                print_state(child)
                found = True
                break
        if found:
            break

nqueen(10,10)
