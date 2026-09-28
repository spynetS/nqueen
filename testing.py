import os

if __name__ == "__main__":
    populations = [2,8]
    mutations = [1,5]
    for mut in mutations:
        os.system(f"make clear")
        for pop in populations:
            os.system(f"./nqueen {pop} {mut}")
        os.system(f"python ./plot.py data.txt p{pop}_mut{mut}.png")
