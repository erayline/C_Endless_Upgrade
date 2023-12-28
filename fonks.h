<<<<<<< HEAD
#ifndef FONKS_H
#define FONKS_H
#include "mystructs.h"
#include "items.h"
//#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "items.h"
int icindemi(float interval_left, float interval_right, float middle_point) {
	//if it is in it is one
	
	if ((middle_point >= interval_left) && (middle_point <= interval_right)) {
		return 1;
	}
	else {
		return 0;
	}
}


int collision_man_enemy() {


	for (int n = 0; n < enemy_count; n++) {
		if (enemyler[n].current_life > 0) {

			if ( 
				(icindemi(man.x, man.x + man.size, enemyler[n].x)						&& icindemi(man.y, man.y + man.size, enemyler[n].y							)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x + enemyler[n].width)	&& icindemi(man.y, man.y + man.size, enemyler[n].y							)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x)						&& icindemi(man.y, man.y + man.size, enemyler[n].y + enemyler[n].height		)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x + enemyler[n].width)	&& icindemi(man.y, man.y + man.size, enemyler[n].y + enemyler[n].width		)) 
			) {
			
				return 1;
			}
		}
	}
	return 0;
}


int collision_man_item() {
	for (int n = 0; n < 100; n++) {
		if (itemler[n].life == 1) {
			if ((icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].height)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && (icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].width)))) {
				
				//eþya listesi
				for (int i = 0; i < 10; i++) {

					if (item_index_list[i] == itemler[n].item_index) {
						break;
					}
					if (item_index_list[i] == 0) {
						item_index_list[i] = itemler[n].item_index;
						break;
					}

				}

				switch (itemler[n].item_index)
				{
				case 1:
					owned_items.CursedGlove_c++;
					break;
				case 2:
					owned_items.MovementFeather_c++;
					break;
				case 3:
					owned_items.LifeReminder_c++;
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
	}


	return 1;
}

//this is the true false generator for entered number 100 for definately true.
int drop_rate(int number) {
	int rastgele_sayi = rand() % 100 + 1; // [1,100] it is going to return a number in this interval.
	if (rastgele_sayi <= number) {
		return 1;
	}
	else {
		return 0;
	}
}

int drop_item(int n) {
	for (int j = 0; j < 100; j++) {
		if (itemler[j].life == 0) {
			if (drop_rate(40)) {
				itemler[j].width = 20;
				itemler[j].height = 20;
				itemler[j].x = enemyler[n].x + enemyler[n].width/2 - itemler[j].width/2;
				itemler[j].y = enemyler[n].y + enemyler[n].height/2 - itemler[j].height / 2;
				itemler[j].life = 1;
				int rastgeleIndex = rand()%item_count;
				itemler[j].item_index = rastgeleIndex; // item index randomlaþtýrýlacak;
				switch (itemler[j].item_index){
				case 1:
					itemler[j].item_texture = game_assets.item_1;
					break;
				case 2:
					itemler[j].item_texture = game_assets.item_2;
					break;
				case 3:
					itemler[j].item_texture = game_assets.item_3;
					break;
				default:
					break;}
			}
			break;
		}
	}
	return 0;
}


								
								
int upgrade_protog() {			
	// move speed;				
	man.move_speed = 200;		
	man.move_speed += owned_items.MovementFeather_c * 30;
								
								
	// attack speed				
	CursedGlove();
	LifeReminder();

	return 0;
}


int calculate_distance(int deger1, int deger2) { // iki sayýnýn farkýnýn mutlaðýný bulan fonksiyon tanýmladým uzaklýk bulurken falan kullanýcam.
	int sonuc = 0;
	return abs(deger1 - deger2);
}


