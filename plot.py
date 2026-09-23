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

            x, y1, y2 = map(float, line.split())

            # x appeared again / wrapped back around -> new line
            if current_line and x <= current_line[-1][0]:
                lines.append(current_line)
                current_line = []

            current_line.append((x, y1, y2))

    if current_line:
        lines.append(current_line)

    return lines


def plot_data(filename, output=None):
    lines = read_data(filename)

    for i, line in enumerate(lines):
        x = [point[0] for point in line]
        y1 = [point[1] for point in line]
        y2 = [point[2] for point in line]

        plt.plot(x, y1, marker="o", label=f"Run {i + 1} MEAN")
        plt.plot(x, y2, marker="o", label=f"Run {i + 1} MEDIAN")

    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    if output:
        plt.savefig(output, dpi=300)
        print(f"Saved plot to {output}")
    else:
        plt.show()


if __name__ == "__main__":
    if len(sys.argv) not in (2, 3):
        print(f"Usage: {sys.argv[0]} <datafile> [outputfile]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) == 3 else None

    plot_data(input_file, output_file)
