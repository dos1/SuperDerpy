#ifndef MAIN_H
#define MAIN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum gamestate_enum {
	GAMESTATE_LOADING,
	GAMESTATE_MENU,
	GAMESTATE_ABOUT,
	GAMESTATE_INTRO,
	GAMESTATE_MAP,
	GAMESTATE_LEVEL
};

struct Level {
	ALLEGRO_BITMAP *fade_bitmap, *image;
	ALLEGRO_SAMPLE *sample;
	int current_level;
};

struct Menu {
	ALLEGRO_BITMAP *menu_bitmap, *menu_fade_bitmap, *image;
	ALLEGRO_BITMAP *cloud_bitmap, *cloud, *cloud2_bitmap, *cloud2, *pie, *pie_bitmap;
	ALLEGRO_BITMAP *pinkcloud_bitmap, *pinkcloud, *rain;
	ALLEGRO_BITMAP *mountain_bitmap, *mountain;
	float cloud_position, cloud2_position;
	int mountain_position;
	ALLEGRO_SAMPLE *sample, *rain_sample, *click_sample;
	ALLEGRO_FONT *font_title, *font_subtitle, *font, *font_selected;
	int selected;
	bool options;
};

struct Loading {
	ALLEGRO_BITMAP *loading_bitmap, *image;
};

struct About {
	ALLEGRO_BITMAP *fade_bitmap, *image, *text_bitmap, *letter;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_FONT *font;
	float x;
};

struct Map {
	ALLEGRO_BITMAP *map, *background, *map_bg, *highlight, *arrow;
	int selected, available;
	float arrowpos;
	ALLEGRO_SAMPLE *sample, *click_sample;
};

struct Intro {
	int position;
	int page;
	bool in_animation;
	ALLEGRO_BITMAP *table, *table_bitmap;
	ALLEGRO_FONT *font;
	ALLEGRO_SAMPLE *sample;
};

struct Game {
	ALLEGRO_DISPLAY *display;
	ALLEGRO_FONT *font, *font_console;
	enum gamestate_enum gamestate;
	enum gamestate_enum loadstate;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *console;
	bool showconsole;
	struct Menu menu;
	struct Loading loading;
	struct Intro intro;
	struct About about;
	struct Map map;
	struct Level level;
};

void PreloadGameState(struct Game *game);
void UnloadGameState(struct Game *game);
void LoadGameState(struct Game *game);
void PrintConsole(struct Game *game, char* format, ...);
void DrawConsole(struct Game *game);

#endif