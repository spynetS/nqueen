#  === Authors ===
#  Alfred Roos, Stefan Strand, Oliver Fiala, Leo Modin


import os
import shutil

def test(selection, mutations, file_name = ""):
    #os.system("make clear")
    for mut in mutations:
        with open("data.txt", "a") as f:
            f.write(f"{mut}% mutation probability\n")
        os.system(f"./nqueen {2} {mut} {selection}")
    os.system(f"python ./plot.py data.txt {file_name}.png")
    
    if file_name != "":
        shutil.copy("./data.txt", file_name)

def bla():
    os.system("make clear")
    os.system("make nqueen")
    test(0, [1,5,10,20,40,60,80,100], "./results/t-kp0.5-mut-all.txt")
    test(1, [1,5,10,20,40,60,80,100], "./results/t-k2-mut-all.txt")
    test(2, [1,5,10,20,40,60,80,100], "./results/r-mut-all.txt")



if __name__ == "__main__":
    # #os.system("make nqueen ROULETTE=-DROULETTE")
    # test([2,8],[80], "tournament-population-test.txt")
    bla()
