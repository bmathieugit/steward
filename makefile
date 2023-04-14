# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc
EXTRA=-save-temps -fconcepts-diagnostics-depth=10 # -fno-exceptions
#EXPER=-fanalyzer

all: clean building/stewsh.app

clean:
	rm -rf building

building/main.o: src/main.cpp 
	mkdir -p building
	${CC} -o building/main.o -c $< ${FLAGS} ${EXTRA}

building/steward.app: building/main.o
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS} ${EXTRA}}

building/stewsh.o: src/stewsh.cpp
	mkdir -p building
	${CC} -o building/stewsh.o -c $< ${FLAGS} ${EXTRA}

building/stewsh.app: building/stewsh.o
	mkdir -p building
	${CC} -o building/stewsh.app $^ ${FLAGS} ${EXTRA}


building/string-test.o: src/test/string-test.cpp
	mkdir -p building
	${CC} -o building/string-test.o -c $< ${FLAGS} ${EXTRA} 

building/string-test.app: building/string-test.o
	mkdir -p building
	${CC} -o building/string-test.app $^ ${FLAGS} ${EXTRA}

test: building/string-test.app

stewsh: building/stewsh.app

