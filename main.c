#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include "mystructs.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>


#define FPS = 30

#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 6

void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Surface** textSurface, SDL_Texture** textTexture, const char* newText) {
	// Free existing text resources if necessary
	if (*textSurface) {
		SDL_FreeSurface(*textSurface);
		*textSurface = NULL;
	}
	if (*textTexture) {
		SDL_DestroyTexture(*textTexture);
		*textTexture = NULL;
	}

	// Create new text surface with updated text
	SDL_Color white = { 255,255,255,255 };
	*textSurface = TTF_RenderText_Solid(font, newText, white);


	// Create new texture from updated text surface
	*textTexture = SDL_CreateTextureFromSurface(renderer, *textSurface);
}




int calculate_distance(int deger1, int deger2) { // iki sayının farkının mutlağını bulan fonksiyon tanımladım uzaklık bulurken falan kullanıcam.
	int sonuc = 0;
	return abs(deger1 - deger2);
}

int yakinlik[enemy_count]; // bu yakınlık listesi. en sondaki eleman en yakındaki eleman oluyor.

int ilk_buyukmu(int deger1, int deger2) {
	if (deger1 >= deger2) {
		return 1;
	}
	else {
		return 0;
	}
}

int atack_speed = 400;
int bullet_speed = 300;
int last_frame_time = 1;
int biterken = 0;
int bullet_x_speed = 0;
int bullet_y_speed = 0; // burada birsürü gereksiz şey tanımlamışım bir ara azaltıcam.
int processEvents(SDL_Window* window, Man* man, Bullet* mermi, Enemy* enemyler, Mix_Chunk* hitEffect, Mix_Chunk* shootEffect) { // burada işlemler oluyor

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








	SDL_Event event;
	int done = 0;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; // burada fps ile ilgili ayarlar var
	last_frame_time = SDL_GetTicks();







	// moving part
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
				mermi->life = 0;
				break;
			}
			break;
		case SDL_QUIT:
			SDL_DestroyWindow(window);
			break;
		}
		break;
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	//mermi fonksiyonları



	for (int n = 0; n < enemy_count; n++) { //burada range içine giren eleman olursa ateş etsin diye ayarladım

		if (((((enemyler[n].x > man->x - man->range) && (enemyler[n].x < man->x + man->range)) && ((enemyler[n].y > man->y - man->range) && (enemyler[n].y < man->y + man->range))) || (((enemyler[n].x + enemyler[n].width > man->x - man->range) && (enemyler[n].x + enemyler[n].width < man->x + man->range)) && ((enemyler[n].y + enemyler[n].height > man->y - man->range) && (enemyler[n].y + enemyler[n].height < man->y + man->range))))) {

			if (mermi->life != 1) { // halihazırda ateşlenmemiş mermiyi ateşliyor
				if (atack_speed < (SDL_GetTicks() - biterken)) { //burada iki saniyede bir ateş etsin diye ayarlamaya çalıştım 
					mermi->x = man->x + (man->size) / 2 - (mermi->width) / 2;
					mermi->y = man->y + man->size / 2 - mermi->width/2;
					mermi->life = 1;
					Mix_PlayChannel(-1, shootEffect, 0);

				}
				else {
					//printf("mermi zaamanı gelmedi");
				}
			}

		}


	}


	if (mermi->life == 1) {
		mermi->x -= mermi->speedx;
		mermi->y -= mermi->speedy;
	}



	if (mermi->y < 0) {
		mermi->life = 0;
		biterken = SDL_GetTicks();
	}
	float speed_constant = 300;
	float diagnal = uzaklikSirasi[enemy_count - 1][0];

	float cosforx = (man_ortasiX - enemyler[closest_enemy_index].x - enemyler[closest_enemy_index].width / 2) / diagnal;
	float sinfory = (man_ortasiY - enemyler[closest_enemy_index].y - enemyler[closest_enemy_index].height / 2) / diagnal;
	if (mermi->life <= 0) {

		mermi->speedx =cosforx * speed_constant * delta_time;
		mermi->speedy = sinfory * speed_constant * delta_time;
		
		mermi->y = 2000;
		mermi->x = 2000;

	}



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

	for (int n = 0; n < enemy_count; n++) {

		if (enemyler[n].life <= 0) {
			enemyler[n].y = -1000;
			enemyler[n].x = -1000;
		}
		else {
			if (enemyler[n].life != 0) { // enemylerin hızlarını ayarladığım bölüm
				if (enemyler[n].y + enemyler[n].height / 2 > man->y + man->size / 2) {
					enemyler[n].y -= enemyler[n].speed * delta_time;
				}
				if (enemyler[n].y + enemyler[n].height / 2 < man->y + man->size / 2) {
					enemyler[n].y += enemyler[n].speed * delta_time;
				}

				if (enemyler[n].x + enemyler[n].width / 2 > man->x + man->size / 2) {
					enemyler[n].x -= enemyler[n].speed * delta_time;
				}
				if (enemyler[n].x + enemyler[n].width / 2 < man->x + man->size / 2) {
					enemyler[n].x += enemyler[n].speed * delta_time;
				}
			}



			if ((((enemyler[n].x < mermi->x) && (enemyler[n].x + enemyler[n].width > mermi->x)) || ((enemyler[n].x < mermi->x + mermi->width) && (enemyler[n].x + enemyler[n].width > mermi->x + mermi->width))) && (((enemyler[n].y < mermi->y) && (enemyler[n].y + enemyler[n].height > mermi->y)) || ((enemyler[n].y < mermi->y + mermi->height) && (enemyler[n].y + enemyler[n].height > mermi->y + mermi->height)))) {
				enemyler[n].life -= 1;
				man->puan += 1; // mermi için collision detection
				mermi->life = 0;
				Mix_PlayChannel(-1, hitEffect, 0);
				biterken = SDL_GetTicks();
			}

		}



	}

	return done;
}



