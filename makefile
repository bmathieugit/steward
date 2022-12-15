# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc 
EXTRA=-save-temps -fconcepts-diagnostics-depth=10
all: clean compile run


clean: 
	rm -rf building

# building/ui.o: src/ui.cpp src/ui.hpp
# 	mkdir -p building
# 	${CC} -o building/ui.o -c $< ${FLAGS}

# building/ui-component.o: src/ui-component.cpp src/ui-component.hpp
# 	mkdir -p building
# 	${CC} -o building/ui-component.o -c $< ${FLAGS}

# building/event.o: src/event.cpp src/event.hpp
# 	mkdir -p building
# 	${CC} -o building/event.o -c $< ${FLAGS}

building/main.o: src/main.cpp
	mkdir -p building
	${CC} -o building/main.o -c $< ${FLAGS} ${EXTRA}

building/steward.app: building/main.o 
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS} ${EXTRA}

compile: building/steward.app

run: compile
	./building/steward.app
