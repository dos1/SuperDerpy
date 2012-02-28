/*! \file main.h
 *  \brief Headers of main file of SuperDerpy engine.
 *
 *   Contains basic functions shared by all views.
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
	GAMESTATE_LOADING,
	GAMESTATE_MENU,
	GAMESTATE_ABOUT,
	GAMESTATE_INTRO,
	GAMESTATE_MAP,
	GAMESTATE_LEVEL
};

/*! \brief Resources used by Level state. */
struct Level {
	ALLEGRO_BITMAP *fade_bitmap; /*!< Bitmap used on fade-in and fade-out animations. */
	ALLEGRO_BITMAP *image; /*!< Background texture. */
	ALLEGRO_BITMAP *derpy_walkcycle; /*!< Derpy walk cycle spritesheet. */
	ALLEGRO_BITMAP *derpy; /*!< Derpy sprite. */
	ALLEGRO_BITMAP *derpytmp; /*!< Unscaled Derpy sprite. */
	ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
	int current_level; /*!< Level number. */
	int derpy_frame; /*!< Current frame of Derpy animation. */ 
	int derpy_frame_tmp; /*!< Counter used to slow down Derpy animation. */
	double derpy_pos; /*!< Position of Derpy on screen. */
};

/*! \brief Resources used by Menu state. */
struct Menu {
	ALLEGRO_BITMAP *menu_bitmap;
	ALLEGRO_BITMAP *menu_fade_bitmap;
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *cloud_bitmap;
	ALLEGRO_BITMAP *cloud;
	ALLEGRO_BITMAP *cloud2_bitmap;
	ALLEGRO_BITMAP *cloud2;
	ALLEGRO_BITMAP *pie;
	ALLEGRO_BITMAP *pie_bitmap;
	ALLEGRO_BITMAP *pinkcloud_bitmap;
	ALLEGRO_BITMAP *pinkcloud;
	ALLEGRO_BITMAP *rain;
	ALLEGRO_BITMAP *mountain_bitmap;
	ALLEGRO_BITMAP *mountain;
	float cloud_position;
	float cloud2_position;
	int mountain_position;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_SAMPLE *rain_sample;
	ALLEGRO_SAMPLE *click_sample;
	ALLEGRO_FONT *font_title;
	ALLEGRO_FONT *font_subtitle;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font_selected;
	int selected;
	bool options;
	bool draw_while_fading;
};

/*! \brief Resources used by Loading state. */
struct Loading {
	ALLEGRO_BITMAP *loading_bitmap;
	ALLEGRO_BITMAP *image;
};

/*! \brief Resources used by About state. */
struct About {
	ALLEGRO_BITMAP *fade_bitmap;
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *text_bitmap;
	ALLEGRO_BITMAP *letter;
	ALLEGRO_SAMPLE *sample;
	ALLEGRO_FONT *font;
	float x;
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
};

/*! \brief Resources used by Intro state. */
struct Intro {
	int position;
	int page;
	bool in_animation;
	ALLEGRO_BITMAP *table;
	ALLEGRO_BITMAP *table_bitmap;
	ALLEGRO_FONT *font;
	ALLEGRO_SAMPLE *sample;
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
	bool fx;
	bool music;
	bool fullscreen;
	bool debug;
	int fps;
	int width;
	int height;
	struct Menu menu;
	struct Loading loading;
	struct Intro intro;
	struct About about;
	struct Map map;
	struct Level level;
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

#endif
