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
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

void al_draw_vertical_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom);
void al_draw_horizontal_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR left, ALLEGRO_COLOR right);
void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text);
