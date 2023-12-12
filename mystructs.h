#ifndef STRUCTER_H
#define STRUCTER_H

#define FPS = 1
#define WINDOW_WIDTH 1920 // ön şişmanlı komutları
#define WINDOW_HEIGHT 1080
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 100
#define mermi_count 40
#define MAP_WIDTH 12000
#define MAP_HEIGHT 8000

#define enemy_life_bar_width 80
#define enemy_life_bar_height 20
#define life_bar_width 380
#define life_bar_height 40

int enemy1_walk_spreadsheet_width = 0;

int last_frame_time_renderer;
SDL_Rect camera = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };

int upgrade_choice = 0;
float speed_constant;
int last_bullet_shot = 10;
int yakinlik[enemy_count]; // bu yakınlık listesi. en sondaki eleman en yakındaki eleman oluyor.
int last_collision_man = 0;



int map_x = 0;
int map_y = 0;


typedef struct {

	int time_counter_min;
	int time_counter_sec;
	int difficulty_factor;
	int state;

}Game_stats;

typedef struct {
	int page;
}GUI_State;

typedef struct {
	char name[20];
	int x, y;
	int size;
	int range;
	int puan;
	int money;

	int current_life;
	int max_life;
	int bullet_power;
	int bullet_speed;
	int attack_speed;
	int move_speed;

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

	int max_life;
	int current_life;
	int life_bar_active;

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


typedef struct {
	int x;
	int y;
	int width;
	int height;
	int amount;
	int life;
	int item_index;
	SDL_Texture* item_texture;
}Item;

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int price;
	int isActive;
}Chest;

#endif
