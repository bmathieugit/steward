# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CXX=g++
CXXFLAGS+= -Wall -Werror -pedantic
CXXFLAGS+= -std=c++20 -O3
CXXFLAGS+= -save-temps -fconcepts-diagnostics-depth=5
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

test-string: test/test-string.cpp building
	${CXX} -o  building/test-string.app test/test-string.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-string.app

test-vector: test/test-vector.cpp building
	${CXX} -o  building/test-vector.app test/test-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-vector.app

test-variant: test/test-variant.cpp building
	${CXX} -o  building/test-variant.app test/test-variant.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-variant.app

test-io: test/test-io.cpp building
	${CXX} -o  building/test-io.app test/test-io.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-io.app

test: test-variant test-vector test-string test-io  

stat:
	wc -l building/*.s