void doRender(SDL_Renderer* renderer, Man* man, Bullet* mermi, Enemy* enemyler) {

	SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
	SDL_RenderClear(renderer);


	SDL_SetRenderDrawColor(renderer, 20, 204, 84, 255);
	SDL_Rect rect = { man->x,man->y, man->size,man->size };
	SDL_RenderFillRect(renderer, &rect); // burada içini dolduracağımız cismin adresine atıfta bulunuyoruz.

	SDL_Rect man_range_rect = { man->x + man->size / 2 - man->range ,man->y + man->size / 2 - man->range, man->range * 2, man->range * 2 };
	SDL_RenderDrawRect(renderer, &man_range_rect, 1);


	if (mermi->life != 0) {


		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_Rect bullet_1 = { mermi->x,mermi->y,mermi->width,mermi->height };
		SDL_RenderFillRect(renderer, &bullet_1);


	}



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

	for (int n = 0; n < enemy_count; n++) { //her düşmanı ayrı ayrı print ediyorum

		if (enemyler[n].life != 0) {
			SDL_SetRenderDrawColor(renderer, 204, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1[n]);
			SDL_SetRenderDrawColor(renderer, 150, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1_dec[n]);
		}
	}

	// text  yazdırma denemeleri
	/*TTF_Font* font = TTF_OpenFont("josefin.ttf", 400);
	SDL_Color white = { 255,255,255,255 };
	int point = 0;
	char textBuffer[20];

	SDL_Surface* surface;
	SDL_Texture* texture;
	surface = TTF_RenderText_Solid(font, textBuffer, white);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect destRect = { 100, 100, 20, 20 };

	snprintf(textBuffer, sizeof(textBuffer), "puan: %d", point);
	if (texture) {
		SDL_DestroyTexture(texture);
	}

	SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);
	SDL_RenderCopy(renderer, texture, point, &destRect);

	point++;*/

	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {



	SDL_Window* window;  //window tanımlama
	SDL_Renderer* renderer; //renderer tanımlama
	SDL_Init(SDL_INIT_EVERYTHING); // sdl'i çalıştırıyor 
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
	//Mix_Music* deneme = Mix_LoadWav("laserShoot.wav");

	Mix_Chunk* shootEffect = Mix_LoadWAV("laserShoot.wav"); // ses yükledim
	Mix_Chunk* hitEffect = Mix_LoadWAV("hitHurt.wav"); // ses yükledim

	TTF_Init();

	Man man;
	man.x = 220;
	man.y = 310;
	man.size = 50;
	man.puan = 0;
	man.range = 200;

	Bullet mermi;
	mermi.x = 10000;
	mermi.y = 10000;
	mermi.width = 20;
	mermi.height = 20;
	mermi.speedx = 10;
	mermi.speedy = 10;

	Mix_Chunk* bornEffect = Mix_LoadWAV("synth.wav");


	Enemy enemyler[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].x = 400 + (n * 100);
		enemyler[n].y = 400;
		enemyler[n].width = 40 + n * 10;
		enemyler[n].height = 40 + n * 10;
		enemyler[n].life = 1 + (n * 1);
		enemyler[n].speed = n * 10;
	}




	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1424, 740, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);


	Mix_Music* background = Mix_LoadWAV("backgro.wav");

	int done = 0;
	Mix_PlayChannel(1, background, -1);
	Mix_PlayChannel(-1, bornEffect, 0);
	while (!done) {
		done = processEvents(window, &man, &mermi, &enemyler,&hitEffect,&shootEffect);
		//render display
		doRender(renderer, &man, &mermi, &enemyler);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);



	return 0;
}
