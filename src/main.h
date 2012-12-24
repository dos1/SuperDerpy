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
#include "gamestate.h"

struct Gamestate;

/*! \brief Main struct of the game. */
struct Game {
		ALLEGRO_DISPLAY *display; /*!< Main Allegro display. */

		struct {
			int width; /*!< Actual available width of viewport. */
			int height; /*!< Actual available height of viewport. */
		} viewport;

		struct {
			int fx; /*!< Effects volume. */
			int music; /*!< Music volume. */
			int voice; /*!< Voice volume. */
			bool fullscreen; /*!< Fullscreen toggle. */
			bool debug; /*!< Toggles debug mode. */
			int fps; /*!< FPS limit */
			int width; /*!< Width of window as being set in configuration. */
			int height; /*!< Height of window as being set in configuration. */
			ALLEGRO_CONFIG *config;
		} config;

		struct {
				ALLEGRO_VOICE *v; /*!< Main voice used by the game. */
				ALLEGRO_MIXER *mixer; /*!< Main mixer of the game. */
				ALLEGRO_MIXER *music; /*!< Music mixer. */
				ALLEGRO_MIXER *voice; /*!< Voice mixer. */
				ALLEGRO_MIXER *fx; /*!< Effects mixer. */
		} audio; /*!< Audio resources. */

		struct {
				struct Gamestate *gamestate; /*!< Current gamestate. */
				struct Gamestate *gamestate_list; /*!< List of known gamestates. */
				ALLEGRO_FONT *font; /*!< Main font used in game. */
				ALLEGRO_FONT *font_console; /*!< Font used in game console. */
				ALLEGRO_BITMAP *console; /*!< Bitmap with game console. */
				ALLEGRO_EVENT_QUEUE *event_queue; /*!< Main event queue. */
				ALLEGRO_TIMER *timer; /*!< Main LPS timer. */
				bool showconsole; /*!< If true, game console is rendered on screen. */

				struct {
						double old_time, fps;
						int frames_done;
				} fps_count;

		} _priv; /*!< Private resources. Do not use in gamestates! */

		bool shuttingdown; /*!< If true then shut down of the game is pending. */
		bool restart; /*!< If true then restart of the game is pending. */

};

#endif
