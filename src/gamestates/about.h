/*! \file about.h
 *  \brief About screen headers.
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

struct AboutResources {
	ALLEGRO_BITMAP *image; /*!< Background bitmap. */
	ALLEGRO_BITMAP *text_bitmap; /*!< Bitmap with scrolled text. */
	ALLEGRO_BITMAP *letter; /*!< Paper bitmap. */
	ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
	ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
	ALLEGRO_FONT *font; /*!< Font used in the text on letter. */
	float x; /*!< Horizontal position of the text. */
	int fadeloop; /*!< Loop counter used in fades. */
};
