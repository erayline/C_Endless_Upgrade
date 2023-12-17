#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "mystructs.h"
#include "fonks.h"
#include <time.h>

//#include <string.h>
typedef struct {
	int enemy_start_coord[16][2]; // 8 adet düşmanın başlangıç koordinatları
} SpawnPoints;

SpawnPoints spawnpoints = {
	{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};

SDL_Rect map1 = { 0,0,MAP_WIDTH,MAP_HEIGHT};


/*  yapılacaklar :

	yapmakta olduklarım:

   yapılanlar:


*/


int last_spawn = 1;
int atack_speed = 400;
int bullet_speed_interval = 500;
int last_frame_time = 0;
float bullet_x_speed = 0;
float bullet_y_speed = 0; // burada birsürü gereksiz şey tanımlamışım bir ara azaltıcam.
int isPressed_left = 0;
int isPressed_right = 0;
int animation_control_timer = 0;
int processEvents(SDL_Window* window, GUI_State* gui_state, Game_stats* game_stats, Man* man, Bullet* mermiler, Enemy* enemyler, Mix_Chunk* hitEffect, Mix_Chunk* shootEffect, Item* itemler) { // burada işlemler oluyor

	int done = 0;
	SDL_Event event;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; // burada fps ile ilgili ayarlar var
	last_frame_time = SDL_GetTicks();


	game_stats->time_counter_sec = SDL_GetTicks() / 1000;
	if (game_stats->time_counter_sec > 60) {
		game_stats->time_counter_min = game_stats->time_counter_sec / 60;
		game_stats->time_counter_sec = game_stats->time_counter_sec % 60;
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
		
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				isPressed_left = 1;
				break;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				isPressed_right = 1;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				isPressed_left = 0;
				break;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				isPressed_right = 0;
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {

			case (SDLK_ESCAPE):
				done = 1;
				break;
			case SDLK_SPACE: //denemek için
				man->current_life = man->max_life;
				gui_state->page = 1;
				break;
			case SDLK_TAB:
				item_bar_active = 1;
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
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_TAB:
				item_bar_active = 0;
				break;

			default:
				break;
			}
			break;
		case SDL_QUIT:
			SDL_DestroyWindow(window);
			break;
		}
		break;
	}


	if (gui_state->page == 0) {

	}
	if (gui_state->page == 1) {

		// ana karakter hasar tespit  solüst sağüst solaşağı sağaşağı

		if (collision_man_enemy()) {
			if (1000 < SDL_GetTicks() - last_collision_man) {
				last_collision_man = SDL_GetTicks();
				man->current_life -= 10;
				if (man->current_life <= 0) {
					man->current_life = 0;
					gui_state->page = 0;
				}
			}
		}

		
		
		for (int n = 0; n < 16; n++) {

			if (n / 4 == 0) {
				spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH/4 + n*(WINDOW_WIDTH/2);
				spawnpoints.enemy_start_coord[n][1] = camera.y - 500;
			}
			if (n / 4 == 1) {
				spawnpoints.enemy_start_coord[n][0] = camera.x + camera.w + WINDOW_WIDTH/4;
				spawnpoints.enemy_start_coord[n][1] = camera.y - WINDOW_HEIGHT / 4 + (n-4) * (WINDOW_HEIGHT / 2);
			}
			if (n / 4 == 2) {
				spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH / 4 + (n-8) * (WINDOW_WIDTH / 2);
				spawnpoints.enemy_start_coord[n][1] = camera.y + camera.w + 500;
			}
			if (n / 4 == 3) {
				spawnpoints.enemy_start_coord[n][0] = camera.x - WINDOW_WIDTH / 4;
				spawnpoints.enemy_start_coord[n][1] = camera.y - WINDOW_HEIGHT / 4 + (n-12) * (WINDOW_HEIGHT / 2);
			}

		}



		if (1040 < SDL_GetTicks() - man->attack_animation_time_holder) {
			if (40 < SDL_GetTicks() - animation_control_timer) {
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
		//enemylerin çarpışması bitiş

		//  2 & 3 -> 10 & 11 _>  2
		// eğer düşmanla aynı pixeldeyse hasar alsın, ve bir saniye invincible olsun

		int man_ortasiX = man->x + man->size / 2;
		int man_ortasiY = man->y + man->size / 2;  // sürekli adamın ortasını bulmaya çalıştığım için kısalttım bu şekilde

		


		collision_man_item();

		// hareket etme başlanıgç


		const Uint8* state = SDL_GetKeyboardState(NULL);


		if ((state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_RIGHT]) || (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN])) {
			if (state[SDL_SCANCODE_D]) {
				man->direction = 1;
				man->direction_key = 1;
				man->x += man->move_speed * delta_time * 0.8f;
			}
			if (state[SDL_SCANCODE_A]) {
				man->direction = 0;
				man->direction_key = 1;
				man->x -= man->move_speed * delta_time * 0.8f * 0.8f;
			}
			if (state[SDL_SCANCODE_W]) {
				man->y -= man->move_speed * delta_time * 0.8f * 0.8f;
			}
			if (state[SDL_SCANCODE_S]) {
				man->y += man->move_speed * delta_time * 0.8f;
			}
		}
		else {
			if (state[SDL_SCANCODE_D]) {
				man->direction = 1;
				man->direction_key = 1;
				man->x += man->move_speed * delta_time;
			}
			if (state[SDL_SCANCODE_A]) {
				man->direction = 0;
				man->direction_key = 1;
				man->x -= man->move_speed * delta_time * 0.8f;
			}
			if (state[SDL_SCANCODE_W]) {
				man->y -= man->move_speed * delta_time * 0.8f;
			}
			if (state[SDL_SCANCODE_S]) {
				man->y += man->move_speed * delta_time;
			}
		}

		// hareket etme bitiş
		upgrade_protog();
		SDL_GetMouseState(&mouse_position.x, &mouse_position.y);


		
		if (isPressed_left) {
			for (int i = 0; i < mermi_count; i++) {
				if (mermiler[i].life != 1) { // halihazırda ateşlenmemiş mermiyi ateşliyor
					if (attack_speed_while_loop < (SDL_GetTicks() - last_bullet_shot) ) { //burada iki saniyede bir ateş etsin diye ayarlamaya çalıştım 

						if (10 < (SDL_GetTicks() - mermiler[i].biterken)) {
							mermiler[i].x = man->x + (man->size) / 2 - (mermiler[i].width) / 2;
							mermiler[i].y = man->y + man->size / 2 - mermiler[i].height / 2;
							mermiler[i].life = 1;

							Mix_PlayChannel(3, shootEffect, 0);

							
							last_bullet_shot = SDL_GetTicks();
							man->animation = 3;
							man->attack_animation_time_holder = SDL_GetTicks();
							if (man_ortasiX < mouse_position.x + camera.x) {
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

		// bu kısım sağ tuşta bulunan yetenek olacak.
		//if (isPressed_left) { 
		//	for (int i = 0; i < mermi_count; i++) {
		//		if (mermiler[i].life != 1) { // halihazırda ateşlenmemiş mermiyi ateşliyor
		//			if (attack_speed_while_loop < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateş etsin diye ayarlamaya çalıştım 
		//				if (10 < (SDL_GetTicks() - mermiler[i].biterken)) {
		//					mermiler[i].x = man->x + (man->size) / 2 - (mermiler[i].width) / 2;
		//					mermiler[i].y = man->y + man->size / 2 - mermiler[i].height / 2;
		//					mermiler[i].life = 1;
		//					Mix_PlayChannel(3, shootEffect, 0);
		//					last_bullet_shot = SDL_GetTicks();
		//					man->animation = 3;
		//					man->attack_animation_time_holder = SDL_GetTicks();
		//					if (man_ortasiX < mouse_position.x + camera.x) {
		//						man->attack_direction = 1;
		//					}
		//					else {
		//						man->attack_direction = 0;
		//					}
		//				}
		//			}
		//		}
		//	}
		//}




		float diagnal = sqrt(pow(((mouse_position.x + camera.x) - (man->x + man->size / 2)), 2) + pow(((mouse_position.y + camera.y) - (man->y + man->size / 2)), 2));
		float cosforx = (man_ortasiX - (mouse_position.x + camera.x)) / diagnal;
		float sinfory = (man_ortasiY - (mouse_position.y + camera.y)) / diagnal;

		speed_constant = 30 / sqrt((cosforx * delta_time * cosforx * delta_time) + (sinfory * delta_time * sinfory * delta_time));

		for (int i = 0; i < mermi_count; i++) {

			if (mermiler[i].life <= 0) {
				mermiler[i].speedx = cosforx * speed_constant * delta_time;
				mermiler[i].speedy = sinfory * speed_constant * delta_time;

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

		}

		/// mermi fonksiyonları bitiş


		//enemy güncelleme başlangıç
		// spawn olacakların sayısını tanımladığın bir değişkenin olabilir.


		int idd = 0;
		if (3000 < SDL_GetTicks() - last_spawn) {
			last_spawn = SDL_GetTicks();

			// enemy born
			for (int n = 0; n < enemy_count; n++) {

				if (enemyler[n].current_life < 1 && idd < 70) {
					enemyler[n].y = spawnpoints.enemy_start_coord[n % 16][1];
					enemyler[n].x = spawnpoints.enemy_start_coord[n % 16][0];
					enemyler[n].width = 100;
					enemyler[n].height = 100;
					enemyler[n].max_life = 2;
					enemyler[n].current_life = enemyler[n].max_life;
					enemyler[n].speed = 100;
					if (rand() % 10 > 5) {
						enemyler[n].spiece = 1;
					}
					else{
						enemyler[n].spiece = 2;
					}
					if (n % 7 == 0) {
						enemyler[n].width = 200;
						enemyler[n].speed = 75;
						enemyler[n].height = 200;
						enemyler[n].max_life = 5;
						enemyler[n].current_life = enemyler[n].max_life;
					}
					if (n % 50 == 0) {
						enemyler[n].speed = 50;
						enemyler[n].width = 500;
						enemyler[n].height = 500;
						enemyler[n].max_life =10;
						enemyler[n].current_life = enemyler[n].max_life;
					}
					enemyler[n].enemy_walking_last_time = 0;
					enemyler[n].enemy1_walking_image_width = 0;
					enemyler[n].enemy2_walking_image_width = 0;
					last_spawn = SDL_GetTicks();
					idd++;

				}
			}
		}



		for (int n = 0; n < enemy_count; n++) {

			int tutucu = collision_kontrol_counter;


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



			if (enemyler[n].current_life != enemyler[n].max_life) {
				enemyler[n].life_bar_active = 1;
			}

			if (enemyler[n].current_life == enemyler[n].max_life) {
				enemyler[n].life_bar_active = 0;
			}


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

						if (enemyler[n].current_life <= 0) { // eğer atışın adamı öldürürse aşağıdaki fonksiyonlar çalışacak.
							man->money += 10;

							drop_item(n);
						}// mermi için collision detection

						mermiler[i].life = 0;
						//Mix_PlayChannel(-1, hitEffect, 0);
						mermiler[i].biterken = SDL_GetTicks();
					}
				}
			}
			//düşman güncelleme bitiş
		}

		camera.x = man->x + man->size / 2 - (camera.w / 2);
		camera.y = man->y + man->size / 2 - (camera.h / 2);

	} // bu gui'nin

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
				
			SDL_Rect player_on_screen = { man->x - camera.x-90,man->y - camera.y - 100, man->size*3,man->size*3};
			SDL_Rect protogonist_image_rect = { 0 + man->idle_image_width_increment, 0, 250, 250 };
		
			if (100<SDL_GetTicks() - man->idle_image_timer) {
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

		if (man->animation == 2) {
			SDL_Rect man_on_screen1 = { man->x - camera.x-90,man->y  -camera.y -100, man->size*3,man->size*3 };
			SDL_Rect protogonist_image_rect1 = { 0 + man->run_image_width_increment, 0, 250, 250 };

			if (90 < SDL_GetTicks() - man->run_image_timer) {
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


		if (man->animation == 3) {			
			SDL_Rect rect2 = { man->x -camera.x-90,man->y -camera.y - 100, man->size*3,man->size*3 };
			SDL_Rect protogonist_image_rect2 = {  0+ man->attack_image_width_increment, 0, 250, 250 };

			if (90 < SDL_GetTicks() - man->attack_image_timer) {
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


		SDL_Rect item_example[100];
		for (int n = 0; n < 100; n++) {
			if (itemler[n].life == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				item_example[n].x = itemler[n].x - camera.x + itemler[n].width / 2;
				item_example[n].y = itemler[n].y - camera.y + itemler[n].height / 2;
				item_example[n].w = itemler[n].width *2;
				item_example[n].h = itemler[n].height *2;
				SDL_RenderCopy(renderer, itemler[n].item_texture, NULL, &item_example[n]);
			}
		}



		// protogonist çizdirme bitiş //

		SDL_Rect mermiler_1[mermi_count];
		for (int n = 0; n < mermi_count; n++) {
			if (mermiler[n].life == 1) {
				mermiler_1[n].x = mermiler[n].x;
				mermiler_1[n].y = mermiler[n].y;
				mermiler_1[n].w = mermiler[n].width;
				mermiler_1[n].h = mermiler[n].height;
			}
			if (mermiler[n].life != 0) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_Rect bullet_1 = { mermiler[n].x - camera.x,mermiler[n].y - camera.y,mermiler[n].width,mermiler[n].height };
				SDL_RenderFillRect(renderer, &bullet_1);
			}
		}	// mermi katmanı bitiş






		// bar oluşturma


		SDL_Rect enemy_1[enemy_count];
		SDL_Rect enemy_2[enemy_count];
		SDL_Rect bar_rect_back[enemy_count];
		SDL_Rect bar_rect_front[enemy_count];

		//enemy print etme başlangıç
		for (int n = 0; n < enemy_count; n++) {

			if (enemyler[n].spiece == 1) {
				enemy_1[n].x = enemyler[n].x - camera.x - 8;
				enemy_1[n].y = enemyler[n].y - camera.y;
				enemy_1[n].w = enemyler[n].width;
				enemy_1[n].h = enemyler[n].height;
			}

			if (enemyler[n].spiece == 2) {
				enemy_2[n].x = enemyler[n].x - camera.x - 8;
				enemy_2[n].y = enemyler[n].y - camera.y;
				enemy_2[n].w = enemyler[n].width;
				enemy_2[n].h = enemyler[n].height;
			}

			enemy_life_bar_width = enemy_life_bar_width + enemyler[n].width / 2;

			bar_rect_back[n].w = enemy_life_bar_width;
			bar_rect_back[n].h = enemy_life_bar_height;
			bar_rect_back[n].x = (enemyler[n].x + enemyler[n].width / 2 - enemy_life_bar_width / 2) - camera.x;
			bar_rect_back[n].y = enemyler[n].y - camera.y;

			bar_rect_front[n].w = (enemy_life_bar_width - 10) - (enemyler[n].max_life - enemyler[n].current_life) * (enemy_life_bar_width / enemyler[n].max_life);
			bar_rect_front[n].h = enemy_life_bar_height - 10;
			bar_rect_front[n].x = (enemyler[n].x + enemyler[n].width / 2 - enemy_life_bar_width / 2) - camera.x + 5;
			bar_rect_front[n].y = enemyler[n].y - camera.y + 5;
			enemy_life_bar_width = 30;

			if ((enemyler[n].current_life > 0) && (enemyler[n].life_bar_active)) {

				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_RenderFillRect(renderer, &bar_rect_back[n]);
				SDL_SetRenderDrawColor(renderer, 250, 40, 40, 255);
				SDL_RenderFillRect(renderer, &bar_rect_front[n]);

			}

			if (enemyler[n].current_life > 0) {

				float diagnal_enem = sqrt(pow(man->x + man->size / 2 - enemyler[n].x - enemyler[n].width / 2, 2) + pow(man->y + man->size / 2 - enemyler[n].y - enemyler[n].width / 2, 2));
				float cosforx_enem = (man->x + man->size / 2 - enemyler[n].x - enemyler[n].width / 2) / diagnal_enem;
				float sinfory_enem = (man->y + man->size / 2 - enemyler[n].y - enemyler[n].height / 2) / diagnal_enem;
				if (cosforx_enem > 0) {
					if (enemyler[n].spiece == 1) {
						if (100 < SDL_GetTicks() - enemyler[n].enemy_walking_last_time) {
							enemyler[n].enemy1_walking_image_width += 150;
							enemyler[n].enemy_walking_last_time = SDL_GetTicks();
							if (enemyler[n].enemy1_walking_image_width > 1050) {
								enemyler[n].enemy1_walking_image_width = 0;
							}
						}

						SDL_Rect enemy1_image_rect = { 50 + enemyler[n].enemy1_walking_image_width,50,50,50 };
						SDL_RenderCopy(renderer, game_assets->enemy1_run_image_texture, &enemy1_image_rect, &enemy_1[n]);
					}

					if (enemyler[n].spiece == 2) {
						if (200 < SDL_GetTicks() - enemyler[n].enemy_walking_last_time) {
							enemyler[n].enemy2_walking_image_width += 150;
							enemyler[n].enemy_walking_last_time = SDL_GetTicks();
							if (enemyler[n].enemy2_walking_image_width > 599) {
								enemyler[n].enemy2_walking_image_width = 0;
							}
						}
						SDL_Rect enemy2_image_rect = { 50 + enemyler[n].enemy2_walking_image_width,50,50,50 };
						SDL_RenderCopy(renderer, game_assets->enemy2_run_image_texture, &enemy2_image_rect, &enemy_2[n]);
					}
				
				}
				else {
					if (enemyler[n].spiece == 1) {
						if (100 < SDL_GetTicks() - enemyler[n].enemy_walking_last_time) {
							enemyler[n].enemy1_walking_image_width += 150;
							enemyler[n].enemy_walking_last_time = SDL_GetTicks();
							if (enemyler[n].enemy1_walking_image_width > 1050) {
								enemyler[n].enemy1_walking_image_width = 0;
							}
						}
						SDL_Rect enemy1_image_rect = { 50 + enemyler[n].enemy1_walking_image_width,50,50,50 };
						SDL_RenderCopyEx(renderer, game_assets->enemy1_run_image_texture, &enemy1_image_rect, &enemy_1[n], 0.0, NULL, SDL_FLIP_HORIZONTAL);
					}

					if (enemyler[n].spiece == 2) {

						if (200 < SDL_GetTicks() - enemyler[n].enemy_walking_last_time) {
							enemyler[n].enemy2_walking_image_width += 150;
							enemyler[n].enemy_walking_last_time = SDL_GetTicks();
							if (enemyler[n].enemy2_walking_image_width > 599) {
								enemyler[n].enemy2_walking_image_width = 0;
							}
						}
						SDL_Rect enemy2_image_rect = { 50 + enemyler[n].enemy2_walking_image_width,50,50,50 };
						SDL_RenderCopyEx(renderer, game_assets->enemy2_run_image_texture, &enemy2_image_rect, &enemy_2[n], 0.0, NULL, SDL_FLIP_HORIZONTAL);
					}
				}
			}
		}// enemy print etme bitiş

		if (item_bar_active) {
			SDL_SetRenderDrawColor(renderer, 180, 200, 170, 0);
			SDL_Rect left_items_bar = { 20,200,200,WINDOW_HEIGHT / 1.5f };
			SDL_RenderFillRect(renderer, &left_items_bar);
			SDL_Rect left_items_bar_items[10];
			for (int n = 0; n < 10; n++) {
				if (item_index_list[n] != 0) {
					left_items_bar_items[n].x = item_bar_x_y_list[n][0];
					left_items_bar_items[n].y = item_bar_x_y_list[n][1];
					left_items_bar_items[n].w = 60;
					left_items_bar_items[n].h = 60;
					switch (item_index_list[n])
					{
					case 1:
						bastirilacak_item = game_assets->item_1;
						break;
					case 2:
						bastirilacak_item = game_assets->item_2;
						break;
					case 3:
						bastirilacak_item = game_assets->item_3;
						break;
					default:
						break;
					}
					SDL_RenderCopy(renderer, bastirilacak_item, NULL, &left_items_bar_items[n]);
				}
			}
		}

		//ekrana yazı yazdırma başlangıç
		char puan_str[16];

		sprintf_s(puan_str, sizeof(puan_str), "Money: %4d", man->money);
		SDL_Color white = { 22,132,22,255 }; // para
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
		SDL_Rect text_start = { 20, 70 - text_surface->h / 2, text_surface->w,text_surface->h };
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
		life_bar_rect_front.w = (life_bar_width - 20) - (man->max_life - man->current_life) * ((life_bar_width - 20) / man->max_life);
		//life_bar_rect_front.w = (life_bar_width - 20) - (life_bar_width)
		life_bar_rect_front.h = life_bar_height-20;

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderFillRect(renderer, &life_bar_rect_back);
		SDL_SetRenderDrawColor(renderer, 250, 40, 40, 255);
		SDL_RenderFillRect(renderer, &life_bar_rect_front);

		char life_bar_str[20];
		sprintf_s(life_bar_str, sizeof(life_bar_str), "%d/%d", man->max_life, man->current_life);
		SDL_Color life_bar_color = { 3,3,3,3 }; // para
		text_surface1 = TTF_RenderText_Solid(font50, life_bar_str, life_bar_color);
		text_texture1 = SDL_CreateTextureFromSurface(renderer, text_surface1);
		SDL_Rect text_rect3 = { 20 + 10, WINDOW_HEIGHT - 50 + 10,text_surface1->w,text_surface1->h /1.5f};
		SDL_RenderCopy(renderer, text_texture1, NULL, &text_rect3);
		SDL_DestroyTexture(text_texture1);
		SDL_FreeSurface(text_surface1);

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

	}



	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
	SDL_Window* window;  //window tanımlama
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer* renderer; //renderer tanımlama
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);


	for (int n = 0; n < 16; n++) {
		spawnpoints.enemy_start_coord[n][0] = 500 + ((MAP_WIDTH - 1000) / 16) * n;
		spawnpoints.enemy_start_coord[n][1] = 300;
	}


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
	man.size = 100;
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

	man.attack_speed = 3;
	



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


	for (int n = 0; n < 100; n++) {
		itemler[n].life = 0;
	}

	owned_items.CursedGlove_c = 0;


	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].y = spawnpoints.enemy_start_coord[n % 5][1];
		enemyler[n].x = spawnpoints.enemy_start_coord[n % 5][0];
		enemyler[n].width = 300;
		enemyler[n].height = 300;
		
		enemyler[n].current_life = 0;
		enemyler[n].max_life = 1;
		enemyler[n].speed = 100;
	}

	Game_stats game_stats;
	game_stats.time_counter_min = 0;
	game_stats.time_counter_sec = 0;
	game_stats.state = 0;

	GUI_State gui_state;
	gui_state.page = 1;




	SDL_Surface* map_image_surface = IMG_Load("images/maps/map1.png"); // resim.png dosyasının adını ve yolunu düzenleyin
	game_assets.map_image_texture = SDL_CreateTextureFromSurface(renderer, map_image_surface);
	SDL_FreeSurface(map_image_surface);

	SDL_Surface* enemy1_run_image_surface = IMG_Load("images/monster_images/Goblin/Run.png");
	game_assets.enemy1_run_image_texture = SDL_CreateTextureFromSurface(renderer, enemy1_run_image_surface);
	SDL_FreeSurface(enemy1_run_image_surface);

	SDL_Surface* enemy2_run_image_surface = IMG_Load("images/monster_images/Skeleton/Walk.png");
	game_assets.enemy2_run_image_texture = SDL_CreateTextureFromSurface(renderer, enemy2_run_image_surface);
	SDL_FreeSurface(enemy2_run_image_surface);

	SDL_Surface* protogonist_image_surface1 = IMG_Load("images/protog/Run.png");
	game_assets.protogonist_texture_run = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface1);
	SDL_FreeSurface(protogonist_image_surface1);

	SDL_Surface* protogonist_image_surface2 = IMG_Load("images/protog/Idle.png");
	game_assets.protogonist_texture_idle = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface2);
	SDL_FreeSurface(protogonist_image_surface2);

	SDL_Surface* protogonist_image_surface3 = IMG_Load("images/protog/Attack2.png");
	game_assets.protogonist_texture_attack = SDL_CreateTextureFromSurface(renderer, protogonist_image_surface3);
	SDL_FreeSurface(protogonist_image_surface3);



	SDL_Surface* lux_avize = IMG_Load("images/items/item_01.png");
	game_assets.item_1 = SDL_CreateTextureFromSurface(renderer, lux_avize);
	SDL_FreeSurface(lux_avize);

	SDL_Surface* wizard_hat = IMG_Load("images/items/item_02.png");
	game_assets.item_2 = SDL_CreateTextureFromSurface(renderer, wizard_hat);
	SDL_FreeSurface(wizard_hat);

	SDL_Surface* life_reminder = IMG_Load("images/items/item_03.png");
	game_assets.item_3 = SDL_CreateTextureFromSurface(renderer, life_reminder);
	SDL_FreeSurface(life_reminder);

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
