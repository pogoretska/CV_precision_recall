OBJECTS = box.o cnd.o gto.o main.o
prtable: $(OBJECTS)
	g++ -Wall -Wextra $(OBJECTS) -o prtable
box.o cnd.o gto.o: box.h objectclass.h
cnd.o main.o: cnd.h
gto.o main.o: gto.h