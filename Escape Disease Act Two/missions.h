#include "resources.h"
#include "object.h"
#include "weapon.h"
#include "rectangle.h"
#include "maps.h"
#include "player.h"
#include "interface.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>

int hangman(Interface *intrf, Strings **s, int lang);

int Not_Not(Interface *intrf, Strings **s, int lang);

int pong(Interface *intrf, Strings **strs, int lang);

int Increasing_Bar(Interface *intrf, Strings **strs, int lang);

int g_21(Interface *intrf);