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
	${CC} -o building/steward.app $^ ${FLAGS}

stats:
	wc -l building/*.s

run: compile
#	./building/steward.app vault list vault-test-repo.csv	
	./building/steward.app vault save github.com/bmathieugit password vault-test-repo.csv
	./building/steward.app vault save github.com/bmathieugit2 password2 vault-test-repo.csv
	./building/steward.app vault save github.com/bmathieugit3 password3 vault-test-repo.csv
	./building/steward.app vault save github.com/bmathieugit password4 vault-test-repo.csv
	./building/steward.app vault remove github.com/bmathieugit vault-test-repo.csv
	./building/steward.app vault list vault-test-repo.csv	