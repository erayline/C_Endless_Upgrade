#ifndef ITEMS_H
#define ITEMS_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "mystructs.h"

// stat increasing items

int CursedGlove(Man* man) { //1
	man->attack_speed += 10;
	return 1;
}

int Coockie(Man* man) { //2
	man->current_life += 10;
	man->max_life += 10;
	return 1;
}

int Syringe(Man* man) { //3
	man->move_speed += 10;
	return 1;
}

// on hit items

int RedTeeth(Man* man) { //4
	man->current_life += 10;
	return 1;
}

// pets




#endif
