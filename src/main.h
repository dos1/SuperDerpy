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
	GAMESTATE_LEVEL,
	GAMESTATE_DISCLAIMER
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
		ALLEGRO_BITMAP *image; /*!< Bitmap with lower portion of menu landscape. */
		ALLEGRO_BITMAP *cloud; /*!< Bitmap with bigger cloud. */
		ALLEGRO_BITMAP *cloud2; /*!< Bitmap with small cloud. */
		ALLEGRO_BITMAP *pie; /*!< Unscaled bitmap with pie. */
		ALLEGRO_BITMAP *pie_bitmap; /*!< Scaled and "rendered" bitmap with pies. */
		ALLEGRO_BITMAP *pinkcloud_bitmap; /*!< Scaled bitmap with pinkcloud and home. */
		ALLEGRO_BITMAP *pinkcloud; /*!< Unscaled bitmap with pinkcloud and home. */
		ALLEGRO_BITMAP *rain; /*!< Unscaled bitmap with rain drop. */
		ALLEGRO_BITMAP *rain_bitmap; /*!< Scaled and "rendered" bitmap with rain drops. */
		ALLEGRO_BITMAP *mountain; /*!< Flashing mountain in background bitmap. */
		ALLEGRO_BITMAP *logo; /*!< Logo displayed in the background. */
		ALLEGRO_BITMAP *logoblur; /*!< Prerendered blurred logo. */
		ALLEGRO_BITMAP *glass; /*!< Texture used for glass effect in the logo. */
		ALLEGRO_BITMAP *blurbg; /*!< Temporary bitmap used for blur effect in glass logo. */
		ALLEGRO_BITMAP *blurbg2; /*!< Temporary bitmap used for blur effect in glass logo. */
		float cloud_position; /*!< Position of bigger cloud. */
		float cloud2_position; /*!< Position of small cloud. */
		int mountain_position; /*!< Position of flashing mountain. */
		ALLEGRO_SAMPLE *sample; /*!< Background music sample. */
		ALLEGRO_SAMPLE *rain_sample; /*!< Rain sound sample. */
		ALLEGRO_SAMPLE *click_sample; /*!< Click sound sample. */
		ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
		ALLEGRO_SAMPLE_INSTANCE *rain_sound; /*!< Sample instance with rain sound. */
		ALLEGRO_SAMPLE_INSTANCE *click; /*!< Sample instance with click sound. */
		ALLEGRO_FONT *font_title; /*!< Font of "Super Derpy" text. */
		ALLEGRO_FONT *font_subtitle; /*!< Font of "Muffin Attack" text. */
		ALLEGRO_FONT *font; /*!< Font of standard menu item. */
		ALLEGRO_FONT *font_selected; /*!< Font of selected menu item. */
		int selected; /*!< Number of selected menu item. */
		enum menustate_enum menustate; /*!< Current menu page. */
		bool loaded; /*!< True if Menu state has been already loaded. */
		struct {
				bool fullscreen;
				int fps;
				int width;
				int height;
		} options; /*!< Options which can be changed in menu. */
};

/*! \brief Resources used by Loading state. */
struct Loading {
		ALLEGRO_BITMAP *loading_bitmap; /*!< Rendered loading bitmap. */
		ALLEGRO_BITMAP *image; /*!< Loading background. */
};


/*! \brief Main struct of the game. */
struct Game {
		ALLEGRO_DISPLAY *display; /*!< Main Allegro display. */
		ALLEGRO_FONT *font; /*!< Main font used in game. */
		ALLEGRO_FONT *font_console; /*!< Font used in game console. */
		enum gamestate_enum gamestate; /*!< Current game state. */
		enum gamestate_enum loadstate; /*!< Game state to be loaded. */
		ALLEGRO_EVENT_QUEUE *event_queue; /*!< Main event queue. */
		ALLEGRO_TIMER *timer; /*!< Main LPS timer. */
		ALLEGRO_BITMAP *console; /*!< Bitmap with game console. */
		int viewportWidth; /*!< Actual available width of viewport. */
		int viewportHeight; /*!< Actual available height of viewport. */
		bool showconsole; /*!< If true, game console is rendered on screen. */
		int fx; /*!< Effects volume. */
		int music; /*!< Music volume. */
		int voice; /*!< Voice volume. */
		bool fullscreen; /*!< Fullscreen toggle. */
		bool debug; /*!< Toggles debug mode. */
		int fps; /*!< FPS limit */
		int width; /*!< Width of window as being set in configuration. */
		int height; /*!< Height of window as being set in configuration. */
		bool shuttingdown; /*!< If true then shut down of the game is pending. */
		bool restart; /*!< If true then restart of the game is pending. */
		struct Menu menu; /*!< Resources used by Menu state. */
		struct Loading loading; /*!< Resources used by Menu state. */
		struct {
				ALLEGRO_VOICE *v; /*!< Main voice used by the game. */
				ALLEGRO_MIXER *mixer; /*!< Main mixer of the game. */
				ALLEGRO_MIXER *music; /*!< Music mixer. */
				ALLEGRO_MIXER *voice; /*!< Voice mixer. */
				ALLEGRO_MIXER *fx; /*!< Effects mixer. */
		} audio; /*!< Audio resources. */
		struct {
				double old_time, fps;
				int frames_done;
		} fps_count;
};

/*! \brief Preloads gamestate set in game->loadstate. */
void PreloadGameState(struct Game *game, void (*progress)(struct Game*, float));

/*! \brief Unloads gamestate set in game->gamestate. */
void UnloadGameState(struct Game *game);

/*! \brief Loads gamestate set in game->loadstate. */
void LoadGameState(struct Game *game);

/*! \brief Pauses gamestate set in game->loadstate. */
void PauseGameState(struct Game *game);

/*! \brief Resumes gamestate set in game->loadstate. */
void ResumeGameState(struct Game *game);

/*! \brief Finds path for data file. */
char* GetDataFilePath(char* filename);

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

/*! \brief Draws frame from current gamestate. */
void DrawGameState(struct Game *game);

/*! \brief Processes logic of current gamestate. */
void LogicGameState(struct Game *game);

/*! \brief Displays fade in or fade out animation on current gamestate. */
void FadeGameState(struct Game *game, bool in);

/*! \brief Load shared resources. */
int Shared_Load(struct Game *game);

/*! \brief Unload shared resources. */
void Shared_Unload(struct Game *game);

/*! \brief Setups letterbox viewport if necessary. */
void SetupViewport(struct Game *game);

#endif
