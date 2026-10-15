#!/bin/sh
set -eu
# Keep the original entry point; analysis and exports now run together.
exec .venv/bin/python tools/ghidra.py analyze
