import io
import random
from pathlib import Path
import numpy as np


def write_numbers_to_file(file: io.TextIOBase, size: int, max_sample: int):
    numbers: list[int] = [random.randint(0, max_sample) for _ in range(size)]
    file.write(str(size))
    file.write("\n")

    for rdn_number in numbers:
        file.write(str(rdn_number))
        file.write(" ")

    file.write("\n")


def generate_samples(lower: int, higher: int, n_sizes: int):
    sizes: list[int] = [int(n) for n in np.linspace(lower, higher, n_sizes)]

    max_random_sample: int = 1.2 * higher

    folder_name: Path = Path("data")
    folder_name.mkdir(exist_ok=True)
    filepath = folder_name / "counting_samples.txt"

    with open(filepath, "w") as file:
        file.write(str(int(max_random_sample)))
        file.write("\n")
        for size in sizes:
            write_numbers_to_file(file, size, max_random_sample)


if __name__ == "__main__":
    lower: int = 10
    higher: int = 8000
    n_sizes: int = 120
    generate_samples(lower, higher, n_sizes)
