import subprocess
from generate_plots import generate_plots
from generate_samples import generate_samples


def run_c_code():
    subprocess.run(["gcc", "count.c", "libs/heap.c",
                    "libs/priority_queue.c", "libs/utils.c", "-o",
                    "count", "-lm"])
    subprocess.run(["./count"])


def main():
    lower: int = 10
    higher: int = 12000
    n_sizes: int = 200

    print(
        f"Generating {n_sizes} samples with values between {lower} and {higher}...")
    generate_samples(lower, higher, n_sizes)

    print("Counting the number of comparisons for each sample...")
    run_c_code()

    print("Generating comparison plots...")
    generate_plots()


if __name__ == "__main__":
    main()
