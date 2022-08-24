# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc -save-temps

all: clean compile stats run

clean: 
	rm -rf building

compile: src/main.cpp
	mkdir -p building
	${CC} -o building/steward.app $< ${FLAGS}

stats:
	wc -l building/*.s

run: compile
	./building/steward.app flag 