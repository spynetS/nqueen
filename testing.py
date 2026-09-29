import os

if __name__ == "__main__":
    populations = [5]
    mutations = [1,5,20,50,75,90,100]
    for mut in mutations:
        os.system(f"make clear")
        for pop in populations:
            os.system(f"./nqueen {pop} {mut}")
        os.system(f"python ./plot.py data.txt p{pop}_mut{mut}.png")
