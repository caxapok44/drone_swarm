# Installation Steps

Below are minimal steps for Ubuntu 22.04.

## Ubuntu 22.04

Install toolchain:
```bash
sudo apt-get update
sudo apt-get install -y --no-install-recommends build-essential cmake ninja-build
```

Clone and build:
```bash
git clone https://github.com/caxapok44/drone_swarm.git
cd drone_swarm
cmake --preset ninja-release
cmake --build --preset build-release
```

Run example:
```bash
./build/release/main_app \
  --file data/20.txt \
  --steps 20 \
  --time_ms 1 \
  --start_x 10 --start_y 10 \
  --regrowth_rate 0.2 \
  --horizon 2
```
OR
```bash
./build/release/app/main_app  --config config.ini 
```
Notes:
- Input files must use header + grid format (first line N, then N lines of N integers). See README for the exact format.
- For very large files like `data/1000.txt`, prefer `--horizon 1` and adjust `--time_ms`.
