#ifndef STRUCTER_H
#define STRUCTER_H

#include <SDL.h>
#include <SDL_ttf.h>

#define FPS = 60
#define WINDOW_WIDTH 1920 // ön þiþmanlý komutlarý
#define WINDOW_HEIGHT 1080
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 200
#define mermi_count 40
#define MAP_WIDTH 2000
#define MAP_HEIGHT 2000

#define life_bar_width 380
#define life_bar_height 40

#define item_count (3 +1)// item adedi (artý biri silme silersen bozuluyor. oradaki dört deðiþken artý bir sabit fonksiyonda sýfýrdan baþladýðý için artý bir demek zorunda kaldýn)
int enemy_life_bar_width = 30;
int enemy_life_bar_height = 20;

int enemy1_walk_spreadsheet_width = 0;

int last_frame_time_renderer;
SDL_Rect camera = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

float max_attack_speed = 500;
float delta_time;
float speed_constant_bullet;
int last_bullet_shot = 10;
int yakinlik[enemy_count]; // bu yakýnlýk listesi. en sondaki eleman en yakýndaki eleman oluyor.
int last_collision_man = 0;

int gidilecek_x = MAP_WIDTH/2; // it doesn't place us execly to the middle of the map.
int gidilecek_y = MAP_HEIGHT/2;
float diagnal_man_m;
float cosforx_man_m;
float sinfory_man_m;



int map_x = 0;
int map_y = 0;

int y_dogrultusunda_yon;


int man_ortasiX;
int man_ortasiY;


int right_click_effect_x = -10000;
int right_click_effect_y = -10000;




int item_bar_active;
int item_index_list[10] = {0,0,0,0,0,0,0,0,0,0};
int item_bar_x_y_list[10][2] = {
	{40,230},
	{130,230},
	{30,300},
	{110,300},
	{30,370},
	{100,370},
	{30,230},
	{100,230},
	{30,230},
	{100,230}
};
SDL_Texture* bastirilacak_item;


TTF_Font* font50;


int collision_kontrol_counter = 0;

typedef struct {
	int time_counter_min;
	int time_counter_sec;
	int difficulty_factor;
	int wave;
}Game_stats;
//
int last_spawn = 1;
int atack_speed = 400;
int bullet_speed_interval = 500;
int last_frame_time = 0;
float bullet_x_speed = 0;
float bullet_y_speed = 0; // burada birsürü gereksiz þey tanýmlamýþým bir ara azaltýcam.
int isPressed_left = 0;
int isPressed_right = 0;
int animation_control_timer = 0;


typedef struct {
	int enemy_start_coord[16][2]; // 8 adet düþmanýn baþlangýç koordinatlarý
} SpawnPoints;

SpawnPoints spawnpoints = {
	{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
};

typedef struct {
	int x;
	int y;
}Mouse;
Mouse mouse_position;

typedef struct {
	int page;
}GUI_State;
int q_pressed;
typedef struct {
	char name[20];
	float x, y;
	int size;
	float range;
	int puan;
	int money;

	int current_life;
	int max_life;
	int bullet_power;
	int bullet_speed;
	float attack_speed;
	int move_speed;

	int idle_image_timer;
	int idle_image_width_increment;
	int run_image_timer;
	int run_image_width_increment;
	int attack_image_timer;
	int attack_image_width_increment;
	int attack_animation_time_holder;
	int attack_direction;
	
	int q_click_ready;
	int q_click_last_use;



	int moving;
	float speedx;
	float speedy;



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
	float x,y;
	int color;

	int max_life;
	int current_life;
	int life_bar_active;

	int spiece;

	float width,height;
	int speed;
	int enemy_walking_last_time;

	int enemy1_walking_image_width;
	int enemy2_walking_image_width;

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
	SDL_Texture* enemy1_run_image_texture;
	SDL_Texture* enemy2_run_image_texture;

	SDL_Texture* enemy2_image_texture;

	SDL_Texture* item_1;
	SDL_Texture* item_2;
	SDL_Texture* item_3;
	SDL_Texture* item_4;
	SDL_Texture* item_5;
	SDL_Texture* item_6;
	SDL_Texture* item_7;

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
	int CursedGlove_c;
	int LifeReminder_c;
	int MovementFeather_c;
	int RedTeeth_c;
}Owned_Items;

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int price;
	int isActive;
}Chest;

Owned_Items owned_items;

Man man;
Item itemler[100];
Enemy enemyler[enemy_count];
Game_assets game_assets;


SDL_Renderer* renderer; //renderer tanýmlama


Game_stats game_stats;


GUI_State gui_state;

//movement part

int clicked_a = 0;
int clicked_w = 0;
int clicked_d = 0;
int clicked_s = 0;

Bullet mermiler[mermi_count];






#endif