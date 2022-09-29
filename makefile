# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc -save-temps

all: clean building compile run

building:
	mkdir -p building

clean: 
	rm -rf building


building/ui-pencil.o: src/ui-pencil.cpp src/ui-pencil.hpp
	${CC} -o building/ui-pencil.o -c $< ${FLAGS}

building/ui-grid.o: src/ui-grid.cpp src/ui-grid.hpp
	${CC} -o building/ui-grid.o -c $< ${FLAGS}

building/ui-event.o: src/ui-event.cpp src/ui-event.hpp
	${CC} -o building/ui-event.o -c $< ${FLAGS}

building/ui-screen.o: src/ui-screen.cpp src/ui-screen.hpp
	${CC} -o building/ui-screen.o -c $< ${FLAGS}

building/main.o: src/main.cpp
	${CC} -o building/main.o -c $< ${FLAGS}

compile: building/main.o building/ui-event.o building/ui-grid.o building/ui-screen.o building/ui-pencil.o
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS}

run: compile
	./building/steward.app