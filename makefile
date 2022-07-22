# $@ : nom de la cible                                                          
# $< : nom de la première dépendance                                            
# $ˆ : liste des dépendances                                                    
# $? : liste des dépendances plus récentes que la cible                         
# $* : nom d’un fichier sans son suffixe  

CC=g++
FLAGS=-std=c++20 -O3 -Ideps/alt/src/ -Isrc -save-temps

all: clean deps compile

clean: 
	rm -rf building


deps: deps/alt
	cd deps/alt; git pull --rebase; cd ../..

deps/alt:
	git clone https://github.com/bmathieugit/alt deps/alt

compile: src/main.cpp
	mkdir -p building
	${CC} -o building/steward.app $< ${FLAGS}
