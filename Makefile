BUILD_DIR := ${CURDIR}/build
AUTOMATON := ${BUILD_DIR}/automaton
BUILD_IMAGE := automaton-build
PROJECT_DIR := /project

.PHONY: default cmake build run clean debug help format

default: build run

cmake: cmake-gcc

cmake-clang:
	mkdir -p ${BUILD_DIR}
	CC=clang CXX=clang++ cmake -S . -B ${BUILD_DIR}

cmake-gcc:
	mkdir -p ${BUILD_DIR}
	CC=gcc CXX=g++ cmake -S . -B ${BUILD_DIR}

build: cmake
	cmake --build ${BUILD_DIR} -- -j

run: run-s
debug: debug-s

run-w: wolfram
wolfram:
	${AUTOMATON} -e wolfram --rule 110 --cell-width 6

run-s: sand
sand:
	${AUTOMATON} -e sand --pause 50 --animation

run-l: life
life:
	${AUTOMATON} -e life

run-a: ant
ant:
	${AUTOMATON} -e ant --pause 10 --cell-width 10 --animation

debug-w:
	G_MESSAGES_DEBUG=all $(MAKE) run-w
debug-s:
	G_MESSAGES_DEBUG=all $(MAKE) run-s
debug-l:
	G_MESSAGES_DEBUG=all $(MAKE) run-l
debug-a:
	G_MESSAGES_DEBUG=all $(MAKE) run-a

help:
	${AUTOMATON} --help-all

clean:
	rm -rf ${BUILD_DIR}

format:
	pre-commit run --all-files

# dependencies

build-deps:
	if which pacman >/dev/null ; then\
		pacman -S gtkmm3 cairomm glibmm ;\
	elif which apt-get >/dev/null ; then\
		apt-get install -y libgtkmm-3.0-dev libcairomm-1.0-dev libglibmm-2.4-dev ;\
	fi

runtime-deps:
	if which pacman >/dev/null ; then\
		$(MAKE) build-deps ;\
	elif which apt-get >/dev/null ; then\
		apt-get install -y libgtkmm-3.0-1t64 ;\
	fi

# docker

docker-image:
	docker build . -t ${BUILD_IMAGE}

docker-build:
	docker run \
		--rm \
		--user $(shell id -u):$(shell id -g) \
		--name ${BUILD_IMAGE} \
		--volume ${CURDIR}:${PROJECT_DIR} \
		--workdir ${PROJECT_DIR} \
		${BUILD_IMAGE} \
		make build

docker-run:
	docker run \
		--rm \
		--user $(shell id -u):$(shell id -g) \
		--name ${BUILD_IMAGE} \
		--volume ${CURDIR}:${PROJECT_DIR} \
		--workdir ${PROJECT_DIR} \
		-it \
		${BUILD_IMAGE} \
		bash

docker-clean:
	docker image rm ${BUILD_IMAGE} || :
