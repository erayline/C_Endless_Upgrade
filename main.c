#include <SDL.h>
#include <stdio.h>

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

int processEvents(SDL_Window *window, Man *man,Bullet *mermi) {
	SDL_Event event;
	int done = 0;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					done = 1;
					break;
				case SDLK_SPACE:
					mermi->x = man->x + (man->size)/2 - (mermi->width)/2;
					mermi->y = man-> y;
					mermi->life = 1;
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
		man->x += 4;
	}
	if (state[SDL_SCANCODE_LEFT]) {
		man->x -= 4;
	}
	if (state[SDL_SCANCODE_UP]) {
		man->y -= 4;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		man->y += 4;
	}
	if (state[SDL_SCANCODE_K]) {
		man->size += 4;
	}
	if (mermi->life == 1) {
		mermi->y -= 7;
	}

	if (mermi->y < 0) {
		mermi->life = 0;
	}


	return done;
}



void doRender(SDL_Renderer* renderer, Man* man, Bullet *mermi) {

	SDL_SetRenderDrawColor(renderer, 20, 0, 20, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 204, 204, 244, 255);

	SDL_Rect bullet_1 = { mermi -> x,mermi -> y,mermi -> width,mermi -> height };
	SDL_Rect rect = { man -> x,man -> y, man->size,man->size };
	SDL_RenderFillRect(renderer, &rect); // burada içini dolduracaðýmýz cismin adresine atýfta bulunuyoruz.
	SDL_RenderFillRect(renderer, &bullet_1);

	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {

	SDL_Window* window;  //window tanýmlama
	SDL_Renderer* renderer; //renderer tanýmlama
	SDL_Init(SDL_INIT_EVERYTHING); // sdl'i çalýþtýrýyor 


	Man man;
	man.x = 220;
	man.y = 110;
	man.size = 50;
	
	Bullet mermi;
	mermi.x = 10;
	mermi.y = 10;
	mermi.width = 10;
	mermi.height = 10;
	
	


	window = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024,740,SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);




	int done = 0;
	while (!done) {

		done = processEvents(window, &man, &mermi);
		//render display
		doRender(renderer, &man, &mermi);

		SDL_Delay(10);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);



	return 0;
}