int ilk_buyukmu(int deger1, int deger2) {
	if (deger1 >= deger2) {
		return 1;
	}
	else {
		return 0;
	}
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


///////// bubble sort yaptýðýn yer ama selection sort'a çevireceksin zaten bunu.
		// burada bubble sort inþa ettim sýfýrdan kendim. iþime yarayacak kadar çalýþýyor.
//int uzaklikSirasi[enemy_count][2];
//int tutucuX, tutucuY, tutucuP;//pisagor
//for (int n = 0; n < enemy_count; n++) {    // bubble sort için uzaklýklarý listede toplama
//	tutucuX = calculate_distance(man_ortasiX, enemyler[n].x + enemyler[n].width / 2);
//	tutucuY = calculate_distance(man_ortasiY, enemyler[n].y + enemyler[n].height / 2);

//	tutucuP = sqrt(tutucuX * tutucuX + tutucuY * tutucuY);
//	uzaklikSirasi[n][0] = tutucuP;
//	uzaklikSirasi[n][1] = n;
//}

//int tutucu2;
//int tutucu2_n;
//for (int n = 0; n < enemy_count - 1; n++) { // bubblesortun kendisi

//	if (!(ilk_buyukmu(uzaklikSirasi[n][0], uzaklikSirasi[n + 1][0]))) {
//		tutucu2 = uzaklikSirasi[n][0];
//		tutucu2_n = uzaklikSirasi[n][1];

//		uzaklikSirasi[n][0] = uzaklikSirasi[n + 1][0];
//		uzaklikSirasi[n][1] = uzaklikSirasi[n + 1][1];

//		uzaklikSirasi[n + 1][0] = tutucu2;
//		uzaklikSirasi[n + 1][1] = tutucu2_n;
//	}
//}
//int closest_enemy_index = uzaklikSirasi[enemy_count - 1][1]; // en yakýn enemy



#endif
=======
#ifndef FONKS_H
#define FONKS_H
#include "mystructs.h"
#include "items.h"
//#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "items.h"
int icindemi(float interval_left, float interval_right, float middle_point) {
	//if it is in it is one
	
	if ((middle_point >= interval_left) && (middle_point <= interval_right)) {
		return 1;
	}
	else {
		return 0;
	}
}


int collision_man_enemy() {


	for (int n = 0; n < enemy_count; n++) {
		if (enemyler[n].current_life > 0) {

			if ( 
				(icindemi(man.x, man.x + man.size, enemyler[n].x)						&& icindemi(man.y, man.y + man.size, enemyler[n].y							)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x + enemyler[n].width)	&& icindemi(man.y, man.y + man.size, enemyler[n].y							)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x)						&& icindemi(man.y, man.y + man.size, enemyler[n].y + enemyler[n].height		)) || 
				(icindemi(man.x, man.x + man.size, enemyler[n].x + enemyler[n].width)	&& icindemi(man.y, man.y + man.size, enemyler[n].y + enemyler[n].width		)) 
			) {
			
				return 1;
			}
		}
	}
	return 0;
}


