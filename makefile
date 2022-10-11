# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc -save-temps

all: clean compile run


clean: 
	rm -rf building

building/ui.o: src/ui.cpp src/ui.hpp
	mkdir -p building
	${CC} -o building/ui.o -c $< ${FLAGS}

building/event.o: src/event.cpp src/event.hpp
	mkdir -p building
	${CC} -o building/event.o -c $< ${FLAGS}

building/main.o: src/main.cpp
	mkdir -p building
	${CC} -o building/main.o -c $< ${FLAGS}

building/steward.app: building/main.o building/event.o building/ui.o 
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS}

compile: building/steward.app

run: compile
	./building/steward.app
