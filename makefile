# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=clang++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc -save-temps

all:  clean building compile stats run

building:
	mkdir -p building

clean: 
	rm -rf building

building/ui-cursor.o: src/ui-cursor.cpp
	${CC} -o building/ui-cursor.o -c $< ${FLAGS}

building/ui-screen.o: src/ui-screen.cpp
	${CC} -o building/ui-screen.o -c $< ${FLAGS}

building/ui-widget.o: src/ui-widget.cpp
	${CC} -o building/ui-widget.o -c $< ${FLAGS}

building/main.o: src/main.cpp
	${CC} -o building/main.o -c $< ${FLAGS}

compile: building/ui-cursor.o building/ui-screen.o building/ui-widget.o building/main.o
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS}

stats:
	wc -l *.s

run: compile
	./building/steward.app