int collision_man_item() {
	for (int n = 0; n < 100; n++) {
		if (itemler[n].life == 1) {
			if ((icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && icindemi(man.y, man.y + man.size, itemler[n].y)) || (icindemi(man.x, man.x + man.size, itemler[n].x) && icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].height)) || (icindemi(man.x, man.x + man.size, itemler[n].x + itemler[n].width) && (icindemi(man.y, man.y + man.size, itemler[n].y + itemler[n].width)))) {
				
				//eÅŸya listesi
				for (int i = 0; i < 10; i++) {

					if (item_index_list[i] == itemler[n].item_index) {
						break;
					}
					if (item_index_list[i] == 0) {
						item_index_list[i] = itemler[n].item_index;
						break;
					}

				}

				switch (itemler[n].item_index)
				{
				case 1:
					owned_items.CursedGlove_c++;
					break;
				case 2:
					owned_items.MovementFeather_c++;
					break;
				case 3:
					owned_items.LifeReminder_c++;
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
	}


	return 1;
}

//this is the true false generator for entered number 100 for definately true.
int drop_rate(int number) {
	int rastgele_sayi = rand() % 100 + 1; // [1,100] it is going to return a number in this interval.
	if (rastgele_sayi <= number) {
		return 1;
	}
	else {
		return 0;
	}
}

int drop_item(int n) {
	for (int j = 0; j < 100; j++) {
		if (itemler[j].life == 0) {
			if (drop_rate(40)) {
				itemler[j].width = 20;
				itemler[j].height = 20;
				itemler[j].x = enemyler[n].x + enemyler[n].width/2 - itemler[j].width/2;
				itemler[j].y = enemyler[n].y + enemyler[n].height/2 - itemler[j].height / 2;
				itemler[j].life = 1;
				int rastgeleIndex = rand()%item_count;
				itemler[j].item_index = rastgeleIndex; // item index randomlaÅŸtÄ±rÄ±lacak;
				switch (itemler[j].item_index){
				case 1:
					itemler[j].item_texture = game_assets.item_1;
					break;
				case 2:
					itemler[j].item_texture = game_assets.item_2;
					break;
				case 3:
					itemler[j].item_texture = game_assets.item_3;
					break;
				default:
					break;}
			}
			break;
		}
	}
	return 0;
}


								
								
int upgrade_protog() {			
	// move speed;				
	man.move_speed = 200;		
	man.move_speed += owned_items.MovementFeather_c * 30;
								
								
	// attack speed				
	CursedGlove();
	LifeReminder();

	return 0;
}


int calculate_distance(int deger1, int deger2) { // iki sayÄ±nÄ±n farkÄ±nÄ±n mutlaÄŸÄ±nÄ± bulan fonksiyon tanÄ±mladÄ±m uzaklÄ±k bulurken falan kullanÄ±cam.
	int sonuc = 0;
	return abs(deger1 - deger2);
}


int ilk_buyukmu(int deger1, int deger2) {
	if (deger1 >= deger2) {
		return 1;
	}
	else {
		return 0;
	}
}
//int alan() { // deÄŸiÅŸecek
//	for (int n = 0; n < enemy_count; n++) { //burada range iÃ§ine giren eleman olursa ateÅŸ etsin diye ayarladÄ±m
//
//		if ((icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
//			(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height))
//			) {
//			for (int i = 0; i < mermi_count; i++) {
//				if (mermiler[i].life != 1) { // halihazÄ±rda ateÅŸlenmemiÅŸ mermiyi ateÅŸliyor
//					if (bullet_speed_interval - man->bullet_speed < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateÅŸ etsin diye ayarlamaya Ã§alÄ±ÅŸtÄ±m 
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


///////// bubble sort yaptÄ±ÄŸÄ±n yer ama selection sort'a Ã§evireceksin zaten bunu.
		// burada bubble sort inÅŸa ettim sÄ±fÄ±rdan kendim. iÅŸime yarayacak kadar Ã§alÄ±ÅŸÄ±yor.
//int uzaklikSirasi[enemy_count][2];
//int tutucuX, tutucuY, tutucuP;//pisagor
//for (int n = 0; n < enemy_count; n++) {    // bubble sort iÃ§in uzaklÄ±klarÄ± listede toplama
//	tutucuX = calculate_distance(man_ortasiX, enemyler[n].x + enemyler[n].width / 2);
//	tutucuY = calculate_distance(man_ortasiY, enemyler[n].y + enemyler[n].height / 2);

//	tutucuP = sqrt(tutucuX * tutucuX + tutucuY * tutucuY);
//	uzaklikSirasi[n][0] = tutucuP;
//	uzaklikSirasi[n][1] = n;
//}

//int tutucu2;
//int tutucu2_n;
//for (int n = 0; n < enemy_count - 1; n++) { // bubblesortun kendisi

//	if (!(ilk_buyukmu(uzaklikSirasi[n][0], uzaklikSirasi[n + 1][0]))) {
//		tutucu2 = uzaklikSirasi[n][0];
//		tutucu2_n = uzaklikSirasi[n][1];

//		uzaklikSirasi[n][0] = uzaklikSirasi[n + 1][0];
//		uzaklikSirasi[n][1] = uzaklikSirasi[n + 1][1];

//		uzaklikSirasi[n + 1][0] = tutucu2;
//		uzaklikSirasi[n + 1][1] = tutucu2_n;
//	}
//}
//int closest_enemy_index = uzaklikSirasi[enemy_count - 1][1]; // en yakÄ±n enemy



#endif
>>>>>>> 6cf67f5a8adef569a2d92394793c98de19602a33
