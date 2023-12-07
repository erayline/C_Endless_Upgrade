#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "mystructs.h"
//#include <string.h>




/*  yapılacaklar :
	//arka plan clash of clans köyü yap.
	//enum kullan image için araştırıp.
	//text oluşturmayı çok kullanacağın için fonksiyon falan oluştur. (eşyalara'lere fareyi sürükleyince, bölümü yazdırırken falan)
	//eşya oluştur sürü sürü
	
	yapmakta olduklarım:
	//arayüz oluşturmaya başla karakterler için gif kullan 


   yapılanlar:


	//ekrana yazılar yazdırmaya başla //done
	her üç saniyede bir düşman oluşsun düşman // dalgaları oluşsun // yaptım gibi gibi, demo için yeterli
	düşmanın her köşesini algılamıyor sol üst sağ alt algılıyor// çözdüm
	düşman hareketlerine mermi hareketi mekaniğini ekle. // çözüldü artık adamakıllı beni takip ediyorlar

*/


int upgrade_choice = 0;
float speed_constant;
int last_bullet_shot=10;
int yakinlik[enemy_count]; // bu yakınlık listesi. en sondaki eleman en yakındaki eleman oluyor.
int last_collision_man = 0;


int icindemi(int interval_left,int interval_right,int nokta_orta) {
	//if it is in it is one
	if ((nokta_orta > interval_left) && (nokta_orta<interval_right) ) {
		return 1;
	}else {
		return 0;
	}
}
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
int processEvents(SDL_Window* window, GUI_State* gui_state, Game_stats* game_stats, Man* man, Bullet* mermiler, Enemy* enemyler, Mix_Chunk* hitEffect, Mix_Chunk* shootEffect) { // burada işlemler oluyor

	int done = 0;
	SDL_Event event;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; // burada fps ile ilgili ayarlar var
	last_frame_time = SDL_GetTicks();




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
		for (int n = 0; n < enemy_count; n++) {
			if (  (icindemi(man->x,man->x+man->size,enemyler[n].x) && icindemi(man->y,man->y+man->size,enemyler[n].y)) || (icindemi(man->x,man->x+man->size,enemyler[n].x,enemyler[n].width) && icindemi(man->y, man->y + man->size,enemyler[n].y)) || (icindemi(man->x, man->x + man->size,enemyler[n].x) && icindemi(man->y, man->y + man->size, enemyler[n].y + enemyler[n].height )) || (icindemi(man->x, man->x + man->size,enemyler[n].x + enemyler[n].width) && (icindemi(man->y, man->y + man->size,enemyler[n].y + enemyler[n].width)))) {
				if (1000 < SDL_GetTicks() - last_collision_man) {
					man->life -= 1;
					last_collision_man = SDL_GetTicks();
					if (man->life <= 0) {
						gui_state->page = 0;
					}
					
				}
			}
		}

		for (int n = 0; n < enemy_count; n++) {
			for (int i = 0; i < enemy_count; i++) {

				if (n != i) { //solüst sağüst solalt sağalt
					int enemXR = enemyler[n].x + enemyler[n].width;
					int enemYR = enemyler[n].y + enemyler[n].height;
					if ((icindemi(enemyler[n].x, enemXR, enemyler[i].x)						&& icindemi(enemyler[n].y, enemYR, enemyler[i].y)) ||
						(icindemi(enemyler[n].x, enemXR, enemyler[i].x + enemyler[i].width) && icindemi(enemyler[n].y, enemYR, enemyler[i].y)) ||
						(icindemi(enemyler[n].x, enemXR, enemyler[i].x)						&& icindemi(enemyler[n].y, enemYR, enemyler[i].y + enemyler[i].height)) ||
						(icindemi(enemyler[n].x, enemXR, enemyler[i].x + enemyler[i].width) && icindemi(enemyler[n].y, enemYR, enemyler[i].y + enemyler[i].height))
						) {
						int enemyNX = (enemyler[n].x + enemyler[n].width)	/2;
						int enemyNY = (enemyler[n].y + enemyler[n].height)	/2;
						int enemyIX = (enemyler[i].x + enemyler[i].width)	/2;
						int enemyIY = (enemyler[i].y + enemyler[n].height)	/2;
						if (enemyNX <= enemyIX) { 
							enemyler[i].x += 40 * delta_time;
						}
						if (enemyNX > enemyIX) {
							enemyler[i].x -= 40 * delta_time;
						}
						if (enemyNY <= enemyIY) {
							enemyler[i].y += 40 * delta_time;
						}
						if (enemyIY > enemyIY) {
							enemyler[i].y -= 40 * delta_time;
						}



					}
				}	

			}
		}
		
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

		

		if ((state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_RIGHT])  || (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN])) {
			if (state[SDL_SCANCODE_RIGHT]) {
				man->x += 280 * delta_time;
			}
			if (state[SDL_SCANCODE_LEFT]) {
				man->x -= 280 * delta_time;
			}
			if (state[SDL_SCANCODE_UP]) {
				man->y -= 280 * delta_time;
			}
			if (state[SDL_SCANCODE_DOWN]) {
				man->y += 280 * delta_time;
			}
		}
		else {
			if (state[SDL_SCANCODE_RIGHT]) {
				man->x += 400 * delta_time;
			}
			if (state[SDL_SCANCODE_LEFT]) {
				man->x -= 400 * delta_time;
			}
			if (state[SDL_SCANCODE_UP]) {
				man->y -= 400 * delta_time;
			}
			if (state[SDL_SCANCODE_DOWN]) {
				man->y += 400 * delta_time;
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

			

			if (mermiler[i].y < 0 || mermiler[i].x < 0 || mermiler[i].y > WINDOW_HEIGHT || mermiler[i].x >WINDOW_WIDTH) {
				mermiler[i].life = 0;
				mermiler[i].biterken = SDL_GetTicks();
			}
		}



		speed_constant = 500;
		float diagnal = uzaklikSirasi[enemy_count - 1][0];

		float cosforx = (man_ortasiX - enemyler[closest_enemy_index].x - enemyler[closest_enemy_index].width / 2) / diagnal;
		float sinfory = (man_ortasiY - enemyler[closest_enemy_index].y - enemyler[closest_enemy_index].height / 2) / diagnal;

		for (int i = 0; i < mermi_count; i++) {

			if (mermiler[i].life <= 0) {
				mermiler[i].speedx = cosforx * speed_constant * delta_time;
				mermiler[i].speedy = sinfory * speed_constant * delta_time;

				mermiler[i].y = 2000;
				mermiler[i].x = 2000;
			}
		}

		/// mermi fonksiyonları bitiş

		//if (man->money >= 100) {
		//	gui_state->page = 8;
		//}

		//enemy güncelleme başlangıç
		// spawn olacakların sayısını tanımladığın bir değişkenin olabilir.
		int idd = 0;
		if (3000 < SDL_GetTicks() - last_spawn) {
			printf("oldu bu iş");
			last_spawn = SDL_GetTicks();

			for (int n = 0; n < enemy_count; n++) {
				if (enemyler[n].life < 1 && idd<10) {
					enemyler[n].y = enemy_start_coord[n%8][1];
					enemyler[n].x = enemy_start_coord[n%8][0];
					enemyler[n].width = 70;
					enemyler[n].height = 70;
					enemyler[n].life = 3;
					enemyler[n].speed = 100;
					enemyler[n].enemy1_walking_last_time = 0;
					enemyler[n].enemy1_walking_image_width = 0;
					last_spawn = SDL_GetTicks();
					idd++;
				}
			}
		}


		for (int n = 0; n < enemy_count; n++) {

			float diagnal_enem = sqrt(pow(man_ortasiX - enemyler[n].x - enemyler[n].width / 2, 2) + pow(man_ortasiY - enemyler[n].y - enemyler[n].width / 2, 2));
			float cosforx_enem = (man_ortasiX - enemyler[n].x - enemyler[n].width / 2) / diagnal_enem;
			float sinfory_enem = (man_ortasiY - enemyler[n].y - enemyler[n].height / 2) / diagnal_enem;

			if (enemyler[n].life <= 0) {
				enemyler[n].y = -1000;
				enemyler[n].x = -1000;
				
			}
			else {

				if (enemyler[n].life != 0) { // enemylerin hızlarını ayarladığım bölüm
					if (enemyler[n].y + enemyler[n].height / 2 != man->y + man->size / 2) {
						enemyler[n].y += sinfory_enem * enemyler[n].speed * delta_time;
					}
					if (enemyler[n].x + enemyler[n].width / 2 != man->x + man->size / 2) {
						enemyler[n].x += cosforx_enem * enemyler[n].speed * delta_time;
					}
				}


				for (int i = 0; i < mermi_count; i++) {
					if ((((enemyler[n].x < mermiler[i].x) && (enemyler[n].x + enemyler[n].width > mermiler[i].x)) || ((enemyler[n].x < mermiler[i].x + mermiler[i].width) && (enemyler[n].x + enemyler[n].width > mermiler[i].x + mermiler[i].width))) && (((enemyler[n].y < mermiler[i].y) && (enemyler[n].y + enemyler[n].height > mermiler[i].y)) || ((enemyler[n].y < mermiler[i].y + mermiler[i].height) && (enemyler[n].y + enemyler[n].height > mermiler[i].y + mermiler[i].height)))) {
						enemyler[n].life -= man->bullet_power;
						if (enemyler[n].life <= 0) {
							man->money += 10;
						}// mermi için collision detection
						mermiler[i].life = 0;
						Mix_PlayChannel(-1, hitEffect, 0);
						mermiler[i].biterken = SDL_GetTicks();
					}
				}
			}
			//düşman güncelleme bitiş
		}
		SDL_Rect kamera = { 40,40,1200,800 };
	} // bu gui'nin

	if (gui_state->page == 9) {

	}

	//if (gui_state->page == 8) { // upgrade page
	//	//while (&SDL_PollEvent) {
	//		SDL_Delay(20);
	//		switch (event.type){
	//		case SDL_KEYDOWN:
	//			switch (event.key.keysym.sym)
	//			{
	//			case SDLK_1:
	//				man->range += 50;
	//				gui_state->page = 1;
	//				man->money -= 100;
	//				break;
	//			case SDLK_2:
	//				man->bullet_speed += 25;
	//				if (man->bullet_speed > 499) {
	//					man->bullet_speed = 499;
	//				}
	//				gui_state->page = 1;
	//				man->money -= 100;
	//				break;
	//			case SDLK_3:
	//				man->bullet_power += 1;
	//				gui_state->page = 1;
	//				man->money -= 100;
	//				break;
	//			default:
	//				break;
	//			}
	//		default:
	//			break;
	//		}
	//	//}
	//}
	return done;
}


