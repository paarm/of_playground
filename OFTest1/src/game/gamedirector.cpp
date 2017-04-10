#include "gamedirector.h"
#include "../engine/director.h"
#include "gamescene.h"

#define VIRTUAL_SCREEN_SIZE_X 1280
#define VIRTUAL_SCREEN_SIZE_Y 768
#define BLOCK_SIZE_X 32
#define BLOCK_SIZE_Y 32
#define BLOCK_COUNT_X 40
#define BLOCK_COUNT_Y 24
#define ENEMY_SPEED_LEFTRIGHT_PER_SECOND 300.0
#define ENEMY_SPEED_TOPBOTTOM_PER_SECOND 300.0
#define PLAYER_SPEED_LEFTRIGHT_PER_SECOND 300.0
#define PLAYER_SPEED_TOPBOTTOM_PER_SECOND 500.0

int GameDirector::getVirtualScreenSizeX() {
	return VIRTUAL_SCREEN_SIZE_X;
}
int GameDirector::getVirtualScreenSizeY() {
	return VIRTUAL_SCREEN_SIZE_Y;
}
int GameDirector::getBlockSizeX() {
	return BLOCK_SIZE_X;
}
int GameDirector::getBlockSizeY() {
	return BLOCK_SIZE_Y;
}
int GameDirector::getBlockCountX() {
	return BLOCK_COUNT_X;
}
int GameDirector::getBlockCountY() {
	return BLOCK_COUNT_Y;
}
float GameDirector::getEnemyLeftRightSpeedPerSecond() {
	return ENEMY_SPEED_LEFTRIGHT_PER_SECOND;
}
float GameDirector::getEnemyTopBottomSpeedPerSecond() {
	return ENEMY_SPEED_TOPBOTTOM_PER_SECOND;
}
float GameDirector::getPlayerSpeedLeftRightPerSecond() {
	return PLAYER_SPEED_LEFTRIGHT_PER_SECOND;
}
float GameDirector::getPlayerSpeedTopBottomPerSecond() {
	return PLAYER_SPEED_TOPBOTTOM_PER_SECOND;
}
