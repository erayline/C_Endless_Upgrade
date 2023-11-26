#include <SDL.h>
#include <stdio.h>

#define FPS = 30
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
int last_frame_time = 0;

typedef struct {
	int x, y;
	int life;
	char* name;
	int size;
} Man;

typedef struct {
	int x;
	int y;
	int life;
	int width;
	int height;
}Bullet;

typedef struct {
	int x;
	int y;
	int color;
	int life;
	int width;
	int height;

}Enemy;

 

int processEvents(SDL_Window *window, Man *man,Bullet *mermi, Enemy *enemy) {
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
					mermi->x = man->x + (man->size)/2 - (mermi->width)/2;
					mermi->y = man-> y;
					mermi->life = 1;
					break;
				case SDLK_0:
					enemy->life += 1;
					enemy->x = 0;
					enemy->y = 0;
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
		mermi->y -= 7;
	}
	if (mermi->y < 0) {
		mermi->life = 0;
	}

	if (mermi->life <= 0) {
		mermi->y = -2000;
		mermi->x = -2000;
	}
	if (enemy->life <= 0) {
		enemy->y = -1000;
		enemy->x = -1000;
	}

	 
	if (enemy->life != 0) {
		if (enemy->y + enemy->height/2 > man->y + man->size/2) {
			enemy->y -= 150 * delta_time;
		}
		if (enemy->y + enemy->height / 2 <= man->y + man->size/2) {
			enemy->y += 150 * delta_time;
		}
		if (enemy->x + enemy->width / 2 > man->x + man->size / 2) {
			enemy->x -= 150 * delta_time;
		}
		if (enemy->x + enemy->width / 2 <= man->x + man->size / 2) {
			enemy->x += 150 * delta_time;
		}
	}



	if ((((enemy->x < mermi->x) && (enemy->x + enemy->width > mermi->x)) || ((enemy->x < mermi->x + mermi->width) && (enemy->x + enemy->width > mermi->x + mermi->width))) && (((enemy->y < mermi->y) && (enemy->y + enemy->height > mermi->y)) || ((enemy->y < mermi->y + mermi->height) && (enemy->y + enemy->height > mermi->y + mermi->height)))) {
		enemy->life -= 1;
		mermi->life = 0;
	}

	int x_fare, y_fare;
	SDL_GetMouseState(&x_fare, &y_fare);
	printf("%d ", x_fare);
	printf("%d\n", y_fare);

	return done;
}



void doRender(SDL_Renderer* renderer, Man* man, Bullet *mermi, Enemy *enemy) {

	SDL_SetRenderDrawColor(renderer, 50, 10, 30, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 204, 204, 244, 255);

	SDL_Rect rect = { man -> x,man -> y, man->size,man->size };

	SDL_RenderFillRect(renderer, &rect); // burada içini dolduracağımız cismin adresine atıfta bulunuyoruz.

	if (mermi->life != 0) {
		SDL_Rect bullet_1 = { mermi -> x,mermi -> y,mermi -> width,mermi -> height };
		SDL_RenderFillRect(renderer, &bullet_1);

	}

	if (enemy->life != 0) {
		SDL_Rect enemy_1 = { enemy->x, enemy->y,enemy->width,enemy->height };
		SDL_SetRenderDrawColor(renderer, 204, 204, 20, 255);
		SDL_RenderFillRect(renderer, &enemy_1);
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
	

	


	window = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024,740,SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);




	int done = 0;
	while (!done) {

		done = processEvents(window, &man, &mermi, &enemy);
		//render display
		doRender(renderer, &man, &mermi, &enemy);
	}

	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	


	return 0;
}
