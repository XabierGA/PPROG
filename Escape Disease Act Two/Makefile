CC = gcc
CFLAGS = -Wall -g
EXE = EDAT

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o defines.o resources.o object.o weapon.o enemies.o player.o rectangle.o maps.o interface.o missions.o strings.o game_easy.o game_medium.o game_hard.o game_legendary.o game_battlemode.o
	$(CC) $(CFLAGS) -pthread -lm -o $@ $@.o defines.o resources.o object.o weapon.o enemies.o player.o rectangle.o maps.o interface.o missions.o strings.o game_easy.o game_medium.o game_hard.o game_legendary.o game_battlemode.o -lm
	
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
	
missions.o : missions.c missions.h
	$(CC) $(CFLAGS) -c $<
	
strings.o : strings.c strings.h
	$(CC) $(CFLAGS) -c $<
	
game_easy.o : game_easy.c game_easy.h
	$(CC) $(CFLAGS) -c $<
	
game_medium.o : game_medium.c game_medium.h
	$(CC) $(CFLAGS) -c $<
	
game_hard.o : game_hard.c game_hard.h
	$(CC) $(CFLAGS) -c $<
	
game_legendary.o : game_legendary.c game_legendary.h
	$(CC) $(CFLAGS) -c $<
	
game_battlemode.o : game_battlemode.c game_battlemode.h
	$(CC) $(CFLAGS) -c $<