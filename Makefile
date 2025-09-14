default: build run

cmake:
	cmake -S . -B build

build:
	cmake --build build -- -j

run: run-s
debug: debug-s

run-w:
	@build/automaton -e wolfram --wf-code 110 --cell-width 6
run-s:
	@build/automaton -e sand --delay 50 --animation
run-l:
	@build/automaton -e life

debug-w:
	@G_MESSAGES_DEBUG=all $(MAKE) run-w
debug-s:
	@G_MESSAGES_DEBUG=all $(MAKE) run-s
debug-l:
	@G_MESSAGES_DEBUG=all $(MAKE) run-l

help:
	@build/automaton --help-all

clean:
	rm -rf build

format:
	pre-commit run --all-files

.PHONY: build run clean debug help
