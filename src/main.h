/*! \file main.h
 *  \brief Headers of main file of SuperDerpy engine.
 *
 *   Contains basic functions shared by all views.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef MAIN_H
#define MAIN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/*! \brief Enum of all available gamestates. */
enum gamestate_enum {
	GAMESTATE_PAUSE,
	GAMESTATE_LOADING,
	GAMESTATE_MENU,
	GAMESTATE_ABOUT,
	GAMESTATE_INTRO,
	GAMESTATE_MAP,
	GAMESTATE_LEVEL
};

/*! \brief Resources used by moonwalk level placeholder. */
struct Moonwalk {
	ALLEGRO_BITMAP *fade_bitmap; /*!< Bitmap used on fade-in and fade-out animations. */
	ALLEGRO_BITMAP *image; /*!< Background texture. */
	int derpy_frame; /*!< Current frame of Derpy animation. */ 
	int derpy_frame_tmp; /*!< Counter used to slow down Derpy animation. */
	double derpy_pos; /*!< Position of Derpy on screen. */
};

struct Obstracle {
	ALLEGRO_BITMAP *bitmap;
	float x, y, speed;
	int points;
	void *callback;
	struct Obstracle *prev, *next;
};

/*! \brief Resources used by Level state. */
struct Level {
	int current_level; /*!< Level number. */
	float speed;
	float bg_pos, st_pos, fg_pos, cl_pos;
	float derpy_x, derpy_y;
	bool handle_input;
	int sheet_rows, sheet_cols, sheet_pos;
	float sheet_tmp, sheet_speed;
	ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	unsigned int music_pos; /*!< Position of sample instance. Used when pausing game. */
	ALLEGRO_BITMAP *background, *stage, *foreground, *clouds;
	ALLEGRO_BITMAP *welcome;
	ALLEGRO_BITMAP *derpy_walkcycle; /*!< Derpy walk cycle spritesheet. */
	ALLEGRO_BITMAP *derpy; /*!< Derpy sprite. */
	struct Obstracle *obstracles;
	struct Moonwalk moonwalk;
};

/*! \brief Enum of menu states in Menu and Pause game states. */
enum menustate_enum {
	MENUSTATE_MAIN,
	MENUSTATE_OPTIONS,
	MENUSTATE_CONTROLS,
	MENUSTATE_VIDEO,
	MENUSTATE_PAUSE,
	MENUSTATE_AUDIO
};

/*! \brief Resources used by Menu state. */
struct Menu {
	ALLEGRO_BITMAP *menu_fade_bitmap;
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *cloud;
	ALLEGRO_BITMAP *cloud2;
	ALLEGRO_BITMAP *pie;
	ALLEGRO_BITMAP *pie_bitmap;
	ALLEGRO_BITMAP *pinkcloud_bitmap;
	ALLEGRO_BITMAP *pinkcloud;
	ALLEGRO_BITMAP *rain;
	ALLEGRO_BITMAP *rain_bitmap;
	ALLEGRO_BITMAP *mountain;
	float cloud_position;
	float cloud2_position;
	int mountain_position;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_SAMPLE *rain_sample;
	ALLEGRO_SAMPLE *click_sample;
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_SAMPLE_INSTANCE *rain_sound; /*!< Sample instance with rain sound. */
	ALLEGRO_SAMPLE_INSTANCE *click; /*!< Sample instance with click sound. */
	ALLEGRO_FONT *font_title;
	ALLEGRO_FONT *font_subtitle;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font_selected;
	int selected;
	enum menustate_enum menustate;
	bool loaded;
	struct {
		bool fullscreen;
		int fps;
		int width;
		int height;
	} options;
};

/*! \brief Resources used by Loading state. */
struct Loading {
	ALLEGRO_BITMAP *loading_bitmap;
	ALLEGRO_BITMAP *image;
};

/*! \brief Resources used by Pause state. */
struct Pause {
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_BITMAP *derpy;
};

/*! \brief Resources used by About state. */
struct About {
	ALLEGRO_BITMAP *fade_bitmap;
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *text_bitmap;
	ALLEGRO_BITMAP *letter;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_FONT *font;
	float x;
	int fadeloop;
};

/*! \brief Resources used by Map state. */
struct Map {
	ALLEGRO_BITMAP *map;
	ALLEGRO_BITMAP *background;
	ALLEGRO_BITMAP *map_bg;
	ALLEGRO_BITMAP *highlight;
	ALLEGRO_BITMAP *arrow;
	int selected;
	int available;
	float arrowpos;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_SAMPLE *click_sample;
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_SAMPLE_INSTANCE *click; /*!< Sample instance with click sound. */
};

/*! \brief Resources used by Intro state. */
struct Intro {
	int position;
	int page;
	bool in_animation; /*!< Animation as in page transition animation. */
	float anim;
	ALLEGRO_BITMAP *table;
	ALLEGRO_BITMAP *table_bitmap;
	ALLEGRO_BITMAP *frame;
	ALLEGRO_BITMAP *animation;
	ALLEGRO_BITMAP *animsprites[5];
	ALLEGRO_FONT *font;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_AUDIO_STREAM *audiostream;
};

/*! \brief Resources used by Game state. */
struct Game {
	ALLEGRO_DISPLAY *display;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font_console;
	enum gamestate_enum gamestate;
	enum gamestate_enum loadstate;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *console;
	bool showconsole;
	int fx;
	int music;
	int voice;
	bool fullscreen;
	bool debug;
	int fps;
	int width;
	int height;
	bool shuttingdown;
	bool restart;
	struct Menu menu;
	struct Loading loading;
	struct Intro intro;
	struct About about;
	struct Map map;
	struct Level level;
	struct Pause pause;
	struct {
		ALLEGRO_VOICE *v;
		ALLEGRO_MIXER *mixer;
		ALLEGRO_MIXER *music;
		ALLEGRO_MIXER *voice;
		ALLEGRO_MIXER *fx;
	} audio;
};

/*! \brief Draws text with shadow.
 * 
 * Draws given text two times: once with color (0,0,0,128) and 1px off in both x and y axis,
 * and second time with actual given color and position.
 */
void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text);

/*! \brief Preloads gamestate set in game->loadstate. */
void PreloadGameState(struct Game *game);

/*! \brief Unloads gamestate set in game->gamestate. */
void UnloadGameState(struct Game *game);

/*! \brief Loads gamestate set in game->loadstate. */
void LoadGameState(struct Game *game);

/*! \brief Print some message on game console.
 * 
 * Draws message on console bitmap, so it'll be displayed when calling DrawConsole.
 * If game->debug is true, then it also prints given message on stdout.
 * It needs to be called in printf style.
 */
void PrintConsole(struct Game *game, char* format, ...);

/*! \brief Draws console bitmap on screen. */
void DrawConsole(struct Game *game);

ALLEGRO_BITMAP* LoadScaledBitmap(char* filename, int width, int height);
float tps(struct Game *game, float t);

void DrawGameState(struct Game *game);

int Shared_Load(struct Game *game);
void Shared_Unload(struct Game *game);

#endif
