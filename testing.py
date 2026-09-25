import os

if __name__ == "__main__":
    populations = [2,5]
    parents = [5,10]
    mutations = [1,4]
    for pop in populations:
        for par in parents:
            os.system(f"make clear")
            for mut in mutations:
                os.system(f"./nqueen {pop} {par} {mut}")
            os.system(f"python ./plot.py data.txt p{pop}_par{par}_mut{mut}.png")
