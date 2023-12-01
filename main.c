#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include "mystructs.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>


/*  yapılacaklar :
	her üç saniyede bir düşman oluşsun düşman // dalgaları oluşsun
	arayüz oluşturmaya başla // karakterler için gif kullan 





   yapılanlar:
	düşmanın her köşesini algılamıyor sol üst sağ alt algılıyor// çözdüm
	düşman hareketlerine mermi hareketi mekaniğini ekle. // çözüldü artık adamakıllı beni takip ediyorlar

*/


float speed_constant;
int last_bullet_shot=10;
int yakinlik[enemy_count]; // bu yakınlık listesi. en sondaki eleman en yakındaki eleman oluyor.

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
int processEvents(SDL_Window* window, Man* man, Bullet* mermiler, Enemy* enemyler, Mix_Chunk* hitEffect, Mix_Chunk* shootEffect) { // burada işlemler oluyor


	/// deneme başlangıç


		
	/// deneme bitiş



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


	// hareket etme başlanıgç
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
				bullet_speed_interval -= 10;
				man->range += 20;
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

	// hareket etme bitiş




	//mermi fonksiyonları başlangıç

	for (int n = 0; n < enemy_count; n++) { //burada range içine giren eleman olursa ateş etsin diye ayarladım
		//old kod // 		if (((((enemyler[n].x > man->x - man->range) && (enemyler[n].x < man->x + man->range)) && ((enemyler[n].y > man->y - man->range) && (enemyler[n].y < man->y + man->range))) || (((enemyler[n].x + enemyler[n].width > man->x - man->range) && (enemyler[n].x + enemyler[n].width < man->x + man->range)) && ((enemyler[n].y + enemyler[n].height > man->y - man->range) && (enemyler[n].y + enemyler[n].height < man->y + man->range))))) {
		// new kod // (icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x)	&& icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y))	||  (icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y + enemyler[n].height)) ||(icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y)) ||(icindemi(man_ortasiX - man->range, man_ortasiX + man->range, enemyler[n].x + enemyler[n].width) && icindemi(man_ortasiY - man->range, man_ortasiY + man->range, enemyler[n].y + enemyler[n].height)))
		if((icindemi(man->x - man->range, man->x +man->size + man->range, enemyler[n].x)						&& icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
		   (icindemi(man->x - man->range, man->x +man->size + man->range, enemyler[n].x)						&& icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height)) ||
		   (icindemi(man->x - man->range, man->x +man->size + man->range, enemyler[n].x + enemyler[n].width)	&& icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y)) ||
		   (icindemi(man->x - man->range, man->x +man->size + man->range, enemyler[n].x + enemyler[n].width)	&& icindemi(man->y - man->range, man->y + man->size + man->range, enemyler[n].y + enemyler[n].height))
			){
			for (int i = 0; i < 30; i++) {
				if (mermiler[i].life != 1) { // halihazırda ateşlenmemiş mermiyi ateşliyor
					if ( bullet_speed_interval < (SDL_GetTicks() - last_bullet_shot)) { //burada iki saniyede bir ateş etsin diye ayarlamaya çalıştım 
						if ( 10 < (SDL_GetTicks() - mermiler[i].biterken)) {
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

	for (int i = 0; i < 30; i++) {

		if (mermiler[i].life == 1) {
			mermiler[i].x -= mermiler[i].speedx;
			mermiler[i].y -= mermiler[i].speedy;
		}


		if (mermiler[i].y < 0 || mermiler[i].x < 0 || mermiler[i].y > WINDOW_HEIGHT || mermiler[i].x >WINDOW_WIDTH) {
			mermiler[i].life = 0;
			mermiler[i].biterken = SDL_GetTicks();
		}
	}

	speed_constant = 300;
	float diagnal = uzaklikSirasi[enemy_count - 1][0];

	float cosforx = (man_ortasiX - enemyler[closest_enemy_index].x - enemyler[closest_enemy_index].width / 2) / diagnal;
	float sinfory = (man_ortasiY - enemyler[closest_enemy_index].y - enemyler[closest_enemy_index].height / 2) / diagnal;

	for (int i = 0; i < 30; i++) {

		if (mermiler[i].life <= 0) {
			mermiler[i].speedx = cosforx * speed_constant * delta_time;
			mermiler[i].speedy = sinfory * speed_constant * delta_time;

			mermiler[i].y = 2000;
			mermiler[i].x = 2000;
		}
	}

	/// mermi fonksiyonları bitiş



	//enemy güncelleme başlangıç
	// spawn olacakların sayısını tanımladığın bir değişkenin olabilir.
	int idd = 0;
	if (5000 < SDL_GetTicks() - last_spawn ) {
		printf("oldu bu iş");
		last_spawn = SDL_GetTicks();

		while (idd < 2){ // UNDER CONSTRUCTİON ŞUAN BOZUK
			for (int n = 0; n < enemy_count; n++) {
				if (enemyler[n].life == 0) {
					enemyler[n].y = enemy_start_coord[n][1];
					enemyler[n].x = enemy_start_coord[n][0];
					enemyler[n].width = 50;
					enemyler[n].height = 50;
					enemyler[n].life = 1;
					enemyler[n].speed = 70 + n * 4;
					idd++;
				}
			}
		}

	}


	for (int n = 0; n < enemy_count; n++) {

		float diagnal_enem = sqrt(pow(man_ortasiX - enemyler[n].x - enemyler[n].width / 2,2)+pow(man_ortasiY - enemyler[n].y - enemyler[n].width / 2,2));
		float cosforx_enem = (man_ortasiX - enemyler[n].x - enemyler[n].width / 2) / diagnal_enem;
		float sinfory_enem = (man_ortasiY - enemyler[n].y - enemyler[n].height / 2) / diagnal_enem;

		if (enemyler[n].life <= 0) {
			enemyler[n].y = -1000;
			enemyler[n].x = -1000;
		}
		else {

			if (enemyler[n].life != 0) { // enemylerin hızlarını ayarladığım bölüm
				if (enemyler[n].y + enemyler[n].height / 2 != man->y + man->size / 2) {
					enemyler[n].y += sinfory_enem*enemyler[n].speed*delta_time;
				}
				if (enemyler[n].x + enemyler[n].width / 2 != man->x + man->size / 2) {
					enemyler[n].x += cosforx_enem*enemyler[n].speed * delta_time;
				}
			}


			for (int i = 0; i < 30; i++) {
				if ((((enemyler[n].x < mermiler[i].x) && (enemyler[n].x + enemyler[n].width > mermiler[i].x)) || ((enemyler[n].x < mermiler[i].x + mermiler[i].width) && (enemyler[n].x + enemyler[n].width > mermiler[i].x + mermiler[i].width))) && (((enemyler[n].y < mermiler[i].y) && (enemyler[n].y + enemyler[n].height > mermiler[i].y)) || ((enemyler[n].y < mermiler[i].y + mermiler[i].height) && (enemyler[n].y + enemyler[n].height > mermiler[i].y + mermiler[i].height)))) {
					enemyler[n].life -= 1;
					man->puan += 1; // mermi için collision detection
					mermiler[i].life = 0;
					Mix_PlayChannel(-1, hitEffect, 0);
					mermiler[i].biterken = SDL_GetTicks();
					if (bullet_speed_interval != 1) { //// denemek için yaptım silinecek
						bullet_speed_interval -= 30;
					}
					else {
						bullet_speed_interval = 1;
					}
				}
			}
		}
		//düşman güncelleme bitiş.


	}
	return done;
}


int enson=0;
void doRender(SDL_Renderer* renderer, Man* man, Bullet* mermiler, Enemy* enemyler,SDL_Surface* enemy1_surface) {
	SDL_Rect birden = { 0,0,enemy1_surface->w,enemy1_surface->h };
	SDL_Rect ikiye = { 0,0,enemy1_surface->w,enemy1_surface->h };
	//SDL_Rect birden = { 0,0,enemy1_surface->w,enemy1_surface,h };
	SDL_BlitSurface(enemy1_surface, &birden, &renderer, &ikiye);


	SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
	SDL_RenderClear(renderer); // arkaplan katmanı


	SDL_SetRenderDrawColor(renderer, 20, 204, 84, 255);
	SDL_Rect rect = { man->x,man->y, man->size,man->size };
	SDL_RenderFillRect(renderer, &rect); // protogonist katmanı bitiş
	SDL_SetRenderDrawColor(renderer, 10, 80, 30, 200);
	SDL_Rect man_range_rect = { man->x + man->size / 2 - man->range ,man->y + man->size / 2 - man->range, man->range * 2, man->range * 2 };
	SDL_RenderDrawRect(renderer, &man_range_rect); // protogonist menzili bitiş
	




	for (int n = 0; n < 30; n++) {
		if (mermiler[n].life != 0) {

			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_Rect bullet_1 = { mermiler[n].x,mermiler[n].y,mermiler[n].width,mermiler[n].height };
			SDL_RenderFillRect(renderer, &bullet_1);

		} 
	} 
	SDL_Rect mermiler_1[30];
	for (int n = 0; n < 30; n++) {
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

	for (int n = 0; n < enemy_count; n++) { //her düşmanı ayrı ayrı print ediyorum

		if (enemyler[n].life != 0) {
			SDL_SetRenderDrawColor(renderer, 204, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1[n]);
			SDL_SetRenderDrawColor(renderer, 150, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1_dec[n]);
		}
	}// enemy katmanı bitiş

	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
	


	SDL_Window* window;  //window tanımlama
	SDL_Renderer* renderer; //renderer tanımlama
	SDL_Init(SDL_INIT_VIDEO); // sdl'i çalıştırıyor 
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);





	//TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
	//Mix_Music* deneme = Mix_LoadWav("laserShoot.wav");
	Mix_Chunk* shootEffect = Mix_LoadWAV("sounds/laserShoot.wav"); // ses yükledim
	Mix_Chunk* hitEffect = Mix_LoadWAV("sounds/hitHurt.wav"); // ses yükledim
	
	///RESİM YÜKLEME
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* enemy1_surface = IMG_Load("images/enemy1.png");
	///RESİM YÜKLEME
	TTF_Init();

	enson = SDL_GetTicks();

	Man man;
	man.x = 720;
	man.y = 410;
	man.size = 50;
	man.puan = 0;
	man.range = 230;

	Bullet mermiler[30];
	for (int n = 0; n < 30; n++) {
		mermiler[n].y = 10000;
		mermiler[n].x = 10000;
		mermiler[n].width = 15;
		mermiler[n].height = 15;
		mermiler[n].speedx = 10;
		mermiler[n].speedy = 10;
		mermiler[n].biterken = 10;
		mermiler[n].life = 0;
	}



	Enemy enemyler[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].y = enemy_start_coord[n][1];
		enemyler[n].x = enemy_start_coord[n][0];
		enemyler[n].width = 50 ;
		enemyler[n].height = 50 ;
		enemyler[n].life = 0;
		enemyler[n].speed = 70+ n*4;
	}




	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);


	Mix_Music* background = Mix_LoadWAV("sounds/backgro.wav");

	int done = 0;
	Mix_PlayChannel(1, background, -1);
	while (!done) {
		done = processEvents(window, &man, &mermiler, &enemyler, hitEffect, shootEffect);
		//render display
		doRender(renderer, &man, &mermiler, &enemyler,&enemy1_surface);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	return 0;
}
