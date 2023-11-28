#ifndef STRUCTER_H
#define STRUCTER_H


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
	float speedx;
	float speedy;
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