#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------------------
# run_project.sh
#
# Builds the library & demo, runs tests, then runs the regression demo and
# logs all outputs to artifacts for final reporting.
# -----------------------------------------------------------------------------

# Configurable parameters
BUILD_DIR=build
DATA_FILE=data/machine.data
TRAIN_SPLIT=0.8
SEED=42

# Clean previous build/logs
rm -rf "$BUILD_DIR" logs
mkdir -p "$BUILD_DIR" logs

echo "=== [1/4] Configuring CMake (Release) ==="
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_COVERAGE=OFF

echo
echo "=== [2/4] Building targets ==="
cmake --build "$BUILD_DIR" --parallel

echo
echo "=== [3/4] Running unit tests ==="
cd "$BUILD_DIR"
ctest --output-on-failure 2>&1 | tee ../logs/tests.log
cd ..

echo
echo "=== [4/4] Running Regression Demo ==="
"$BUILD_DIR/RegressionDemo" \
    --data "$DATA_FILE" \
    --train-split "$TRAIN_SPLIT" \
    --seed "$SEED" \
    2>&1 | tee logs/regression.log

echo
echo "All done. Logs are in the 'logs/' folder:"
ls -l logs

exit 0
