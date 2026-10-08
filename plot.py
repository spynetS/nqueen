#!/usr/bin/env python3
#  === Authors ===
#  Alfred Roos, Stefan Strand, Oliver Fiala, Leo Modin

import sys
import matplotlib.pyplot as plt


def read_data(filename):
    lines = []
    current_line = []
    current_label = None

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()

            # Blank line -> finish current run
            if not line:
                if current_line:
                    lines.append((current_label, current_line))
                    current_line = []
                    current_label = None
                continue

            # Header -> label for the next run
            try:
                x, mean, std, success_rate = map(float, line.split())
            except ValueError:
                # Finish previous run if necessary
                if current_line:
                    lines.append((current_label, current_line))
                    current_line = []

                current_label = line
                continue

            # x appeared again / wrapped back around -> new line
            if current_line and x <= current_line[-1][0]:
                lines.append((current_label, current_line))
                current_line = []

            current_line.append((x, mean, std, success_rate))

    if current_line:
        lines.append((current_label, current_line))

    return lines


def plot_data(filename, output=None):
    lines = read_data(filename)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

    # -------------------------------------------------
    # Plot 1: Mean + standard deviation
    # -------------------------------------------------

    for label, line in lines:
        x = [point[0] for point in line]
        mean = [point[1] for point in line]
        std = [point[2] for point in line]

        lower = [m - s for m, s in zip(mean, std)]
        upper = [m + s for m, s in zip(mean, std)]

        ax1.plot(x, mean, marker="o", label=label)
        ax1.fill_between(x, lower, upper, alpha=0.2)

    ax1.set_xlabel("Number of Queens")
    ax1.set_ylabel("Generations")
    ax1.set_title("Performance")
    ax1.grid(True)
    ax1.legend()

    # -------------------------------------------------
    # Plot 2: Success rate
    # -------------------------------------------------

    for label, line in lines:
        x = [point[0] for point in line]
        success_rate = [point[3] for point in line]

        ax2.plot(
            x,
            success_rate,
            marker="o",
            label=label
        )

    ax2.set_xlabel("Number of Queens")
    ax2.set_ylabel("Success Rate (%)")
    ax2.set_title("Success Rate")
    ax2.set_ylim(0, 100)
    ax2.grid(True)
    ax2.legend()

    fig.suptitle("N-Queens Genetic Algorithm")
    fig.tight_layout()

    if output:
        plt.savefig(output, dpi=300, bbox_inches="tight")
        print(f"Saved plot to {output}")
    else:
        plt.show()

    plt.close()


if __name__ == "__main__":
    if len(sys.argv) not in (2, 3):
        print(f"Usage: {sys.argv[0]} <datafile> [outputfile]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) == 3 else None

    plot_data(input_file, output_file)
