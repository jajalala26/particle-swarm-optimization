import argparse
import subprocess
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib.animation import FuncAnimation

# ── CLI ──────────────────────────────────────────────────────────────────────
parser = argparse.ArgumentParser(description="PSO visualizer")
parser.add_argument("--inertia",  choices=["static", "linear", "random"], default="linear")
parser.add_argument("--topology", choices=["gbest", "lbest"],             default="gbest")
args = parser.parse_args()

# ── Build & run C++ binary ───────────────────────────────────────────────────
subprocess.run(
    ["cmake", "--build", "build", "--target", "visualize"],
    check=True, stdout=subprocess.DEVNULL,
)
subprocess.run(
    ["./build/visualize", "--inertia", args.inertia, "--topology", args.topology],
    check=True,
)

# ── Read positions.csv ───────────────────────────────────────────────────────
frames: dict[int, list] = {}
done = None

with open("positions.csv", newline="") as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        if row[0] == "DONE":
            done = (float(row[1]), float(row[2]), float(row[3]))
        else:
            it = int(row[0])
            frames.setdefault(it, []).append(
                [float(row[2]), float(row[3]), float(row[4])]
            )

sorted_iters = sorted(frames.keys())

# ── Contour data ─────────────────────────────────────────────────────────────
grid = np.linspace(-10, 10, 300)
X, Y = np.meshgrid(grid, grid)
Z = X**2 + Y**2

# ── Aesthetic: dark monochrome / terminal feel ────────────────────────────────
BG      = "#080808"
FG_DIM  = "#303030"   # contour lines
FG_GRID = "#1a1a1a"   # grid
TICK_C  = "#3a3a3a"
LABEL_C = "#4a4a4a"
PART_C  = "#cccccc"   # particles
BEST_C  = "#ffffff"   # global best
TEXT_C  = "#606060"

plt.rcParams.update({
    "font.family":       "monospace",
    "font.size":         9,
    "text.color":        TEXT_C,
    "axes.facecolor":    BG,
    "figure.facecolor":  BG,
    "axes.edgecolor":    FG_GRID,
    "xtick.color":       TICK_C,
    "ytick.color":       TICK_C,
    "axes.labelcolor":   LABEL_C,
    "savefig.facecolor": BG,
})

fig, ax = plt.subplots(figsize=(7, 7))
fig.subplots_adjust(left=0.10, right=0.97, top=0.93, bottom=0.08)

# Sparse iso-lines — no fill
ax.contour(X, Y, Z, levels=14, colors=FG_DIM, linewidths=0.5, alpha=0.7)

ax.set_xlim(-10, 10)
ax.set_ylim(-10, 10)
ax.set_xlabel("x", labelpad=6)
ax.set_ylabel("y", labelpad=6)
ax.xaxis.set_major_locator(ticker.MultipleLocator(5))
ax.yaxis.set_major_locator(ticker.MultipleLocator(5))
ax.tick_params(length=3, width=0.5)

for sp in ("top", "right"):
    ax.spines[sp].set_visible(False)
for sp in ("left", "bottom"):
    ax.spines[sp].set_color(FG_GRID)
    ax.spines[sp].set_linewidth(0.5)

# Particle scatter
scatter  = ax.scatter([], [], c=PART_C, s=14, marker=".", linewidths=0, zorder=5, alpha=0.85)
best_dot = ax.scatter([], [], c=BEST_C, s=60,  marker="+", linewidths=1.4, zorder=6)

header = ax.text(
    0.5, 1.025,
    f"PSO  ·  inertia={args.inertia}  ·  topology={args.topology}",
    transform=ax.transAxes, ha="center", va="bottom",
    color=LABEL_C, fontsize=9,
)
info = ax.text(
    0.02, 0.97, "",
    transform=ax.transAxes, ha="left", va="top",
    color=TEXT_C, fontsize=8,
)


# ── Animation ────────────────────────────────────────────────────────────────
def update(frame_idx):
    it   = sorted_iters[frame_idx]
    data = np.array(frames[it])
    xs, ys, fits = data[:, 0], data[:, 1], data[:, 2]

    scatter.set_offsets(np.c_[xs, ys])

    bi = int(np.argmin(fits))
    best_dot.set_offsets([[xs[bi], ys[bi]]])

    info.set_text(f"iter {it:>4d}   best {fits[bi]:.3e}")
    return scatter, best_dot, info


# ~100 ms/frame ≈ 10 fps — feels like a slow drifting flock
anim = FuncAnimation(
    fig, update,
    frames=len(sorted_iters),
    interval=100,
    blit=True,
)

anim.save("pso_animation.gif", writer="pillow", fps=10)
print("Saved pso_animation.gif")

if done is not None:
    print(f"best: ({done[0]:.4e}, {done[1]:.4e})  fitness={done[2]:.4e}")

plt.show()
