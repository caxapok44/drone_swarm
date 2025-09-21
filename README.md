# drone_swarm


# Installation

See `installation-steps.md` for step-by-step setup on Ubuntu.

# Build and Run

## Build (CMake + Ninja)

```bash
# Configure
cmake --preset ninja-release

# Build
cmake --build --preset build-release
```

The executable is `main_app` in `build/release/`.

## Tests

```bash
cmake --preset ninja-release

cmake --build --preset build-release

# Run all tests
ctest --test-dir build/release -V

```

Notes:
- The first configure will download GoogleTest (via FetchContent).
- The smoke test `app_help_works` just runs `main_app --help`.

## Input format (header + grid)

Text file with:
1) First line: `N` (grid size)  
2) Next `N` lines: each contains `N` space-separated integers

Example (5x5):
```
5
1 2 3 4 5
2 3 4 5 6
3 4 8 4 3
2 3 4 5 6
1 2 3 4 5
```

## Run

```bash
./build/release/app/main_app \
  --file data/20.txt \
  --steps 500 \
  --time_ms 50 \
  --start_x 10 --start_y 10 \
  --regrowth_rate 0.2 \
  --horizon 2
```


Flags:
- `--file`: path to grid file
- `--steps`: total discrete steps `t`
- `--time_ms`: algorithm time budget `T` in milliseconds
- `--start_x --start_y`: starting coordinates `(x,y)`
- `--regrowth_rate`: fraction of base regained per step (0..1)
- `--horizon`: 1 or 2-step lookahead
- `--no-stay`: forbid staying in place

Program prints a JSON-like object with total score, steps returned (may be cut short by `T`), elapsed time, and the path.

## Design choices and assumptions

- Single-drone planner with per-cell linear regrowth toward base value.
- Receding-horizon greedy (1–2 steps) to balance immediate reward vs near-future gain.
- Includes 8-neighborhood moves; optional stay-in-place.


Example for `1000.txt` :
```bash
./build/release/app/main_app \
  --file data/1000.txt \
  --steps 500 \
  --time_ms 50 \
  --start_x 10 --start_y 10 \
  --regrowth_rate 0.2 \
  --horizon 2
```

Example for with config argument :
```bash
./build/release/app/main_app --config config.ini
```