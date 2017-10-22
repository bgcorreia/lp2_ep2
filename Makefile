COMPILADOR=g++
REMOVER=rm -rf
SOURCES=BanheiroUnissex.cpp
OBJETOS=$(subst .cpp,.o,$(SOURCES))
FLAGS=-std=c++11 -lpthread
DIR_OBJ=OBJ
DIR_INCLUDE=include
BINARIO=AbreBanheiroUnissex

all: $(BINARIO)

AbreBanheiroUnissex: $(OBJETOS)
	$(COMPILADOR) $(DIR_OBJ)/BanheiroUnissex.o -o $(BINARIO) $(FLAGS)
	$(REMOVER) $(DIR_OBJ)

BanheiroUnissex.o: BanheiroUnissex.cpp $(DIR_INCLUDE)/Colors.h
	[ -d $(DIR_OBJ) ] || mkdir $(DIR_OBJ)
	$(COMPILADOR) -c BanheiroUnissex.cpp $(FLAGS) -o $(DIR_OBJ)/BanheiroUnissex.o

clean:
	$(REMOVER) $(DIR_OBJ)

distclean: clean
	$(REMOVER) $(BINARIO)
