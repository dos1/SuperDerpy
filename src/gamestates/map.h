/*! \file map.h
 *  \brief Map view headers.
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

/*! \brief Resources used by Map state. */
struct MapResources {
	ALLEGRO_BITMAP *map; /*!< Background table bitmap. */
	ALLEGRO_BITMAP *map_bg; /*!< Map bitmap. */
	ALLEGRO_BITMAP *highlight; /*!< Level highlights bitmap. */
	ALLEGRO_BITMAP *arrow; /*!< Arrow bitmap. */
	int selected; /*!< Number of currently selected level. */
	int available; /*!< Number of highest available level. */
	float arrowpos; /*!< Vertical position of the arrow. */
	ALLEGRO_SAMPLE *sample; /*!< Sample with backgrond music. */
	ALLEGRO_SAMPLE *click_sample; /*!< Sample with click sound. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_SAMPLE_INSTANCE *click; /*!< Sample instance with click sound. */
};
