OBJECTS = box.o cnd.o gto.o main.o
project: $(OBJECTS)
	g++ -Wall $(OBJECTS) -o project
box.o cnd.o gto.o: box.h objectclass.h
cnd.o main.o: cnd.h
gto.o main.o: gto.h