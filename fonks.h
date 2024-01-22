#ifndef FONKS_H
#define FONKS_H
#include "mystructs.h"
#include "items.h"
//#include <time.h>
#include <stdlib.h>
#include <math.h>
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
int girdiye_en_yakin_dusman_x_y(int x, int y) {

	int tutucuX, tutucuY, tutucuP;
	int uzaklikSirasi[enemy_count][2];
	for (int n = 0; n < enemy_count; n++) { 
		tutucuX = calculate_distance(x, enemyler[n].x + enemyler[n].width / 2);
		tutucuY = calculate_distance(y, enemyler[n].y + enemyler[n].height / 2);
		tutucuP = sqrt(tutucuX * tutucuX + tutucuY * tutucuY);
		uzaklikSirasi[n][0] = tutucuP;
		uzaklikSirasi[n][1] = n;
	}
	int selection_sort_holder[2];
	selection_sort_holder[0] = 10000000;
	selection_sort_holder[1] = 0;

	for (int i = 0; i < enemy_count; i++) {
		if (selection_sort_holder[0] > uzaklikSirasi[i][0]) {
			selection_sort_holder[0] = uzaklikSirasi[i][0];
			selection_sort_holder[1] = i;
		}
	}
	int will_return = selection_sort_holder[1];
	return will_return;
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

int enemyEnemyCollision(int n) {

	int tutucu = collision_kontrol_counter;

	for (int i = 0; i < enemy_count; i++) {

		if (n != i) { //solüst saðüst solalt saðalt
			int enemXR = enemyler[n].x + enemyler[n].width;
			int enemYR = enemyler[n].y + enemyler[n].height;
			if ((icindemi(enemyler[n].x, enemXR, enemyler[i].x) && icindemi(enemyler[n].y, enemYR, enemyler[i].y)) ||
				(icindemi(enemyler[n].x, enemXR, enemyler[i].x + enemyler[i].width) && icindemi(enemyler[n].y, enemYR, enemyler[i].y)) ||
				(icindemi(enemyler[n].x, enemXR, enemyler[i].x) && icindemi(enemyler[n].y, enemYR, enemyler[i].y + enemyler[i].height)) ||
				(icindemi(enemyler[n].x, enemXR, enemyler[i].x + enemyler[i].width) && icindemi(enemyler[n].y, enemYR, enemyler[i].y + enemyler[i].height))
				) {
				int enemyNX = (enemyler[n].x + enemyler[n].width) / 2;
				int enemyNY = (enemyler[n].y + enemyler[n].height) / 2;
				int enemyIX = (enemyler[i].x + enemyler[i].width) / 2;
				int enemyIY = (enemyler[i].y + enemyler[i].height) / 2;
				if (enemyNX > enemyIX) {
					enemyler[n].x += 60 * delta_time;
					enemyler[i].x -= 60 * delta_time;
				}
				else {
					enemyler[n].x -= 60 * delta_time;
					enemyler[i].x += 60 * delta_time;
				}
				if (enemyNY > enemyIY) {
					enemyler[n].y += 60 * delta_time;
					enemyler[i].y -= 60 * delta_time;
				}
				else {
					enemyler[n].y -= 60 * delta_time;
					enemyler[i].y += 60 * delta_time;
				}
			}
		}

	}
}

int autoBullet(int fromx,int fromy) {
	int closest_enemy = girdiye_en_yakin_dusman_x_y(fromx, fromy);
	int tutucuX = calculate_distance(fromx, enemyler[closest_enemy].x + enemyler[closest_enemy].width / 2);
	int tutucuY = calculate_distance(fromy, enemyler[closest_enemy].y + enemyler[closest_enemy].height / 2);
	int tutucuP = sqrt(tutucuX * tutucuX + tutucuY * tutucuY);
	if (man.range > tutucuP) {
		for (int i = 0; i < mermi_count; i++) {
			if (mermiler[i].life != 1) { // halihazýrda ateþlenmemiþ mermiyi ateþliyor
				if (attack_speed_while_loop < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateþ etsin diye ayarlamaya çalýþtým 

					if (10 < (SDL_GetTicks() - mermiler[i].biterken)) {
						mermiler[i].x = man.x + (man.size) / 2 - (mermiler[i].width) / 2;
						mermiler[i].y = man.y + man.size / 2 - mermiler[i].height / 2;
						mermiler[i].life = 1;
						last_bullet_shot = SDL_GetTicks();
						man.animation = 3;
						man.attack_animation_time_holder = SDL_GetTicks();
						if (fromx < mouse_position.x + camera.x) {
							man.attack_direction = 1;
						}
						else {
							man.attack_direction = 0;
						}
					}
				}
			}
		}
	}
}

void setTheMovement() {
	gidilecek_x = mouse_position.x + camera.x;
	gidilecek_y = mouse_position.y + camera.y;

	if (gidilecek_x > man_ortasiX) {
		man.direction = 1; //sol sýfýr  
	}
	else {
		man.direction = 0;
	}
	if (gidilecek_y > man_ortasiY) {
		y_dogrultusunda_yon = 1;
	}
	else {
		y_dogrultusunda_yon = 0;
	}

	diagnal_man_m = sqrt(pow(((gidilecek_x)-(man_ortasiX)), 2) + pow(((gidilecek_y)-(man_ortasiY + man.size / 2)), 2));
	cosforx_man_m = (man_ortasiX - (gidilecek_x)) / diagnal_man_m;
	sinfory_man_m = (man_ortasiY + man.size / 2 - (gidilecek_y)) / diagnal_man_m;
	man.moving = 1;

	float speed_constant_man = man.move_speed / sqrt((cosforx_man_m * delta_time * cosforx_man_m * delta_time) + (sinfory_man_m * delta_time * sinfory_man_m * delta_time));

	man.speedx = cosforx_man_m * speed_constant_man * delta_time;
	man.speedy = sinfory_man_m * speed_constant_man * delta_time;

	isPressed_right = 0; // bunu mod olarak ekleyebilirsin.
}

int moveTheMan() {
	if (!(man_ortasiX > gidilecek_x - 10 && man_ortasiX < gidilecek_x + 10)) {
		if (man.direction == 0 && man_ortasiX >= gidilecek_x) {
			man.x -= man.speedx;
		}

		if (man.direction == 1 && man_ortasiX <= gidilecek_x) {
			man.x -= man.speedx;
		}

	}
	if (!(man_ortasiY + man.size / 2 > gidilecek_y - 10 && man_ortasiY + man.size / 2 < gidilecek_y + 10)) {

		if (y_dogrultusunda_yon == 1 && man_ortasiY <= gidilecek_y) {
			man.y -= man.speedy;
		}
		if (y_dogrultusunda_yon == 0 && man_ortasiY >= gidilecek_y) {
			man.y -= man.speedy;
		}

	}
}

int bulletMove(int i) {
	int closest_enemy = girdiye_en_yakin_dusman_x_y(man_ortasiX, man_ortasiY);
	float diagnal = sqrt(pow(((enemyler[closest_enemy].x + enemyler[closest_enemy].width / 2) - (man.x + man.size / 2)), 2) + pow(((enemyler[closest_enemy].y + enemyler[closest_enemy].height / 2) - (man.y + man.size / 2)), 2));
	float cosforx = (man_ortasiX - (enemyler[closest_enemy].x + enemyler[closest_enemy].width / 2)) / diagnal;
	float sinfory = (man_ortasiY - (enemyler[closest_enemy].y + enemyler[closest_enemy].height / 2)) / diagnal;

	speed_constant_bullet = 30 / sqrt((cosforx * delta_time * cosforx * delta_time) + (sinfory * delta_time * sinfory * delta_time));

	if (mermiler[i].life <= 0) {
		mermiler[i].speedx = cosforx * speed_constant_bullet * delta_time;
		mermiler[i].speedy = sinfory * speed_constant_bullet * delta_time;

		mermiler[i].y = -20000;
		mermiler[i].x = -20000;
	}

	if (mermiler[i].life == 1) {
		mermiler[i].x -= mermiler[i].speedx;
		mermiler[i].y -= mermiler[i].speedy;
		if (mermiler[i].x > MAP_WIDTH || mermiler[i].y > MAP_HEIGHT || mermiler[i].x < 0 || mermiler[i].y < 0) {
			mermiler[i].life = 0;
		}
	}

	return 1;
}


int couldown_control() {

	//right click
	if (!man.q_click_ready) {
		if (10000 < SDL_GetTicks() - man.q_click_last_use) {
			man.q_click_ready = 1;
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

int spawnSpiece1() {
	int idd = 0;
	for (int n = 0; n < enemy_count; n++) {
		if (enemyler[n].current_life < 1 && idd < 1) {
			enemyler[n].y = spawnpoints.enemy_start_coord[n % 16][1];
			enemyler[n].x = spawnpoints.enemy_start_coord[n % 16][0];
			enemyler[n].width = 80;
			enemyler[n].height = 80;
			enemyler[n].max_life = 3;
			enemyler[n].current_life = enemyler[n].max_life;
			enemyler[n].speed = 120;
			enemyler[n].spiece = 1;
			enemyler[n].enemy_walking_last_time = 0;
			enemyler[n].enemy1_walking_image_width = 0;
			enemyler[n].enemy2_walking_image_width = 0;
			last_spawn = SDL_GetTicks();
			idd++;
		}
	}
	return 1;
}

int spawnSpiece2(int level) {
	int idd = 0;
	for (int n = 0; n < enemy_count; n++) {
		if (enemyler[n].current_life < 1 && idd < 1) {
			enemyler[n].y = spawnpoints.enemy_start_coord[n % 16][1];
			enemyler[n].x = spawnpoints.enemy_start_coord[n % 16][0];
			enemyler[n].width = 120 * level;
			enemyler[n].height = 120 * level;
			enemyler[n].max_life = 7 * level;
			enemyler[n].current_life = enemyler[n].max_life;
			enemyler[n].speed = 100;
			enemyler[n].spiece = 2;
			enemyler[n].enemy_walking_last_time = 0;
			enemyler[n].enemy1_walking_image_width = 0;
			enemyler[n].enemy2_walking_image_width = 0;
			last_spawn = SDL_GetTicks();
			idd++;
		}
	}
	return 1;
}

int defineSpawnPoints() {
	for (int n = 0; n < 16; n++) {

		if (n / 4 == 0) {
			spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH / 4 + n * (WINDOW_WIDTH / 2);
			spawnpoints.enemy_start_coord[n][1] = camera.y - 500;
		}
		if (n / 4 == 1) {
			spawnpoints.enemy_start_coord[n][0] = camera.x + camera.w + WINDOW_WIDTH / 4;
			spawnpoints.enemy_start_coord[n][1] = camera.y - WINDOW_HEIGHT / 4 + (n - 4) * (WINDOW_HEIGHT / 2);
		}
		if (n / 4 == 2) {
			spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH / 4 + (n - 8) * (WINDOW_WIDTH / 2);
			spawnpoints.enemy_start_coord[n][1] = camera.y + camera.w + 500;
		}
		if (n / 4 == 3) {
			spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH / 4;
			spawnpoints.enemy_start_coord[n][1] = camera.y - WINDOW_HEIGHT / 4 + (n - 12) * (WINDOW_HEIGHT / 2);
		}

	}
}


int initSpellQ() {
	man.q_click_ready = 0;
	man.q_click_last_use = SDL_GetTicks();
	for (int n = 0; n < enemy_count; n++) {

		if (icindemi(mouse_position.x - 400 + camera.x, mouse_position.x + 400 + camera.x, enemyler[n].x) && icindemi(mouse_position.y - 400 + camera.y, mouse_position.y + 400 + camera.y, enemyler[n].y)) {
			enemyler[n].current_life -= man.bullet_power * 5;
		}

		if (enemyler[n].current_life <= 0) { // eðer atýþýn adamý öldürürse aþaðýdaki fonksiyonlar çalýþacak.
			man.money += 10;

			drop_item(n);
		}// mermi için collision detection
	}
}

int manGetsDamage() {
	if (1000 < SDL_GetTicks() - last_collision_man) {
		last_collision_man = SDL_GetTicks();
		man.current_life -= 10;
		if (man.current_life <= 0) {
			man.current_life = 0;
			gui_state.page = 0;
		}
	}
	return 1;
}


int findTime() {
	game_stats.time_counter_sec = SDL_GetTicks() / 1000;
	if (game_stats.time_counter_sec > 60) {
		game_stats.time_counter_min = game_stats.time_counter_sec / 60;
		game_stats.time_counter_sec = game_stats.time_counter_sec % 60;
	}
}


int isWaveOver() {

	for (int n = 0; n < enemy_count; n++) {

		if (enemyler[n].current_life > 0) {
			return 0;
			break;
		}
	}

	return 1;

}

int spawn1Enemy() { // wave spawn function
	int zorlukPoint = game_stats.wave*2 + 3;
	int zorlukPointCheck = 0;
	srand(time(NULL));
	if (1) {
		while (zorlukPoint != zorlukPointCheck) {
			int iterasRan = rand() % 5;
			printf("%d \n", iterasRan);
			if (iterasRan == 4) {
				zorlukPointCheck += 3;
				if (zorlukPointCheck > zorlukPoint) {
					zorlukPointCheck -= 3;
					continue;
				}
				spawnSpiece2(3);
			}
			else if (iterasRan == 1 || iterasRan == 2) {
				zorlukPointCheck += 1; 
				if (zorlukPointCheck > zorlukPoint) {
					zorlukPointCheck -= 1;
					continue;
				}
				spawnSpiece2(1);
			}
			else if (iterasRan == 0 || iterasRan == 3) {
				zorlukPointCheck += 1; 
				if (zorlukPointCheck > zorlukPoint) {
					zorlukPointCheck -= 1;
					continue;
				}
				spawnSpiece1(1);
			}
		}
		zorlukPointCheck = 0;
		game_stats.wave++;
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
	man.move_speed = 4;		
	man.move_speed += owned_items.MovementFeather_c * 1;
								
								
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


#endif