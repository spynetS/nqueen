import random


def random_state(n: int):
    state = []
    for i in range(n):
        state.append(random.randint(0,n))
    return state



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

def populate(n: int):
    population = []
    for i in range(n):
        population.append(random_state(n))

    return population

def sum_diag(state):
    n = len(state)
    diag_sum = 0
    for i in range(0,n):
        col = state[i]
        # here we check all the squares
        # diagonaly to our cell
        for j in range(i+1,n-1):
            # if the row above us 
            if abs(state[j] - state[i]) == abs(j - i):
                diag_sum += 1
                print("fail at index", (i, j),"val", (state[i], state[j]), "becase", state[j], "==", state[i]+j)
    return diag_sum

def fitness(state):
    # Here we sum the number of queens on the same rows
    # if the sum is 0 we have a solution
    fail_at = []
    seen = set()
    row_sum = 0
    for i in range(len(state)):
        col = state[i]
        if col in seen:
            row_sum += 1
            print("fail at", (i, col))
        else: seen.add(col)
    return row_sum+sum_diag(state)

#  TODO remove me (unnacecery)
def test_fitness():
    state = [0,6,3,5,7,1,4,2]
    print_state(state)
    fit = fitness(state)
    if fit == 0: print("pass")
    else: print("fail")


# ===== main program =====
population = populate(8)
for state in population:
    print_state(state)

    

#test_fitness()