int enson=0;
void doRender(SDL_Renderer* renderer,GUI_State* gui_state,Game_stats* game_stats, Man* man, Bullet* mermiler, Enemy* enemyler, TTF_Font* font50,Game_assets* game_assets){
	
	if (gui_state->page == 0) {

		SDL_Color white = { 252,215,222,255 };
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Welcome to the COLLECTOR", white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_welcome = { WINDOW_WIDTH/2 - text_surface->w/2, WINDOW_HEIGHT/2 -text_surface->h/2, text_surface->w,text_surface->h};
		SDL_RenderCopy(renderer, text_texture, NULL, &text_welcome);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);


		text_surface = TTF_RenderText_Solid(font50, "press SPACE to start ", white);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { WINDOW_WIDTH/2 - text_surface->w/2, WINDOW_HEIGHT - text_surface->h - 50, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);



	}
	if (gui_state->page == 1) {

		SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
		SDL_RenderClear(renderer); // arkaplan katmanı



		// protogonist çizdirme başlangıç //
		SDL_SetRenderDrawColor(renderer, 20, 204, 84, 255);
		SDL_Rect rect = { man->x,man->y, man->size,man->size };
		SDL_RenderFillRect(renderer, &rect); // protogonist katmanı bitiş
		SDL_SetRenderDrawColor(renderer, 10, 80, 30, 200);
		SDL_Rect man_range_rect = { man->x + man->size / 2 - man->range ,man->y + man->size / 2 - man->range, man->range * 2, man->range * 2 };
		SDL_RenderDrawRect(renderer, &man_range_rect); // protogonist menzili bitiş
		// protogonist çizdirme bitiş //


		for (int n = 0; n < mermi_count; n++) {
			if (mermiler[n].life != 0) {

				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_Rect bullet_1 = { mermiler[n].x,mermiler[n].y,mermiler[n].width,mermiler[n].height };
				SDL_RenderFillRect(renderer, &bullet_1);

			}
		}
		SDL_Rect mermiler_1[mermi_count];
		for (int n = 0; n < mermi_count; n++) {
			mermiler_1[n].x = mermiler[n].x;
			mermiler_1[n].y = mermiler[n].y;
			mermiler_1[n].w = mermiler[n].width;
			mermiler_1[n].h = mermiler[n].height;
		}	// mermi katmanı bitiş



		SDL_Rect enemy_1[enemy_count];
		SDL_Rect enemy_1_dec[enemy_count];
		for (int n = 0; n < enemy_count; n++) {
			enemy_1[n].x = enemyler[n].x;
			enemy_1[n].y = enemyler[n].y;
			enemy_1[n].w = enemyler[n].width;
			enemy_1[n].h = enemyler[n].height;

			enemy_1_dec[n].x = enemyler[n].x + enemyler[n].width / 4;
			enemy_1_dec[n].y = enemyler[n].y + enemyler[n].height / 4;
			enemy_1_dec[n].w = enemyler[n].width - enemyler[n].width / 2;
			enemy_1_dec[n].h = enemyler[n].height - enemyler[n].height / 2;
		}


		//enemy print etme başlangıç
		for (int n = 0; n < enemy_count; n++) { 

			if (enemyler[n].life != 0) {
				if (  100 < SDL_GetTicks() - enemyler[n].enemy1_walking_last_time) {
					enemyler[n].enemy1_walking_image_width += 150;
					enemyler[n].enemy1_walking_last_time = SDL_GetTicks();
					if (enemyler[n].enemy1_walking_image_width > 1050) {
						enemyler[n].enemy1_walking_image_width = 0;
					}
				}
				SDL_Rect enemy1_image_rect = { 50 + enemyler[n].enemy1_walking_image_width,50,50,50};
				SDL_RenderCopy(renderer, game_assets->enemy1_image_texture, &enemy1_image_rect, &enemy_1[n]);

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

		SDL_Color pause_color = { 252,215,222,255 }; // pause
		text_surface = TTF_RenderText_Solid(font50, "P For Pause ", pause_color);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { 70, 70 - text_surface->h / 2, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);

		char can_str[16];
		sprintf_s(can_str, sizeof(can_str), "life: %d",man->life);
		SDL_Color can_color = { 222,32,22,233 }; // para
		SDL_Surface* can_text_surface = TTF_RenderText_Solid(font50, can_str, can_color);
		SDL_Texture* can_text_texture = SDL_CreateTextureFromSurface(renderer, can_text_surface);
		SDL_Rect can_text_rect = { 30, WINDOW_HEIGHT- 30 - can_text_surface->h,can_text_surface->w,can_text_surface->h };
		SDL_RenderCopy(renderer, can_text_texture, NULL, &can_text_rect);
		SDL_DestroyTexture(can_text_texture);
		SDL_FreeSurface(can_text_surface);
		// Puanı metin olarak biçimlendirme
		///////HARİTA KAYDIRMA başlangıç

		const Uint8* state = SDL_GetKeyboardState(NULL);
		int done = 0;
		float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; // burada fps ile ilgili ayarlar var
		last_frame_time = SDL_GetTicks();

		// map 1 //
		SDL_Rect map1 = { map_x,map_y,2000,2000 };
		SDL_RenderCopy(renderer, game_assets->map_image_texture, NULL, &map1);
		// map 1 bitiş //  

		//if ((state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_RIGHT]) || (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN])) {
		//	if (state[SDL_SCANCODE_RIGHT]) { // 280
		//		map_x -= man->move_speed;
		//	}
		//	if (state[SDL_SCANCODE_LEFT]) {
		//		map_x += man->move_speed;
		//	}
		//	if (state[SDL_SCANCODE_UP]) {
		//		map_y += man->move_speed;
		//	}
		//	if (state[SDL_SCANCODE_DOWN]) {
		//		map_y -= man->move_speed;
		//	}
		//}
		//else {
		//	if (state[SDL_SCANCODE_RIGHT]) { // 280
		//		map_x -= man->move_speed;
		//
		//	if (state[SDL_SCANCODE_LEFT]) {
		//		map_x += man->move_speed;
		//	}
		//	if (state[SDL_SCANCODE_UP]) {
		//		map_y += man->move_speed;
		//	}
		//	if (state[SDL_SCANCODE_DOWN]) {
		//		map_y -= man->move_speed;
		//	}
		//}


		///////HARİTA KAYDIRMA bitiş



	}
	if (gui_state->page == 9) {
		SDL_Color white = { 222,0,22,233 };
		SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Paused ", white);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2,  WINDOW_HEIGHT/2 -text_surface->h / 2, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
	}
	if (gui_state->page == 8) {

		SDL_SetRenderDrawColor(renderer, 20, 200, 100, 105);
		SDL_Rect upgrade_back_rect = {30,WINDOW_HEIGHT/4,WINDOW_WIDTH-30,WINDOW_HEIGHT/2 };
		SDL_RenderFillRect(renderer,&upgrade_back_rect);

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

		text_surface = TTF_RenderText_Solid(font50, "2-> attack speed(+1) ", white);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_up2 = { 3*WINDOW_WIDTH / 6 - text_surface->w / 2, 520, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_up2);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);

		text_surface = TTF_RenderText_Solid(font50, "3-> bullet power(+1) ", white);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_up3 = { 5*WINDOW_WIDTH / 6 - text_surface->w / 2,  520, text_surface->w,text_surface->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_up3);
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);

	}



	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
	SDL_Window* window;  //window tanımlama
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);
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
	TTF_Font* font50 = TTF_OpenFont("josefin.ttf", 50); // Font dosyasının adını ve boyutunu ayarlayın
	///yazı yükleme bitiş


	//enson = SDL_GetTicks();
	Man man;
	man.x = 720;
	man.y = 410;
	man.size = 35;
	man.puan = 0;
	man.life = 30;
	man.range = 230;
	man.money = 0;
	man.bullet_power = 1;
	man.bullet_speed = 10;
	man.move_speed = 400;

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



	Enemy enemyler[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].y = enemy_start_coord[n%7][1];
		enemyler[n].x = enemy_start_coord[n%7][0];
		enemyler[n].width = 300;
		enemyler[n].height = 300;
		enemyler[n].life = 0;
		enemyler[n].speed = 70+ n*4;
	}

	Game_stats game_stats;
	game_stats.time_counter = 0;
	game_stats.level = 0;

	GUI_State gui_state;
	gui_state.page = 0;


	Game_assets game_assets;
	SDL_Surface* map_image_surface = IMG_Load("images/map1.png"); // resim.png dosyasının adını ve yolunu düzenleyin
	game_assets.map_image_texture = SDL_CreateTextureFromSurface(renderer, map_image_surface);
	SDL_FreeSurface(map_image_surface);
	SDL_Surface* enemy1_image_surface = IMG_Load("images/monster_images/Goblin/Run.png");
	game_assets.enemy1_image_texture = SDL_CreateTextureFromSurface(renderer, enemy1_image_surface);
	SDL_FreeSurface(enemy1_image_surface);


	Mix_Music* background = Mix_LoadWAV("sounds/backgro.wav");




	int done = 0;
	//Mix_PlayChannel(1, background, -1);
	while (!done) {
		done = processEvents(window, &gui_state, &game_stats, &man, &mermiler, &enemyler, hitEffect, shootEffect);
		//render display
		doRender(renderer, &gui_state, &game_stats, &man, &mermiler, &enemyler, font50,&game_assets);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	return 0;
}
