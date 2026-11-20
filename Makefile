PYTHON ?= .venv/bin/python
ISO ?= disc/Richard Burns Rally [149731fe].iso
FUNCTION ?= sub_00401000
.PHONY: setup extract verify-inputs build check-function check-overlay check-integrated progress analyze qualify test
setup:
	python3 -m venv .venv
	.venv/bin/python -m pip install -r requirements.txt
extract:
	$(PYTHON) tools/extract.py "$(ISO)"
verify-inputs:
	$(PYTHON) tools/workflow.py verify-inputs
build:
	$(PYTHON) tools/workflow.py build
check-function:
	$(PYTHON) tools/workflow.py check-function $(FUNCTION)
check-overlay:
	$(PYTHON) tools/workflow.py check-overlay
check-integrated:
	$(PYTHON) tools/workflow.py check-integrated
progress:
	$(PYTHON) tools/workflow.py progress
	$(PYTHON) tools/census.py progress
analyze:
	$(PYTHON) tools/analyze.py
qualify:
	$(PYTHON) tools/qualify.py
test:
	$(PYTHON) -m unittest discover -s tests -v
	mkdir -p build/tests
	$(CXX) -std=c++11 tests/behavior.cpp -o build/tests/behavior
	build/tests/behavior
	$(CXX) -std=c++11 tests/behavior_large.cpp -o build/tests/behavior_large
	build/tests/behavior_large
	$(CXX) -std=c++11 tests/behavior_packets.cpp -o build/tests/behavior_packets
	build/tests/behavior_packets

.PHONY: ghidra-analyze ghidra-function setup-msvc qualify-msvc
ghidra-analyze:
	$(PYTHON) tools/ghidra.py analyze
ghidra-function:
	$(PYTHON) tools/ghidra.py function $(FUNCTION)
setup-msvc:
	$(PYTHON) tools/msvc.py setup
qualify-msvc:
	$(PYTHON) tools/qualify_msvc.py

.PHONY: check-msvc-link
check-msvc-link:
	$(PYTHON) tools/msvc.py link-smoke

.PHONY: census-inventory census-analyze census-refine census-audit census-decompile census-progress census
census-inventory:
	$(PYTHON) tools/census.py inventory
census-analyze:
	$(PYTHON) tools/census.py analyze
census-refine:
	$(PYTHON) tools/census.py refine
census-audit:
	$(PYTHON) tools/census.py audit
census:
	$(PYTHON) tools/census.py report

census-decompile:
	$(PYTHON) tools/census.py decompile

census-progress:
	$(PYTHON) tools/census.py progress

.PHONY: doctor candidates new-function lookup worktree
LIMIT ?= 25
MAX_SIZE ?= 128
doctor:
	$(PYTHON) tools/harness.py doctor
candidates:
	$(PYTHON) tools/harness.py candidates --limit $(LIMIT) --max-size $(MAX_SIZE) $(if $(TIER),--tier $(TIER))
new-function:
	$(PYTHON) tools/harness.py new-function $(FUNCTION) $(if $(SIZE),--size $(SIZE)) $(if $(SOURCE),--source $(SOURCE))
lookup:
	$(PYTHON) tools/harness.py lookup $(ADDR)
worktree:
	$(PYTHON) tools/harness.py worktree $(FUNCTION)
