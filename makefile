# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CXX=g++
CXXFLAGS+= -Wall -Werror -pedantic
CXXFLAGS+= -std=c++20 -O3
CXXFLAGS+= -save-temps -fconcepts-diagnostics-depth=5
CXXFLAGS+= -ftemplate-backtrace-limit=0 
CXXINCS=-Isrc -Itest

all: clean test

clean:
	rm -rf building
	rm -rf dist

building:
	mkdir -p building

main: src/main.cpp building
	${CXX} -o  building/main.app src/main.cpp ${CXXFLAGS} ${CXXINCS}
	./building/main.app

test: test/test.cpp building
	${CXX} -o  building/test.app test/test.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test.app

cloc: 
	cloc src/

stat:
	wc -l building/*.s

