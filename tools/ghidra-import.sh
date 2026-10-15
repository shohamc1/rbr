#!/bin/sh
set -eu
JAVA_HOME=${JAVA_HOME:-/opt/homebrew/opt/openjdk@21/libexec/openjdk.jdk/Contents/Home}
export JAVA_HOME
PATH="$JAVA_HOME/bin:$PATH"
export PATH
# No global analysis state is reused. Generated projects stay in build/.
headless=${GHIDRA_HEADLESS:-/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless}
mkdir -p build/analysis
reference=$(.venv/bin/python -c 'import sys; sys.path.insert(0, "tools"); from workflow import reference; print(reference()[1])')
exec "$headless" build/analysis rbr-nocd \
  -import "$reference" \
  -processor x86:LE:32:default -cspec windows -noanalysis
