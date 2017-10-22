COMPILADOR=g++
REMOVER=rm -rf
SOURCES=BanheiroUnissex.cpp
FLAGS=-std=c++11 -lpthread
BINARIO=BanheiroUnissex

all: $(BINARIO)

BanheiroUnissex:
	$(COMPILADOR) $(BINARIO).cpp -o $(BINARIO) $(FLAGS)

clean:
	$(REMOVER) $(DIR_OBJ)

distclean: clean
	$(REMOVER) $(BINARIO)
