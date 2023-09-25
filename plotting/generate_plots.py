import time
from pathlib import Path
import matplotlib.pyplot as plt
import matplotlib


def matploblib_config():
    """Configurar matplotlib."""

    plt.style.use("ggplot")

    matplotlib.rcParams["mathtext.fontset"] = "cm"
    # matplotlib.rcParams["font.family"] = "CMU Serif"

    axes = {
        "labelsize": 16,
        "titlesize": 12,
        "titleweight": "bold",
        "labelweight": "bold",
    }
    matplotlib.rc("axes", **axes)

    lines = {"linewidth": 1}
    matplotlib.rc("lines", **lines)

    legends = {"fontsize": 14}
    matplotlib.rc("legend", **legends)

    savefig = {"dpi": 100}
    matplotlib.rc("savefig", **savefig)

    matplotlib.rcParams["axes.prop_cycle"] = matplotlib.cycler(
        color=["004c6d", "7aaac6"],
    )

    matplotlib.rcParams["ytick.labelsize"] = 12
    matplotlib.rcParams["xtick.labelsize"] = 12
    matplotlib.rcParams["figure.titlesize"] = 18
    matplotlib.rcParams["figure.titleweight"] = 'bold'


def update_data(prev: int, data: dict, chunk: dict):
    data["samples"].append(chunk["samples"])
    data["heap"].append(chunk["heap"])
    data["queue"].append(chunk["queue"])
    data["sizes"].append(prev)
    chunk["heap"] = []
    chunk["samples"] = []
    chunk["queue"] = []


def read_data() -> dict:
    data = {
        "sizes": [],
        "samples": [],
        "heap": [],
        "queue": []
    }
    with open("data/processed.txt", "r") as file:
        chunk = {
            "samples": [],
            "heap": [],
            "queue": []
        }

        prev = -1
        for line in file:
            row = [int(value) for value in line.strip("\n").split(",")]

            if row[0] != prev and prev != -1:
                update_data(prev, data, chunk)

            chunk["samples"].append(row[1])
            chunk["heap"].append(row[2])
            chunk["queue"].append(row[3])

            prev = row[0]

        update_data(prev, data, chunk)

    return data

def generate_main_plot(sizes, queue, heap, description):
    fig = plt.figure(dpi=100, figsize=(10, 8))
    fig.suptitle("Contagem e Plotagem: Heap vs Queue")

    axs: plt.Axes = fig.subplot_mosaic(
        [['heap', 'queue'], ['both', 'both']]
    )

    axs["queue"].plot(sizes, queue, 'r')
    axs["queue"].set_title("Queue")
    axs["queue"].set_ylabel("Comparações")
    axs["queue"].set_xlabel("$n$")

    axs["heap"].plot(sizes, heap, 'b')
    axs["heap"].set_title("Heap")
    axs["heap"].set_ylabel("Comparações")
    axs["heap"].set_xlabel("n")

    axs["both"].set_title("Heap vs Queue")
    axs["both"].plot(sizes, queue, 'r', sizes, heap, 'b')
    axs["both"].set_ylabel("Comparações")
    axs["both"].set_xlabel("n")

    folder_name: Path = Path("plots")
    folder_name.mkdir(exist_ok=True)

    fig.tight_layout()
    fig.savefig(folder_name / f"{description}_{round(time.time())}.jpg")
    plt.show()

def generate_scatter_plot(data):
    fig = plt.figure(dpi=100, figsize=(10, 4.5))
    fig.suptitle("Contagem e Plotagem: Heap vs Queue (Scatter plot)")

    axs = fig.subplot_mosaic(
        [['heap', 'queue']]
    )

    n_samples: int = 4
    for size, queue, heap in zip(data["sizes"], data["queue"], data["heap"]):
        filt_queue = sorted(queue, reverse=True)[:n_samples]
        filt_heap = sorted(heap, reverse=True)[:n_samples]

        axs["queue"].scatter([size] * len(filt_queue), filt_queue)
        axs["heap"].scatter([size] * len(filt_heap), filt_heap)

    axs["heap"].set_title("Heap")
    axs["heap"].set_ylabel("Comparações")
    axs["heap"].set_xlabel("n")

    axs["queue"].set_title("Queue")
    axs["queue"].set_ylabel("Comparações")
    axs["queue"].set_xlabel("n")

    folder_name: Path = Path("plots")
    folder_name.mkdir(exist_ok=True)

    fig.tight_layout()
    fig.savefig(folder_name / f"sct_{round(time.time())}.jpg")
    plt.show()

def generate_plots():
    data: dict = read_data()
    queue: list[int] = [max(sample) for sample in data["queue"]]
    heap: list[int] = [max(sample) for sample in data["heap"]]

    matploblib_config()
    generate_main_plot(data["sizes"], queue, heap, "bigO")
    # generate_scatter_plot(data)


if __name__ == "__main__":
    generate_plots()
