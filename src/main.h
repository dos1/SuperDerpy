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
#include "allegro_utils.h"

/*! \brief Declares variables used by displaying progress bar on loading screen.
 * Takes number of loading steps as parameter.
 */
#define PROGRESS_INIT(a) float load_p = 0, load_a = a;
/*! \brief Increments progress of loading. */
#define PROGRESS if (progress) (*progress)(game, load_p+=1/load_a);

struct Game;

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

/*! \brief Structure representing obstacles and power-ups flying through the level. */
struct Obstacle {
	ALLEGRO_BITMAP **bitmap; /*!< Pointer to bitmap used by obstacle. */
	float x; /*!< Horizontal position on the screen, in range 0-1. */
	float y; /*!< Vertical position on the screen, in range 0-1. */
	float speed; /*!< Horizontal speed of obstracle. */
	int points; /*!< Number of points given when hit by player. Positive gives HP to power, negative takes it. */
	bool hit; /*!< Indicates if this obstacle was already hit by the player or not. */
	void (*callback)(struct Game*, struct Obstacle*); /*!< Pointer to function called to update obstacle position, animate it, etc. */
	void *data; /*!< Pointer passed to callback function. */
	struct Obstacle *prev; /*!< Previous obstacle on the list. */
	struct Obstacle *next; /*!< Next obstacle on the list. */
};

struct Spritesheet {
	char* name; /*!< Readable name of the spritesheet. */
	ALLEGRO_BITMAP* bitmap; /*!< Spritesheet bitmap. */
	int rows; /*!< Number of rows in the spritesheet. */
	int cols; /*!< Number of columns in the spritesheet. */
	int blanks; /*!< Number of blank frames at the end of the spritesheet. */
	float speed; /*!< Speed modifier of spritesheet animation. */
	float aspect; /*!< Aspect ratio of the frame. */
	float scale; /*!< Scale modifier of the frame. */
	struct Spritesheet* next; /*!< Next spritesheet in the queue. */
};

/*! \brief Resources used by Level state. */
struct Level {
	int current_level; /*!< Level number. */
	float speed; /*!< Speed of the player. */
	float bg_pos; /*!< Position of the background layer of the scene. */
	float st_pos; /*!< Position of the stage layer of the scene. */
	float fg_pos; /*!< Position of the foreground layer of the scene. */
	float cl_pos; /*!< Position of the clouds layer of the scene. */
	float derpy_x; /*!< Horizontal position of Derpy (0-1). */
	float derpy_y; /*!< Vertical position of Derpy (0-1). */
	float hp; /*!< Player health points (0-1). */
	bool handle_input; /*!< When false, player looses control over Derpy. */
	bool failed; /*!< Indicates if player failed level. */
	float meter_alpha; /*!< Alpha level of HP meter. */
	int sheet_rows; /*!< Number of rows in current spritesheet. */
	int sheet_cols; /*!< Number of cols in current spritesheet. */
	int sheet_pos; /*!< Frame position in current spritesheet. */
	int sheet_blanks; /*!< Number of blank frames at the end of current spritesheet. */
	float sheet_tmp; /*!< Temporary counter used to slow down spritesheet animation. */
	float sheet_speed; /*!< Current speed of Derpy animation. */
	float sheet_speed_modifier; /*!< Modifier of speed, specified by current spritesheet. */
	float sheet_scale; /*!< Scale modifier of current spritesheet. */
	ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	unsigned int music_pos; /*!< Position of sample instance. Used when pausing game. */
	ALLEGRO_BITMAP *background; /*!< Bitmap of the background layer of the scene. */
	ALLEGRO_BITMAP *stage; /*!< Bitmap of the stage layer of the scene. */
	ALLEGRO_BITMAP *foreground; /*!< Bitmap of the foreground layer of the scene. */
	ALLEGRO_BITMAP *clouds; /*!< Bitmap of the clouds layer of the scene. */
	ALLEGRO_BITMAP *welcome; /*!< Bitmap of the welcome text (for instance "Level 1: Fluttershy"). */
	ALLEGRO_BITMAP **derpy_sheet; /*!< Pointer to active Derpy sprite sheet. */
	ALLEGRO_BITMAP *derpy; /*!< Derpy sprite. */
	ALLEGRO_BITMAP *meter_bmp; /*!< Bitmap of the HP meter. */
	ALLEGRO_BITMAP *meter_image; /*!< Derpy image used in the HP meter. */
	struct Spritesheet* derpy_sheets; /*!< List of sprite sheets of Derpy character. */
	struct Spritesheet* pony_sheets; /*!< List of sprite sheets of character rescued by Derpy. */
	struct {
		ALLEGRO_BITMAP *pie; /*!< Pie bitmap. */
		ALLEGRO_BITMAP *muffin; /*!< Muffin bitmap. */
	} obst_bmps; /*!< Obstacle bitmaps. */
	struct Obstacle *obstacles; /*!< List of obstacles being currently rendered. */
	struct Moonwalk moonwalk; /*!< Moonwalk placeholder data. */
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
	ALLEGRO_BITMAP *loading_bitmap; /*!< Rendered loading bitmap. */
	ALLEGRO_BITMAP *image; /*!< Loading background. */
};

/*! \brief Resources used by Pause state. */
struct Pause {
	ALLEGRO_BITMAP *bitmap; /*!< Bitmap with screenshot. */
	ALLEGRO_BITMAP *derpy; /*!< Derpy on foreground. */
};

/*! \brief Resources used by About state. */
struct About {
	ALLEGRO_BITMAP *fade_bitmap; /*!< Bitmap with screenshot, used in fades. */
	ALLEGRO_BITMAP *image; /*!< Background bitmap. */
	ALLEGRO_BITMAP *text_bitmap; /*!< Bitmap with scrolled text. */
	ALLEGRO_BITMAP *letter; /*!< Paper bitmap. */
	ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_FONT *font; /*!< Font used in the text on letter. */
	float x; /*!< Horizontal position of the text. */
	int fadeloop; /*!< Loop counter used in fades. */
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
	float arrowpos; /*!< Vertical position of the arrow. */
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

/*! \brief Preloads gamestate set in game->loadstate. */
void PreloadGameState(struct Game *game, void (*progress)(struct Game*, float));

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

/*! \brief Loads bitmap into memory and scales it with software linear filtering. */
ALLEGRO_BITMAP* LoadScaledBitmap(char* filename, int width, int height);

/*! \brief Draws console bitmap on screen. */
float tps(struct Game *game, float t);

/*! \brief Draws frame from current gamestate. */
void DrawGameState(struct Game *game);

/*! \brief Load shared resources. */
int Shared_Load(struct Game *game);

/*! \brief Unload shared resources. */
void Shared_Unload(struct Game *game);

#endif
