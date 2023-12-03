#ifndef STRUCTER_H
#define STRUCTER_H

#define FPS = 1
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define FRAME_TARGET_TIME = (1000/FPS) //her frame kaç ms sürecek
#define enemy_count 10
#define mermi_count 40
#define MAP_WIDTH 3000
#define MAP_HEIGHT 3000

int enemy_start_coord[60][2] = { // enemylerin konumlarýný manuel ayarlayacaðým ondan for kullanmadým henüz.
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
	{10,10},{WINDOW_WIDTH - 100,10},{10,WINDOW_HEIGHT - 100},{WINDOW_WIDTH - 100,WINDOW_HEIGHT - 100},{WINDOW_WIDTH / 2,10},{WINDOW_WIDTH / 2,WINDOW_HEIGHT - 100},
};

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
	int life;
	int size;
	int range;
	int puan;
	int money;
	char name[20];
} Man;

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
}Enemy;

#endif