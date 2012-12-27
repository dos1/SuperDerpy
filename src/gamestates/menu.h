/*! \file menu.h
 *  \brief Main Menu view headers.
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

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

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
struct MenuResources {
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

//void DrawMenuState(struct Game *game);
//void ChangeMenuState(struct Game *game, enum menustate_enum state);
