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

building/ui-pencil.o: src/ui-pencil.cpp src/ui-pencil.hpp
	mkdir -p building
	${CC} -o building/ui-pencil.o -c $< ${FLAGS}

building/ui-grid.o: src/ui-grid.cpp src/ui-grid.hpp
	mkdir -p building
	${CC} -o building/ui-grid.o -c $< ${FLAGS}

building/ui-event.o: src/ui-event.cpp src/ui-event.hpp
	mkdir -p building
	${CC} -o building/ui-event.o -c $< ${FLAGS}

building/ui-screen.o: src/ui-screen.cpp src/ui-screen.hpp
	mkdir -p building
	${CC} -o building/ui-screen.o -c $< ${FLAGS}

building/ui-view.o: src/ui-view.cpp src/ui-view.hpp
	mkdir -p building
	${CC} -o building/ui-view.o -c $< ${FLAGS}

building/ui-application.o: src/ui-application.cpp src/ui-application.hpp
	mkdir -p building
	${CC} -o building/ui-application.o -c $< ${FLAGS}

building/ui-config.o: src/ui-config.cpp
	mkdir -p building
	${CC} -o building/ui-config.o -c $< ${FLAGS}

building/main.o: src/main.cpp
	mkdir -p building
	${CC} -o building/main.o -c $< ${FLAGS}

building/steward.app:  building/main.o building/ui-config.o building/ui-event.o 
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS}

compile: building/steward.app

run: compile
	./building/steward.app
