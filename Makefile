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
analyze:
	$(PYTHON) tools/analyze.py
qualify:
	$(PYTHON) tools/qualify.py
test:
	$(PYTHON) -m unittest discover -s tests -v
	mkdir -p build/tests
	$(CXX) -std=c++11 tests/behavior.cpp -o build/tests/behavior
	build/tests/behavior
