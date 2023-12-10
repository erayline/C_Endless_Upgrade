#ifndef STRUCTER_H
#define STRUCTER_H

#define FPS = 1
#define WINDOW_WIDTH 1220 // ön şişmanlı komutları
#define WINDOW_HEIGHT 880
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 100
#define mermi_count 40
#define MAP_WIDTH 3000
#define MAP_HEIGHT 3000

int enemy1_walk_spreadsheet_width = 0;

int last_frame_time_renderer;


int map_x = 0;
int map_y = 0;


typedef struct {
	int time_counter;
	int difficulty_factor;
	int level;
}Game_stats;

typedef struct {
	int page;
}GUI_State;

typedef struct {
	int x, y;
	int current_life;
	int max_life;
	int size;
	int range;
	int puan;
	int money;
	int bullet_power;
	int bullet_speed;
	char name[20];
	int move_speed;
	int attack_speed;

	int idle_image_timer;
	int idle_image_width_increment;
	int run_image_timer;
	int run_image_width_increment;
	int attack_image_timer;
	int attack_image_width_increment;
	int attack_animation_time_holder;
	int attack_direction;



	int animation;
	int direction;
	int direction_key;
	int y_holder;
	int x_holder;
}Man;

typedef struct {
	float x;
	float y;
	int life;
	int width;
	int height;
	float speedx;
	float speedy;
	int biterken;
}Bullet;

typedef struct {
	float x;
	float y;
	int color;
	int life;
	int width;
	int height;
	int speed;
	int enemy1_walking_last_time;
	int enemy1_walking_image_width;

	int animation;
	int direction;
	int direction_key;
	int y_holder;
	int x_holder;
}Enemy;



typedef struct {
	SDL_Texture* protogonist_texture_idle;
	SDL_Texture* protogonist_texture_run;
	SDL_Texture* protogonist_texture_attack;
	SDL_Texture* map_image_texture;
	SDL_Texture* enemy1_image_texture;
}Game_assets;

#endif
