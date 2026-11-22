PYTHON ?= .venv/bin/python
ISO ?= disc/Richard Burns Rally [149731fe].iso

.PHONY: setup extract build check diff report test
setup:
	python3 -m venv .venv
	.venv/bin/python -m pip install -r requirements.txt
	$(PYTHON) tools/msvc.py setup
extract:
	$(PYTHON) tools/extract.py "$(ISO)"
build:
	$(PYTHON) tools/build.py
check:
	$(PYTHON) tools/check.py
diff:
	$(PYTHON) tools/check.py $(ADDR)
report: check
	cd build && PATH="$(CURDIR)/tools/bin:$$PATH" ../.venv/bin/reccmp-reccmp --target RBR --html report.html --silent
test: build
	$(PYTHON) -m unittest discover -s tests
	cd build && PATH="$(CURDIR)/tools/bin:$$PATH" ../.venv/bin/reccmp-decomplint --target RBR
