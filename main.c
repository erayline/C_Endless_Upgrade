#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "mystructs.h"
#include "items.h"
#include "fonks.h"

//#include <string.h>
typedef struct {
	int enemy_start_coord[16][2]; // 8 adet düşmanın başlangıç koordinatları
} SpawnPoints;

SpawnPoints spawnpoints = {
	{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};

SDL_Rect map1 = { 0,0,MAP_WIDTH,MAP_HEIGHT};




/*  yapılacaklar :
	//text oluşturmayı çok kullanacağın için fonksiyon falan oluştur. (eşyalara'lere fareyi sürükleyince, bölümü yazdırırken falan)
	//eşya oluştur sürü sürü
	//haritayı adam akıllı yap

	yapmakta olduklarım:


   yapılanlar:


*/





int calculate_distance(int deger1, int deger2) { // iki sayının farkının mutlağını bulan fonksiyon tanımladım uzaklık bulurken falan kullanıcam.
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



int last_spawn = 1;
int atack_speed = 400;
int bullet_speed_interval = 500;
int last_frame_time = 0;
float bullet_x_speed = 0;
float bullet_y_speed = 0; // burada birsürü gereksiz şey tanımlamışım bir ara azaltıcam.

int animation_control_timer = 0;
int processEvents(SDL_Window* window, GUI_State* gui_state, Game_stats* game_stats, Man* man, Bullet* mermiler, Enemy* enemyler, Mix_Chunk* hitEffect, Mix_Chunk* shootEffect, Item* itemler) { // burada işlemler oluyor

	int done = 0;
	SDL_Event event;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; // burada fps ile ilgili ayarlar var
	last_frame_time = SDL_GetTicks();


	game_stats->time_counter_sec = SDL_GetTicks()/1000;
	if (game_stats->time_counter_sec > 60) {
		game_stats->time_counter_min = game_stats->time_counter_sec / 60;
		game_stats->time_counter_sec = game_stats->time_counter_sec%60;
	}

	for (int n = 0; n < 16; n++) {
		spawnpoints.enemy_start_coord[n][0] = (MAP_WIDTH / 16) * n;
		spawnpoints.enemy_start_coord[n][1] = 50;
	}


	while (SDL_PollEvent(&event)) {  //klavye harektlerini detect

		SDL_Delay(20);

		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {

			case (SDLK_ESCAPE):
				done = 1;
				break;
			case SDLK_SPACE: //denemek için
				gui_state->page = 1;
				man->animation = 3;
				break;
			case SDLK_p:
				if (gui_state->page == 1) {
					gui_state->page = 9;
					break;
				}
				if (gui_state->page == 9) {
					gui_state->page = 1;
				}
				break;
			}
			break;

		case SDL_QUIT:
			SDL_DestroyWindow(window);
			break;
		}
		break;
	}

	/// deneme başlangıç


	/// deneme bitiş

	if (gui_state->page == 0) {

	}
	if (gui_state->page == 1) {


	
		// ana karakter hasar tespit  solüst sağüst solaşağı sağaşağı
		
		if (collision_man_enemy(man, enemyler, gui_state)) {
			//if (1000 < SDL_GetTicks() - last_collision_man) {
				man->current_life -= 10;
				last_collision_man = SDL_GetTicks();

				if (man->current_life <= 0) {
					gui_state->page = 0;
				}
			//}
		}

		//for (int n = 0; n < enemy_count; n++) {
		//	if ((icindemi(man->x, man->x + man->size, enemyler[n].x) && icindemi(man->y, man->y + man->size, enemyler[n].y)) || (icindemi(man->x, man->x + man->size, enemyler[n].x, enemyler[n].width) && icindemi(man->y, man->y + man->size, enemyler[n].y)) || (icindemi(man->x, man->x + man->size, enemyler[n].x) && icindemi(man->y, man->y + man->size, enemyler[n].y + enemyler[n].height)) || (icindemi(man->x, man->x + man->size, enemyler[n].x + enemyler[n].width) && (icindemi(man->y, man->y + man->size, enemyler[n].y + enemyler[n].width)))) {
		//		if (1000 < SDL_GetTicks() - last_collision_man) {
		//			man->current_life -= 10;
		//			last_collision_man = SDL_GetTicks();
		//			if (man->current_life <= 0) {
		//				gui_state->page = 0;
		//			}
		//		}
		//	}
		//}

		printf("%d %d\n", man->x, man->y);




		if (1040 < SDL_GetTicks() - man->attack_animation_time_holder) {

			if (100 < SDL_GetTicks() - animation_control_timer) {
				if ((man->x != man->x_holder) || (man->y != man->y_holder)) {
					man->animation = 2;
					man->x_holder = man->x;
					man->y_holder = man->y;
				}
				else {
					man->animation = 1;
					man->x_holder = man->x;
					man->y_holder = man->y;
				}
				animation_control_timer = SDL_GetTicks();
			}

		}

		//enemylerin collision başlangıç
		for (int n = 0; n < enemy_count; n++) {
			for (int i = 0; i < enemy_count; i++) {

				if (n != i) { //solüst sağüst solalt sağalt
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
						}
						else{
							enemyler[n].x -= 60 * delta_time;
						}
						if (enemyNY > enemyIY) {
							enemyler[n].y += 60 * delta_time;
						}
						else{
							enemyler[n].y -= 60 * delta_time;
						}
					}
				}
			}
		}
		//enemylerin çarpışması bitiş

		//item drop başlangıç



		//item drop bitiş

		//  2 & 3 -> 10 & 11 _>  2
		// eğer düşmanla aynı pixeldeyse hasar alsın, ve bir saniye invincible olsun

		int man_ortasiX = man->x + man->size / 2;
		int man_ortasiY = man->y + man->size / 2;  // sürekli adamın ortasını bulmaya çalıştığım için kısalttım bu şekilde


		// burada bubble sort inşa ettim sıfırdan kendim. işime yarayacak kadar çalışıyor.
		int uzaklikSirasi[enemy_count][2];
		int tutucuX, tutucuY, tutucuP;//pisagor

		for (int n = 0; n < enemy_count; n++) {    // bubble sort için uzaklıkları listede toplama
			tutucuX = calculate_distance(man_ortasiX, enemyler[n].x + enemyler[n].width / 2);
			tutucuY = calculate_distance(man_ortasiY, enemyler[n].y + enemyler[n].height / 2);

			tutucuP = sqrt(tutucuX * tutucuX + tutucuY * tutucuY);
			uzaklikSirasi[n][0] = tutucuP;
			uzaklikSirasi[n][1] = n;
		}

		int tutucu2;
		int tutucu2_n;
		for (int n = 0; n < enemy_count - 1; n++) { // bubblesortun kendisi

			if (!(ilk_buyukmu(uzaklikSirasi[n][0], uzaklikSirasi[n + 1][0]))) {
				tutucu2 = uzaklikSirasi[n][0];
				tutucu2_n = uzaklikSirasi[n][1];

				uzaklikSirasi[n][0] = uzaklikSirasi[n + 1][0];
				uzaklikSirasi[n][1] = uzaklikSirasi[n + 1][1];

				uzaklikSirasi[n + 1][0] = tutucu2;
				uzaklikSirasi[n + 1][1] = tutucu2_n;
			}
		}
		int closest_enemy_index = uzaklikSirasi[enemy_count - 1][1]; // en yakın enemy


		// hareket etme başlanıgç


		const Uint8* state = SDL_GetKeyboardState(NULL);



		if ((state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_RIGHT]) || (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN])) {
			if (state[SDL_SCANCODE_RIGHT]) {
				man->direction = 1;
				man->direction_key = 1;
				man->x += man->move_speed * delta_time * 0.8f;
			}
			if (state[SDL_SCANCODE_LEFT]) {
				man->direction = 0;
				man->direction_key = 1;
				man->x -= man->move_speed * delta_time * 0.8f * 0.8f;

			}
			if (state[SDL_SCANCODE_UP]) {
				man->y -= man->move_speed * delta_time * 0.8f * 0.8f;

			}
			if (state[SDL_SCANCODE_DOWN]) {
				man->y += man->move_speed * delta_time * 0.8f;

			}
		}

		else {
			if (state[SDL_SCANCODE_RIGHT]) {
				man->direction = 1;
				man->direction_key = 1;
				man->x += man->move_speed * delta_time;


			}
			if (state[SDL_SCANCODE_LEFT]) {
				man->direction = 0;
				man->direction_key = 1;
				man->x -= man->move_speed * delta_time * 0.8f;

			}
			if (state[SDL_SCANCODE_UP]) {
				man->y -= man->move_speed * delta_time * 0.8f;


			}
			if (state[SDL_SCANCODE_DOWN]) {
				man->y += man->move_speed * delta_time;

			}
		}

		// hareket etme bitiş



		//mermi fonksiyonları başlangıç

		for (int n = 0; n < enemy_count; n++) { //burada range içine giren eleman olursa ateş etsin diye ayarladım
			//old kod // 		if (((((enemyler[n].x > man->x - man->range) && (enemyler[n].x < man->x + man->range)) && ((enemyler[n].y > man->y - man->range) && (enemyler[n].y < man->y + man->range))) || (((enemyler[n].x + enemyler[n].width > man->x - man->range) && (enemyler[n].x + enemyler[n].width < man->x + man->range)) && ((enemyler[n].y + enemyler[n].height > man->y - man->range) && (enemyler[n].y + enemyler[n].height < man->y + man->range))))) {
			// new kod // (icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x)	&& icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y))	||  (icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y + enemyler[n].height)) ||(icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y)) ||(icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y + enemyler[n].height)))
			if ((icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
				(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height)) ||
				(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
				(icindemi(man->x - man->range, man->x + man->size + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height))
				) {
				for (int i = 0; i < mermi_count; i++) {
					if (mermiler[i].life != 1) { // halihazırda ateşlenmemiş mermiyi ateşliyor
						if (bullet_speed_interval - man->bullet_speed < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateş etsin diye ayarlamaya çalıştım 
							if (10 < (SDL_GetTicks() - mermiler[i].biterken)) {
								mermiler[i].x = man->x + (man->size) / 2 - (mermiler[i].width) / 2;
								mermiler[i].y = man->y + man->size / 2 - mermiler[i].width / 2;
								mermiler[i].life = 1;
								Mix_PlayChannel(-1, shootEffect, 0);
								last_bullet_shot = SDL_GetTicks();
								man->animation = 3;
								man->attack_animation_time_holder = SDL_GetTicks();
								if (man_ortasiX < enemyler[closest_enemy_index].x) {
									man->attack_direction = 1;
								}
								else {
									man->attack_direction = 0;
								}


							}
						}
					}
				}
			}
		}

		for (int i = 0; i < mermi_count; i++) {

			if (mermiler[i].life == 1) {
				mermiler[i].x -= mermiler[i].speedx;
				mermiler[i].y -= mermiler[i].speedy;
			}
		}



		speed_constant = 400;
		float diagnal = uzaklikSirasi[enemy_count - 1][0];

		float cosforx = (man_ortasiX - enemyler[closest_enemy_index].x - enemyler[closest_enemy_index].width / 2) / diagnal;
		float sinfory = (man_ortasiY - enemyler[closest_enemy_index].y - enemyler[closest_enemy_index].height / 2) / diagnal;

		for (int i = 0; i < mermi_count; i++) {

			if (mermiler[i].life <= 0) {
				mermiler[i].speedx = cosforx * speed_constant * delta_time;
				mermiler[i].speedy = sinfory * speed_constant * delta_time;

				mermiler[i].y = 20000;
				mermiler[i].x = 20000;
			}
		}

		/// mermi fonksiyonları bitiş


		//enemy güncelleme başlangıç
		// spawn olacakların sayısını tanımladığın bir değişkenin olabilir.
		int idd = 0;
		if (3000 < SDL_GetTicks() - last_spawn) {
			last_spawn = SDL_GetTicks();


			for (int n = 0; n < enemy_count; n++) {

				if (enemyler[n].current_life < 1 && idd < 3) {
					enemyler[n].y = spawnpoints.enemy_start_coord[n % 8][1];
					enemyler[n].x = spawnpoints.enemy_start_coord[n % 8][0];
					enemyler[n].width = 50;
					enemyler[n].height = 50;
					enemyler[n].current_life = 3;
					enemyler[n].max_life = 3;
					enemyler[n].speed = 100;
					enemyler[n].enemy1_walking_last_time = 0;
					enemyler[n].enemy1_walking_image_width = 0;
					last_spawn = SDL_GetTicks();
					idd++;

				}
			}
		}

		for (int n = 0; n < enemy_count; n++) {


			if (enemyler[n].current_life != enemyler[n].max_life) {
				enemyler[n].life_bar_active = 1;
			}

			if (enemyler[n].current_life == enemyler[n].max_life) {
				enemyler[n].life_bar_active = 0;
			}


		}


		for (int n = 0; n < enemy_count; n++) {

			float diagnal_enem = sqrt(pow(man_ortasiX - (enemyler[n].x + enemyler[n].width / 2), 2) + pow(man_ortasiY - (enemyler[n].y + enemyler[n].width / 2), 2));
			float cosforx_enem = (man_ortasiX - enemyler[n].x - enemyler[n].width / 2) / diagnal_enem;
			float sinfory_enem = (man_ortasiY - enemyler[n].y - enemyler[n].height / 2) / diagnal_enem;

			if (enemyler[n].current_life <= 0) {
				enemyler[n].y = -10000;
				enemyler[n].x = -10000;
			}
			else {
				if (enemyler[n].current_life != 0) { // enemylerin hızlarını ayarladığım bölüm
					if (enemyler[n].y + enemyler[n].height / 2 != man->y + man->size / 2) {
						enemyler[n].y += sinfory_enem * enemyler[n].speed * delta_time;
					}
					if (enemyler[n].x + enemyler[n].width / 2 != man->x + man->size / 2) {
						enemyler[n].x += cosforx_enem * enemyler[n].speed * delta_time;
					}
				}


				for (int i = 0; i < mermi_count; i++) {
					if ((((enemyler[n].x < mermiler[i].x) && (enemyler[n].x + enemyler[n].width > mermiler[i].x)) || ((enemyler[n].x < mermiler[i].x + mermiler[i].width) && (enemyler[n].x + enemyler[n].width > mermiler[i].x + mermiler[i].width))) && (((enemyler[n].y < mermiler[i].y) && (enemyler[n].y + enemyler[n].height > mermiler[i].y)) || ((enemyler[n].y < mermiler[i].y + mermiler[i].height) && (enemyler[n].y + enemyler[n].height > mermiler[i].y + mermiler[i].height)))) {
						enemyler[n].current_life -= man->bullet_power;
						if (enemyler[n].current_life <= 0) {
							man->money += 10;

							int item_drop_kontrol = 1;
							for (int j = 0; j < 100; j++) {
								if (itemler[j].life == 0 && item_drop_kontrol) {
									itemler[j].x = enemyler[n].x;
									itemler[j].y = enemyler[n].y;
									itemler[j].width = 50;
									itemler[j].height = 50;
									itemler[j].life = 1;
									itemler[j].item_index = 1; // item index randomlaştırılacak;
									item_drop_kontrol = 0;
								}
							}
						
						}// mermi için collision detection

						mermiler[i].life = 0;
						Mix_PlayChannel(-1, hitEffect, 0);
						mermiler[i].biterken = SDL_GetTicks();
					}
				}
			}
			//düşman güncelleme bitiş
		}

		camera.x = man->x - (camera.w / 2);
		camera.y = man->y - (camera.h / 2);

	} // bu gui'nin

	if (gui_state->page == 9) {

	}

	return done;
}
// ölü değilken collisen olsun enemyler için

