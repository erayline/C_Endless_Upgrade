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

int cursed_glove_counter = 0;
int life_reminder_counter = 0;
int attack_speed_while_loop = 500;

void CursedGlove() { //1
	if (owned_items.CursedGlove_c != cursed_glove_counter) {
		man.attack_speed = man.attack_speed + 0.3f;
		attack_speed_while_loop = 500 / man.attack_speed;
		cursed_glove_counter++;
	}
}

int LifeReminder() { //2
	if (owned_items.LifeReminder_c != life_reminder_counter) {
		man.current_life += 10;
		man.max_life += 10;
		life_reminder_counter++;
	}
	return 1;
}

int Adrenaline(Man* man) { //3
	man->move_speed += 10;
	return 1;
}

// on hit items

int RedTeeth(Man* man) { //4
	man->current_life += 10;
	return 1;
}

// pets

int Bekmert(Man* man) {
	return 0;
}// rakiplerden can paketi düþürsün.Ü
 
//int Bilmem() { // hasar almasýn anlamasýn.
//
//}

#endif