alias b := build

@_default:
	just -l

@build:
	make all

@clean:
	make clean

@rebuild:
	just clean
	just build

@watch:
	watch -c 'make CFLAGS="-fdiagnostics-color=always"'

