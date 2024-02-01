#ifndef FRONTEND_H
#define FRONTEND_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "mystructs.h"


int loadImages() {

	SDL_Surface* map_image_surface = IMG_Load("images/maps/map1.png"); // resim.png dosyasýnýn adýný ve yolunu düzenleyin
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

}

///yazý YÜKLEME baþlangýç

int drawUIText() {



	char time_str[16];
	sprintf_s(time_str, sizeof(time_str), "%d:%d", game_stats.time_counter_min, game_stats.time_counter_sec);
	SDL_Color time_color = { 223,233,223,233 }; // para
	SDL_Surface* text_surface1 = TTF_RenderText_Solid(font50, time_str, time_color);
	SDL_Texture* text_texture1 = SDL_CreateTextureFromSurface(renderer, text_surface1);
	SDL_Rect text_rect1 = { WINDOW_WIDTH - 90, 200 - text_surface1->h / 2,text_surface1->w,text_surface1->h };
	SDL_RenderCopy(renderer, text_texture1, NULL, &text_rect1);
	SDL_DestroyTexture(text_texture1);
	SDL_FreeSurface(text_surface1);


	SDL_Color pause_color = { 252,215,222,255 }; // pause
	SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "P For Pause ", pause_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_start = { 20, 70 - text_surface->h / 2, text_surface->w,text_surface->h };
	SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);

	char puan_str[16];
	sprintf_s(puan_str, sizeof(puan_str), "Money: %4d", man.money);
	SDL_Color white = { 22,132,22,255 }; // para
	SDL_Surface* text_surface2 = TTF_RenderText_Solid(font50, puan_str, white);
	SDL_Texture* text_texture2 = SDL_CreateTextureFromSurface(renderer, text_surface2);
	SDL_Rect text_rect = { WINDOW_WIDTH - 290, 70 - text_surface2->h / 2,text_surface2->w,text_surface2->h };
	SDL_RenderCopy(renderer, text_texture2, NULL, &text_rect);
	SDL_DestroyTexture(text_texture2);
	SDL_FreeSurface(text_surface2);


	return 0;
}

int drawWaveCount() {
	char WaveSTR[16];

	SDL_Color white = { 121,13,22,255 }; // para
	sprintf_s(WaveSTR, sizeof(WaveSTR), "Wave: %3d", game_stats.wave);
	SDL_Surface* text_surface2 = TTF_RenderText_Solid(font50, WaveSTR, white);
	SDL_Texture* text_texture2 = SDL_CreateTextureFromSurface(renderer, text_surface2);
	SDL_Rect text_rect = { WINDOW_WIDTH/2-text_surface2->w/2, 70 - text_surface2->h / 2,text_surface2->w,text_surface2->h };
	SDL_RenderCopy(renderer, text_texture2, NULL, &text_rect);
	SDL_DestroyTexture(text_texture2);
	SDL_FreeSurface(text_surface2);

}

void drawHowToEnter() {

	SDL_Color white = { 252,215,222,255 };

	SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "press SPACE to start ", white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2, WINDOW_HEIGHT - text_surface->h - 50, text_surface->w,text_surface->h };
	SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}

void drawWelcomeText() {
	SDL_Color white = { 252,215,222,255 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Welcome to the COLLECTOR", white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_welcome = { WINDOW_WIDTH / 2 - text_surface->w / 2, WINDOW_HEIGHT / 2 - text_surface->h / 2, text_surface->w,text_surface->h };
	SDL_RenderCopy(renderer, text_texture, NULL, &text_welcome);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}

int drawPausedText() {

	SDL_Color white = { 222,0,22,233 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(font50, "Paused ", white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_start = { WINDOW_WIDTH / 2 - text_surface->w / 2,  WINDOW_HEIGHT / 2 - text_surface->h / 2, text_surface->w,text_surface->h };
	SDL_RenderCopy(renderer, text_texture, NULL, &text_start);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);

	return 0;
}



int drawUpgradeCardBack(int indexNumber) {
	int x_coord=0, y_coord=0;
	if (indexNumber == 0) {
		x_coord = 130;
		y_coord = 110;
	}
	else if (indexNumber == 1) {
		x_coord = 530;
		y_coord = 110;
	}
	else if (indexNumber == 2) {
		x_coord = 130;
		y_coord = 610;
	}
	else if (indexNumber == 3) {
		x_coord = 530;
		y_coord = 610;
	}
	int width = 300;
	int height = 400;
	SDL_Rect card_rect = { x_coord,y_coord,width,height};
	if (isMouseInThis(x_coord, y_coord, width, height)) {
		SDL_SetRenderDrawColor(renderer, 240, 20, 60, 1);
		if (isPressed_left && indexNumber==0) {
			if (upgrade_names[upgradeIndexList[0]] == "max life           ") {
				printf("can artti");
			}
		}
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 20, 10, 1);
	}

	if (isPressed_left && isMouseInThis(x_coord, y_coord, width, height)) {
		gui_state.page = 1;
	}

	SDL_RenderFillRect(renderer, &card_rect);
	return 1;
}


// upgrade part

void drawUpgradeCardInside(int indexNumber) {
	char WaveSTR[20];
	SDL_Color white = { 255,225,122,255 }; // para
	sprintf_s(WaveSTR, sizeof(WaveSTR), "%s", upgrade_names[upgradeIndexList[indexNumber]]);
	SDL_Surface* surface = TTF_RenderText_Solid(font50, WaveSTR, white);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int x_coord=0, y_coord=0;

	if (indexNumber == 0) {
		x_coord = 170;
		y_coord = 140;
	}
	else if (indexNumber == 1) {
		x_coord = 570;
		y_coord = 140;
	}
	else if (indexNumber == 2) {
		x_coord = 170;
		y_coord = 640;
	}
	else if (indexNumber == 3) {
		x_coord = 570;
		y_coord = 640;
	}	

	SDL_Rect text_rect = { x_coord, y_coord,215,60 };
	SDL_RenderCopy(renderer, texture, NULL, &text_rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

#endif