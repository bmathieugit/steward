# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc
EXTRA=-save-temps -fconcepts-diagnostics-depth=10 # -fno-exceptions
#EXPER=-fanalyzer

all: clean compile run

clean:
	rm -rf building

building/main.o: src/main.cpp 
	mkdir -p building
	${CC} -o building/main.o -c $< ${FLAGS} ${EXTRA} ${EXPER}

building/steward.app: building/main.o
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS} ${EXTRA} ${EXPER}

building/slides.o: src/slides.cpp
	mkdir -p building
	${CC} -o building/slides.o -c $< ${FLAGS} ${EXTRA} ${EXPER}

building/slides.app: building/slides.o
	mkdir -p building
	${CC} -o building/slides.app $^ ${FLAGS} ${EXTRA} ${EXPER}

building/string-test.o: src/test/string-test.cpp
	mkdir -p building
	${CC} -o building/string-test.o -c $< ${FLAGS} ${EXTRA} ${EXPER}

building/string-test.app: building/string-test.o
	mkdir -p building
	${CC} -o building/string-test.app $^ ${FLAGS} ${EXTRA} ${EXPER}


compile: building/steward.app building/slides.app

test: building/string-test.app

run: compile
	./building/steward.app -k github.com
