#  === Authors ===
#  Alfred Roos, Stefan Strand, Oliver Fiala, Leo Modin


import os
import shutil

def test(populations, mutations, file_name = ""):
    for pop in populations:
        #os.system("make clear")
        for mut in mutations:
            with open("data.txt", "a") as f:
                f.write(f"{mut}% mutation probability\n")
            os.system(f"./nqueen {pop} {mut}")
        os.system(f"python ./plot.py data.txt p{pop}_mut{mut}.png")
        
        if file_name != "":
            shutil.copy("./data.txt", file_name)

def bla():
    # test the tournament mutation
    test([2], [1,5,10,20,40,60,80,100], "tournament-samep-k2-mut-1-100.txt")
#    test([2], [60,80], "tournament-k2-mut-60-80.txt")
    # create the comparison
 #   os.system("./nqueen 2 80")
 #    os.system("make nqueen ROULETTE=-DROULETTE")
#     os.system("./nqueen 2 60")
#     shutil.copy("./data.txt", "tournament-vs-roulette.txt")
#     os.system("make clear")
    
#     # create the tests for roulette
#     test([2], [1,5,10,20,40,60,80,100], "roulette-mut-1-100.txt")
# -

if __name__ == "__main__":
    # #os.system("make nqueen ROULETTE=-DROULETTE")
    # test([2,8],[80], "tournament-population-test.txt")
    bla()
