/*! \file intro.h
 *  \brief Intro view headers.
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

/*! \brief Resources used by Intro state. */
struct IntroResources {
	int position; /*!< Position of the page. */
	int page; /*!< Current page number. */
	bool in_animation; /*!< Animation as in page transition animation. */
	float anim; /*!< Counter used for spritesheet animations. */
	ALLEGRO_BITMAP *table; /*!< Background paper bitmap, two pages long. */
	ALLEGRO_BITMAP *table_bitmap; /*!< Unscaled background paper bitmap. */
	ALLEGRO_BITMAP *frame; /*!< Bitmap with frame around the screen. */
	ALLEGRO_BITMAP *animsprites[5]; /*!< Array with spritesheet bitmaps. */
	ALLEGRO_FONT *font; /*!< Font used for text. */
	ALLEGRO_SAMPLE *sample; /*!< Background music sample. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_AUDIO_STREAM *audiostream; /*!< Audiostream used for Celestia voice. */
};
