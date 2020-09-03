PYTHON3 = /usr/bin/env python3
TWINE = /usr/bin/env twine

spemu: spemu.c
	$(CC) -Wall -Wpedantic -O3 -std=c11 spemu.c -o spemu

dist:
	$(PYTHON3) setup.py sdist bdist_wheel

upload:
	$(TWINE) upload dist/*.whl
	$(TWINE) upload dist/*.tar.gz

clean:
	-rm -rf ./dist
	-rm -rf ./build
	-rm -rf ./spemu
	-rm -rf ./*.egg-info
