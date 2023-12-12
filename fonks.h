#ifndef FONKS_H
#define FONKS_H
#include "mystructs.h"


int icindemi(int interval_left, int interval_right, int nokta_orta) {
	//if it is in it is one
	if ((nokta_orta > interval_left) && (nokta_orta < interval_right)) {
		return 1;
	}
	else {
		return 0;
	}
}


//int openChest(Man* man, Chest* chestler, Item* itemler) {
//
//
//
//
//}

int collision_man_enemy(Man* man, Enemy* enemyler, GUI_State* gui_state) {
	for (int n = 0; n < enemy_count; n++) {
		if ((icindemi(man->x, man->x + man->size, enemyler[n].x) && icindemi(man->y, man->y + man->size, enemyler[n].y)) || (icindemi(man->x, man->x + man->size, enemyler[n].x + enemyler[n].width) && icindemi(man->y, man->y + man->size, enemyler[n].y)) || (icindemi(man->x, man->x + man->size, enemyler[n].x) && icindemi(man->y, man->y + man->size, enemyler[n].y + enemyler[n].height)) || (icindemi(man->x, man->x + man->size, enemyler[n].x + enemyler[n].width) && (icindemi(man->y, man->y + man->size, enemyler[n].y + enemyler[n].width)))) {
			return 1;
		}
		else {
			return 0;
		}
	}
}





#endif
