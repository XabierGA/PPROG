CC = gcc -pedantic
CFLAGS = -Wall -g
EXE = tutorial

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o defines.o resources.o object.o weapon.o enemies.o player.o rectangle.o maps.o interface.o strings.o
	$(CC) $(CFLAGS) -pthread -lm -o $@ $@.o defines.o resources.o object.o weapon.o enemies.o player.o rectangle.o maps.o interface.o strings.o
	
defines.o : defines.c defines.h
	$(CC) $(CFLAGS) -c $<

resources.o : resources.c resources.h
	$(CC) $(CFLAGS) -c $<

object.o : object.c object.h
	$(CC) $(CFLAGS) -c $<
	
weapon.o : weapon.c weapon.h
	$(CC) $(CFLAGS) -c $<
	
player.o : player.c player.h
	$(CC) $(CFLAGS) -c $<
	
rectangle.o : rectangle.c rectangle.h
	$(CC) $(CFLAGS) -c $<
	
maps.o : maps.c maps.h
	$(CC) $(CFLAGS) -c $<
	
interface.o : interface.c interface.h
	$(CC) $(CFLAGS) -c $<

enemies.o : enemies.c enemies.h
	$(CC) $(CFLAGS) -c $<
	
strings.o : strings.c strings.h
	$(CC) $(CFLAGS) -c $<