/// <summary>
///  taşınacaklar
/// </summary>
int enson = 0;
void doRender(SDL_Renderer* renderer, GUI_State* gui_state, Game_stats* game_stats, Man* man, Bullet* mermiler, Enemy* enemyler, TTF_Font* font50, Game_assets* game_assets, Item* itemler) {

	if (gui_state->page == 0) {

		SDL_Color white = { 252,215,222,255 };
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Welcome to the COLLECTOR", white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_welcome = { WINDOW_WIDTH / 2 - text_surface->w / 2, WINDOW_HEIGHT / 2 - text_surface->h / 2, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_welcome);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);


		text_surface = TTF_RenderText_Solid(font50, "press SPACE to start ", white);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2, WINDOW_HEIGHT - text_surface->h - 50, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);



	}
	if (gui_state->page == 1) {
		if (camera.x < 0) camera.x = 0;
		if (camera.y < 0) camera.y = 0;
		if (camera.x > map1.w - camera.w) camera.x = map1.w - camera.w;
		if (camera.y > map1.h - camera.h) camera.y = map1.h - camera.h;

		SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
		SDL_RenderClear(renderer); // arkaplan katmanı

		SDL_Rect map_on_screen = { map1.x - camera.x ,map1.y - camera.y,map1.w,map1.h };
		SDL_RenderCopy(renderer, game_assets->map_image_texture, NULL, &map_on_screen);




		// protogonist çizdirme başlangıç //

		int enemy_start_coord[8][2] = { // enemylerin konumlarını manuel ayarlayacağım ondan for kullanmadım henüz.
		{10,10},{10,WINDOW_HEIGHT - 10},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 10},{WINDOW_WIDTH - 10,10},{WINDOW_WIDTH - 10,WINDOW_HEIGHT - 10},{10,WINDOW_HEIGHT / 2},{WINDOW_WIDTH - 10,WINDOW_HEIGHT / 2}
		};


		if (man->animation == 1) {
			

			
				
			SDL_Rect player_on_screen = { man->x - man->size*3 - camera.x,man->y - man->size*3 - camera.y, 250,250};
			SDL_Rect protogonist_image_rect = { 0 + man->idle_image_width_increment, 0, 250, 250 };
		
			while (100<SDL_GetTicks() - man->idle_image_timer) {
				man->idle_image_timer = SDL_GetTicks();
				man->idle_image_width_increment += 250;
				if (man->idle_image_width_increment > 1200) {
					man->idle_image_width_increment = 0;
				}
			}
			if (man->direction == 1) {
				SDL_RenderCopy(renderer, game_assets->protogonist_texture_idle, &protogonist_image_rect, &player_on_screen);
			}
			if (man->direction == 0) {
				SDL_RenderCopyEx(renderer, game_assets->protogonist_texture_idle, &protogonist_image_rect, &player_on_screen, 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
		}
		SDL_Rect item_example[100];
		for (int n = 0; n < 100; n++) {
			if (itemler[n].life == 1) {

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

				item_example[n].x = itemler[n].x - camera.x;
				item_example[n].y = itemler[n].y - camera.y;
				item_example[n].w = itemler[n].width;
				item_example[n].h = itemler[n].height;

				SDL_RenderFillRect(renderer, &item_example);
			}
		}
		if (man->animation == 2) {
			SDL_Rect man_on_screen1 = { man->x - man->size * 3 - camera.x,man->y - man->size * 3 -camera.y , 250,250 };
			SDL_Rect protogonist_image_rect1 = { 0 + man->run_image_width_increment, 0, 250, 250 };

			while (90 < SDL_GetTicks() - man->run_image_timer) {
				man->run_image_timer = SDL_GetTicks();
				man->run_image_width_increment += 250;
				if (man->run_image_width_increment > 1200) {
					man->run_image_width_increment = 0;
				}
			}
			if (man->direction == 1) {
				SDL_RenderCopy(renderer, game_assets->protogonist_texture_run, &protogonist_image_rect1, &man_on_screen1);
			}
			if (man->direction == 0) {
				SDL_RenderCopyEx(renderer, game_assets->protogonist_texture_run, &protogonist_image_rect1, &man_on_screen1, 0.0, NULL, SDL_FLIP_HORIZONTAL);
				//man->direction_key = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 10, 80, 30, 200);
		SDL_Rect man_range_rect = { man->x + man->size/2 - man->range - camera.x,man->y + man->size / 2 - man->range - camera.y, man->range * 2, man->range * 2 };
		SDL_RenderDrawRect(renderer, &man_range_rect); // protogonist menzili bitiş

		if (man->animation == 3) {

			
			SDL_Rect rect2 = { man->x - man->size * 3-camera.x,man->y - man->size * 3-camera.y, 250,250 };
			SDL_Rect protogonist_image_rect2 = {  0+ man->attack_image_width_increment, 0, 250, 250 };

			while (90 < SDL_GetTicks() - man->attack_image_timer) {
				man->attack_image_timer = SDL_GetTicks();
				man->attack_image_width_increment += 250;
				if (man->attack_image_width_increment > 1200) {
					man->attack_image_width_increment = 0;
				}
			}
			if (man->attack_direction == 0) {
				SDL_RenderCopyEx(renderer, game_assets->protogonist_texture_attack, &protogonist_image_rect2, &rect2, 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			if (man->attack_direction == 1) {
				SDL_RenderCopy(renderer, game_assets->protogonist_texture_attack, &protogonist_image_rect2, &rect2);
			}
		}






		// protogonist çizdirme bitiş //

		SDL_Rect mermiler_1[mermi_count];
		for (int n = 0; n < mermi_count; n++) {
			mermiler_1[n].x = mermiler[n].x;
			mermiler_1[n].y = mermiler[n].y;
			mermiler_1[n].w = mermiler[n].width;
			mermiler_1[n].h = mermiler[n].height;
		}	// mermi katmanı bitiş

		for (int n = 0; n < mermi_count; n++) {
			if (mermiler[n].life != 0) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_Rect bullet_1 = { mermiler[n].x - camera.x,mermiler[n].y - camera.y,mermiler[n].width,mermiler[n].height };
				SDL_RenderFillRect(renderer, &bullet_1);
			}
		}




		SDL_Rect enemy_1[enemy_count];
		SDL_Rect enemy_1_dec[enemy_count];
		for (int n = 0; n < enemy_count; n++) {
			enemy_1[n].x = enemyler[n].x - camera.x;
			enemy_1[n].y = enemyler[n].y - camera.y;
			enemy_1[n].w = enemyler[n].width*2;
			enemy_1[n].h = enemyler[n].height*2;
		}

		// bar oluşturma


		SDL_Rect bar_rect_back[enemy_count];
		SDL_Rect bar_rect_front[enemy_count];
		for (int n = 0; n < enemy_count; n++) {

			bar_rect_back[n].x = enemyler[n].x - camera.x;
			bar_rect_back[n].y = enemyler[n].y - camera.y;
			bar_rect_back[n].w = enemy_life_bar_width;
			bar_rect_back[n].h = enemy_life_bar_height;

			bar_rect_front[n].x = enemyler[n].x - camera.x + 5;
			bar_rect_front[n].y = enemyler[n].y - camera.y + 5;
			bar_rect_front[n].w = (enemy_life_bar_width - 10) - (enemyler[n].max_life - enemyler[n].current_life) * (enemy_life_bar_width/enemyler[n].max_life);
			bar_rect_front[n].h = enemy_life_bar_height - 10;

			
			if ((enemyler[n].current_life > 0) && (enemyler[n].life_bar_active)) {



				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_RenderFillRect(renderer, &bar_rect_back[n]);
				SDL_SetRenderDrawColor(renderer, 250, 40, 40, 255);
				SDL_RenderFillRect(renderer, &bar_rect_front[n]);

			}


		}

		//enemy print etme başlangıç
		for (int n = 0; n < enemy_count; n++) {

			if (enemyler[n].current_life > 0) {

				float diagnal_enem = sqrt(pow(man->x + man->size / 2 - enemyler[n].x - enemyler[n].width / 2, 2) + pow(man->y + man->size / 2 - enemyler[n].y - enemyler[n].width / 2, 2));
				float cosforx_enem = (man->x + man->size / 2 - enemyler[n].x - enemyler[n].width / 2) / diagnal_enem;
				float sinfory_enem = (man->y + man->size / 2 - enemyler[n].y - enemyler[n].height / 2) / diagnal_enem;
				if (cosforx_enem > 0) {

					if (100 < SDL_GetTicks() - enemyler[n].enemy1_walking_last_time) {
						enemyler[n].enemy1_walking_image_width += 150;
						enemyler[n].enemy1_walking_last_time = SDL_GetTicks();
						if (enemyler[n].enemy1_walking_image_width > 1050) {
							enemyler[n].enemy1_walking_image_width = 0;
						}
					}
					SDL_Rect enemy1_image_rect = { 50 + enemyler[n].enemy1_walking_image_width,50,50,50 };
					SDL_RenderCopy(renderer, game_assets->enemy1_image_texture, &enemy1_image_rect, &enemy_1[n]);
				}
				else {
					if (100 < SDL_GetTicks() - enemyler[n].enemy1_walking_last_time) {
						enemyler[n].enemy1_walking_image_width += 150;
						enemyler[n].enemy1_walking_last_time = SDL_GetTicks();
						if (enemyler[n].enemy1_walking_image_width > 1050) {
							enemyler[n].enemy1_walking_image_width = 0;
						}
					}
					SDL_Rect enemy1_image_rect = { 50 + enemyler[n].enemy1_walking_image_width,50,50,50 };
					SDL_RenderCopyEx(renderer, game_assets->enemy1_image_texture, &enemy1_image_rect, &enemy_1[n], 0.0, NULL, SDL_FLIP_HORIZONTAL);
				}
			}
		}// enemy print etme bitiş


		//ekrana yazı yazdırma başlangıç
		char puan_str[16];

		sprintf_s(puan_str, sizeof(puan_str), "Money: %d", man->money);
		SDL_Color white = { 22,132,22,233 }; // para
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, puan_str, white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_rect = { WINDOW_WIDTH - 290, 70 - text_surface->h / 2,text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);

		char time_str[16];
		sprintf_s(time_str, sizeof(time_str), "%d:%d", game_stats->time_counter_min,game_stats->time_counter_sec);
		SDL_Color time_color = { 223,233,223,233 }; // para
		SDL_Surface* text_surface1 = TTF_RenderText_Solid(font50, time_str, time_color);
		SDL_Texture* text_texture1 = SDL_CreateTextureFromSurface(renderer, text_surface1);
		SDL_Rect text_rect1 = { WINDOW_WIDTH - 90, 200 - text_surface1->h / 2,text_surface1->w,text_surface1->h };
		SDL_RenderCopy(renderer, text_texture1, NULL, &text_rect1);
		SDL_DestroyTexture(text_texture1);
		SDL_FreeSurface(text_surface1);

		SDL_Color pause_color = { 252,215,222,255 }; // pause
		text_surface = TTF_RenderText_Solid(font50, "P For Pause ", pause_color);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { 70, 70 - text_surface->h / 2, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
		// Puanı metin olarak biçimlendirme
		///////HARİTA KAYDIRMA başlangıç

		SDL_Rect life_bar_rect_back;
		life_bar_rect_back.x = 20;
		life_bar_rect_back.y = WINDOW_HEIGHT - 50;
		life_bar_rect_back.w = life_bar_width;
		life_bar_rect_back.h = life_bar_height;

		SDL_Rect life_bar_rect_front;
		life_bar_rect_front.x = 20 +10;
		life_bar_rect_front.y = WINDOW_HEIGHT - 50 +10;
		life_bar_rect_front.w = (life_bar_width) - (man->max_life - man->current_life) * (life_bar_width/ man->max_life)- 20;
		life_bar_rect_front.h = life_bar_height-20;

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderFillRect(renderer, &life_bar_rect_back);
		SDL_SetRenderDrawColor(renderer, 250, 40, 40, 255);
		SDL_RenderFillRect(renderer, &life_bar_rect_front);


		float delta_time_renderer = (SDL_GetTicks() - last_frame_time_renderer) / 1000.0f; // burada fps ile ilgili ayarlar var
		last_frame_time_renderer = SDL_GetTicks();

		// map 1 başlangıç 
		const Uint8* durum = SDL_GetKeyboardState(NULL);
		

			///////HARİTA KAYDIRMA bitiş
		//}
	}
	if (gui_state->page == 9) {
		SDL_Color white = { 222,0,22,233 };
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Paused ", white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2,  WINDOW_HEIGHT / 2 - text_surface->h / 2, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
	}
	if (gui_state->page == 8) {

		SDL_SetRenderDrawColor(renderer, 20, 200, 100, 105);
		SDL_Rect upgrade_back_rect = { 30,WINDOW_HEIGHT / 4,WINDOW_WIDTH - 30,WINDOW_HEIGHT / 2 };
		SDL_RenderFillRect(renderer, &upgrade_back_rect);

		SDL_Color white = { 222,0,22,233 };
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "UPGRADE TIME ", white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2,  300, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);

		text_surface = TTF_RenderText_Solid(font50, "1-> range (+1) ", white);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_up1 = { WINDOW_WIDTH / 6 - text_surface->w / 2,  520, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_up1);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
	}



	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
	SDL_Window* window;  //window tanımlama
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer* renderer; //renderer tanımlama
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);





	///müzikleri yükleme başlangıç
	SDL_Init(SDL_INIT_EVERYTHING); // sdl'i çalıştırıyor 
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk* shootEffect = Mix_LoadWAV("sounds/laserShoot.wav"); // ses yükledim
	Mix_Chunk* hitEffect = Mix_LoadWAV("sounds/hitHurt.wav"); // ses yükledim
	///müzikleri yükleme bitiş

	///resim yükleme başlangıç
	// ////////////////////////empty
	///resim yükleme bitiş

	///yazı YÜKLEME başlangıç
	TTF_Init();
	TTF_Font* font50 = TTF_OpenFont("josefin.ttf", 38); // Font dosyasının adını ve boyutunu ayarlayın
	///yazı yükleme bitiş


	//enson = SDL_GetTicks();
	Man man;
	man.size = 40;
	man.x = WINDOW_WIDTH / 2 - man.size;
	man.y = WINDOW_HEIGHT / 2 - man.size;
	man.puan = 0;
	man.current_life = 100;
	man.max_life = 100;
	man.range = 230;
	man.money = 0;
	man.bullet_power = 1;
	man.bullet_speed = 10;
	man.move_speed = 200;
	man.idle_image_timer = 0;
	man.run_image_timer = 0;
	man.attack_image_timer = 0;
	man.idle_image_width_increment = 0;
	man.run_image_width_increment = 0;
	man.attack_image_width_increment = 0;
	man.attack_animation_time_holder = 0;
	man.animation = 1;
	man.direction = 0;
	man.direction_key = 0;
	man.x_holder = 0;
	man.y_holder = 0;
	





	Bullet mermiler[mermi_count];
	for (int n = 0; n < mermi_count; n++) {
		mermiler[n].y = 10000;
		mermiler[n].x = 10000;
		mermiler[n].width = 10;
		mermiler[n].height = 10;
		mermiler[n].speedx = 10;
		mermiler[n].speedy = 10;
		mermiler[n].biterken = 10;
		mermiler[n].life = 0;
	}


	Item itemler[100];
	for (int n = 0; n < 100; n++) {
		itemler[n].life = 0;
	}



	Enemy enemyler[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].y = spawnpoints.enemy_start_coord[n % 7][1];
		enemyler[n].x = spawnpoints.enemy_start_coord[n % 7][0];
		enemyler[n].width = 300;
		enemyler[n].height = 300;
		enemyler[n].current_life = 0;
		enemyler[n].max_life = 1;
		enemyler[n].speed = 70 + n * 4;
	}

	Game_stats game_stats;
	game_stats.time_counter_min = 0;
	game_stats.time_counter_sec = 0;
	game_stats.state = 0;

	GUI_State gui_state;
	gui_state.page = 1;




	Game_assets game_assets;
	SDL_Surface* map_image_surface = IMG_Load("images/maps/map1.png"); // resim.png dosyasının adını ve yolunu düzenleyin
	game_assets.map_image_texture = SDL_CreateTextureFromSurface(renderer, map_image_surface);
	SDL_FreeSurface(map_image_surface);

	SDL_Surface* enemy1_image_surface = IMG_Load("images/monster_images/Goblin/Run.png");
	game_assets.enemy1_image_texture = SDL_CreateTextureFromSurface(renderer, enemy1_image_surface);
	SDL_FreeSurface(enemy1_image_surface);

	SDL_Surface* protogonist_image_surface1 = IMG_Load("images/protog/Run.png");
	game_assets.protogonist_texture_run = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface1);
	SDL_FreeSurface(protogonist_image_surface1);

	SDL_Surface* protogonist_image_surface2 = IMG_Load("images/protog/Idle.png");
	game_assets.protogonist_texture_idle = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface2);
	SDL_FreeSurface(protogonist_image_surface2);

	SDL_Surface* protogonist_image_surface3 = IMG_Load("images/protog/Attack2.png");
	game_assets.protogonist_texture_attack = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface3);
	SDL_FreeSurface(protogonist_image_surface3);

	Mix_Music* background = Mix_LoadWAV("sounds/backgro.wav");




	int done = 0;
	//Mix_PlayChannel(1, background, -1);
	while (!done) {
		done = processEvents(window, &gui_state, &game_stats, &man, &mermiler, &enemyler, hitEffect, shootEffect, &itemler);
		//render display
		doRender(renderer, &gui_state, &game_stats, &man, &mermiler, &enemyler, font50, &game_assets, &itemler);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
