#include <SDL.h>
#include <stdio.h>

#define FPS = 30
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 10
int last_frame_time = 0;

typedef struct {
	int x, y;
	int life;
	int size;
	int puan;
} Man;
typedef struct {
	float x;
	float y;
	int life;
	int width;
	int height;
}Bullet;
typedef struct {
	float x;
	float y;
	int color;
	int life;
	int width;
	int height;
	int speed;
}Enemy;
int processEvents(SDL_Window *window, Man *man,Bullet *mermi, Enemy *enemy, Enemy *enemyler) {
	SDL_Event event;
	int done = 0;
	
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks();

	

	// moving part
	while (SDL_PollEvent(&event)) {

		SDL_Delay(20);

		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
				case (SDLK_ESCAPE):
					done = 1;
					break;
				case SDLK_SPACE:
					if (mermi->life != 1) {
						mermi->x = man->x + (man->size)/2 - (mermi->width)/2;
						mermi->y = man-> y - man->size/10;
						mermi->life = 1;
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
	if (state[SDL_SCANCODE_DOWN]){
		man->y += 400 * delta_time;
	}

	if (mermi->life == 1) {
		mermi->y -= 1000 * delta_time;
	}
	if (mermi->y < 0) {
		mermi->life = 0;
	}

	if (mermi->life <= 0) {
		mermi->y = -2000;
		mermi->x = -2000;
	}

	for (int n = 0; n < enemy_count; n++) {
		
		if (enemyler[n].life <= 0) {
			enemyler[n].y = -1000;
			enemyler[n].x = -1000;
		}
		else {
			if (enemyler[n].life != 0) {
				if (enemyler[n].y + enemyler[n].height/2 > man->y + man->size/2) {
					enemyler[n].y -= enemyler[n].speed * delta_time;
				}
				if (enemyler[n].y + enemyler[n].height / 2 < man->y + man->size/2) {
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
				man->puan += 1;
				mermi->life = 0;
			}

		}


	}




	return done;
}



void doRender(SDL_Renderer* renderer, Man* man, Bullet *mermi, Enemy *enemy, Enemy *enemyler) {

	SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
	SDL_RenderClear(renderer);


	SDL_SetRenderDrawColor(renderer, 20, 204, 84, 255);
	SDL_Rect rect = { man -> x,man -> y, man->size,man->size };
	SDL_RenderFillRect(renderer, &rect); // burada içini dolduracağımız cismin adresine atıfta bulunuyoruz.

	if (mermi->life != 0) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_Rect bullet_1 = { mermi -> x,mermi -> y,mermi -> width,mermi -> height };
		SDL_RenderFillRect(renderer, &bullet_1);

	}

	SDL_Rect enemy_1[enemy_count];
	SDL_Rect enemy_1_dec[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemy_1[n].x = enemyler[n].x;
		enemy_1[n].y = enemyler[n].y;
		enemy_1[n].w = enemyler[n].width;
		enemy_1[n].h = enemyler[n].height;

		enemy_1_dec[n].x = enemyler[n].x + enemyler[n].width/4;
		enemy_1_dec[n].y = enemyler[n].y + enemyler[n].height / 4;
		enemy_1_dec[n].w = enemyler[n].width - enemyler[n].width/2;
		enemy_1_dec[n].h = enemyler[n].height - enemyler[n].height / 2;
	}

	for (int n = 0; n < enemy_count; n++) {

		if (enemyler[n].life != 0) {
			SDL_SetRenderDrawColor(renderer, 204, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1[n]);
			SDL_SetRenderDrawColor(renderer, 150, 20, 20, 255);
			SDL_RenderFillRect(renderer, &enemy_1_dec[n]);
		}
	}


	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {

	SDL_Window* window;  //window tanımlama
	SDL_Renderer* renderer; //renderer tanımlama
	SDL_Init(SDL_INIT_EVERYTHING); // sdl'i çalıştırıyor 



	Man man;
	man.x = 220;
	man.y = 110;
	man.size = 50;
	man.puan = 0;
	
	Bullet mermi;
	mermi.x = -10; 
	mermi.y = -10;
	mermi.width = 20;
	mermi.height = 20;
	
	Enemy enemy;
	enemy.x = 400;
	enemy.y = 30;
	enemy.width = 60;
	enemy.height = 60;
	enemy.life = 1;

	Enemy enemyler[enemy_count];
	for (int n = 0; n < enemy_count; n++) {
		enemyler[n].x = n * 100;
		enemyler[n].y = 400;
		enemyler[n].width = 40 + n*4;
		enemyler[n].height =  40 + n*4;
		enemyler[n].life = 1;
		enemyler[n].speed =  140 - n * 10 ;
	}

	


	window = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024,740,SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);




	int done = 0;
	while (!done) {

		done = processEvents(window, &man, &mermi, &enemy, &enemyler);
		//render display
		doRender(renderer, &man, &mermi, &enemy,&enemyler);
	}

	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	


	return 0;
}
