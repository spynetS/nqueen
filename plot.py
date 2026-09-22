#!/usr/bin/env python3

import sys
import matplotlib.pyplot as plt


def read_data(filename):
    lines = []
    current_line = []

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()

            if not line:
                continue

            x, y = map(float, line.split())

            # x appeared again / wrapped back around -> new line
            if current_line and x <= current_line[-1][0]:
                lines.append(current_line)
                current_line = []

            current_line.append((x, y))

    if current_line:
        lines.append(current_line)

    return lines


def plot_data(filename):
    lines = read_data(filename)

    for i, line in enumerate(lines):
        x = [point[0] for point in line]
        y = [point[1] for point in line]

        plt.plot(x, y, marker="o", label=f"Run {i + 1}")

    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <datafile>")
        sys.exit(1)

    plot_data(sys.argv[1])
