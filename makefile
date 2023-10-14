# $@ : nom de la cible
# $< : nom de la première dépendance
# $ˆ : liste des dépendances
# $? : liste des dépendances plus récentes que la cible
# $* : nom d’un fichier sans son suffixe

CXX=g++
CXXFLAGS+= -Wall -Werror -pedantic
CXXFLAGS+= -std=c++20 -O3 -save-temps
CXXFLAGS+= -fconcepts-diagnostics-depth=5
CXXINCS=-Isrc

all: clean test

clean:
	rm -rf building
	rm -rf dist
	rm -f test*

building:
	mkdir -p building

test-string: src/test/test-string.cpp building
	${CXX} -o  building/test-string.app src/test/test-string.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-string.app

test-vector: src/test/test-vector.cpp building
	${CXX} -o  building/test-vector.app src/test/test-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-vector.app

test-ostream: src/test/test-ostream.cpp building
	${CXX} -o  building/test-ostream.app src/test/test-ostream.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-ostream.app

test-istream: src/test/test-istream.cpp building
	${CXX} -o  building/test-istream.app src/test/test-istream.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-istream.app

test-io: src/test/test-io.cpp building
	${CXX} -o  building/test-io.app src/test/test-io.cpp ${CXXFLAGS} ${CXXINCS}
	./building/test-io.app

test: test-vector test-string test-ostream test-istream  #test-io  

stat:
	wc -l building/*.s

