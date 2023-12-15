#ifndef FONKS_H
#define FONKS_H
#include "mystructs.h"
#include "items.h"
//#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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

int collision_man_item() {
	for (int n = 0; n < 100; n++) {

		if ((icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].height)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && (icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].width)))) {
			switch (itemler[n].item_index)
			{
			case 1:
				owned_items.CursedGlove_c++;
				break;
			case 2:
				owned_items.Cookie_c++;
				break;
			case 3:
				owned_items.Adrenaline_c++;
				break;
			case 4:
				owned_items.RedTeeth_c++;
				break;

			default:
				break;
			}
			itemler[n].life = 0;
			itemler[n].x = NULL;
			itemler[n].y = NULL;
		}

	}
	return 0;
}

int drop_item(int n) {
	int item_drop_kontrol = 1;

	for (int j = 0; j < 100; j++) {
		if (itemler[j].life == 0 && item_drop_kontrol) {
			itemler[j].width = 30;
			itemler[j].height = 30;
			itemler[j].x = enemyler[n].x + enemyler[n].width/2 - itemler[j].width/2;
			itemler[j].y = enemyler[n].y + enemyler[n].height/2 - itemler[j].height / 2;
			itemler[j].life = 1;
			int rastgeleIndex = rand()%item_count;
			itemler[j].item_index = rastgeleIndex; // item index randomlaþtýrýlacak;
			item_drop_kontrol = 0;

			switch (itemler[j].item_index){
			case 1:
				itemler[j].item_texture = game_assets.item_1;
				break;
			case 2:
				itemler[j].item_texture = game_assets.item_2;
			default:
				break;}

		}
	}
	return 0;
}



int upgrade_protog() {
	man.move_speed = 200;
	man.move_speed += owned_items.Adrenaline_c * 30;


	man.max_life = 100;
	man.max_life += owned_items.Cookie_c * 5;


	return 0;
}





//int alan() { // deðiþecek
//	for (int n = 0; n < enemy_count; n++) { //burada range içine giren eleman olursa ateþ etsin diye ayarladým
//
//		if ((icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height))
//			) {
//			for (int i = 0; i < mermi_count; i++) {
//				if (mermiler[i].life != 1) { // halihazýrda ateþlenmemiþ mermiyi ateþliyor
//					if (bullet_speed_interval - man->bullet_speed < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateþ etsin diye ayarlamaya çalýþtým 
//						if (10 < (SDL_GetTicks() - mermiler[i].biterken)) {
//							mermiler[i].x = man->x + (man->size) / 2 - (mermiler[i].width) / 2;
//							mermiler[i].y = man->y + man->size / 2 - mermiler[i].height / 2;
//							mermiler[i].life = 1;
//							Mix_PlayChannel(-1, shootEffect, 0);
//							last_bullet_shot = SDL_GetTicks();
//							man->animation = 3;
//							man->attack_animation_time_holder = SDL_GetTicks();
//							if (man_ortasiX < enemyler[closest_enemy_index].x) {
//								man->attack_direction = 1;
//							}
//							else {
//								man->attack_direction = 0;
//							}
//
//
//						}
//					}
//				}
//			}
//		}
//	}
//}


#endif