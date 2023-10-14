# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CXX=g++
CXXFLAGS+= -Wall -Werror -pedantic
CXXFLAGS+= -std=c++20 -O3 -save-temps
CXXFLAGS+= -fconcepts-diagnostics-depth=5
CXXINCS=-Isrc -Itest

all: clean test

clean:
	rm -rf building
	rm -rf dist
	rm -f test*

building:
	mkdir -p building

test-string: test/test-string.cpp building
	${CXX} -o  building/test-string.app test/test-string.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-string.app

test-vector: test/test-vector.cpp building
	${CXX} -o  building/test-vector.app test/test-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-vector.app

test-ostream: test/test-ostream.cpp building
	${CXX} -o  building/test-ostream.app test/test-ostream.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-ostream.app

test-istream: test/test-istream.cpp building
	${CXX} -o  building/test-istream.app test/test-istream.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-istream.app

test-io: test/test-io.cpp building
	${CXX} -o  building/test-io.app test/test-io.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-io.app

test: test-vector test-string test-ostream test-istream  #test-io  

stat:
	wc -l building/*.s

