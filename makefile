# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=clang++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc

all: clean building compile run

building:
	mkdir -p building

clean: 
	rm -rf building

building/main.o: src/main.cpp
	${CC} -o building/main.o -c $< ${FLAGS}

compile: building/main.o
	mkdir -p building
	${CC} -o building/steward.app $^ ${FLAGS}

run: compile
	./building/steward.app