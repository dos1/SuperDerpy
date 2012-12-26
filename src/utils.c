/*! \file allegro_utils.c
 *  \brief Helper functions for Allegro.
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

#include <allegro5/allegro_primitives.h>
#include "stdio.h"
#include "config.h"
#include "math.h"
#include "utils.h"

void DrawVerticalGradientRect(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = top},
		{.x = x + w, .y = y, .z = 0, .color = top},
		{.x = x, .y = y + h, .z = 0, .color = bottom},
		{.x = x + w, .y = y + h, .z = 0, .color = bottom}};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void DrawHorizontalGradientRect(float x, float y, float w, float h, ALLEGRO_COLOR left, ALLEGRO_COLOR right) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = left},
		{.x = x + w, .y = y, .z = 0, .color = right},
		{.x = x, .y = y + h, .z = 0, .color = left},
		{.x = x + w, .y = y + h, .z = 0, .color = right}};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void DrawTextWithShadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text) {
	al_draw_text(font, al_map_rgba(0,0,0,128), x+1, y+1, flags, text);
	al_draw_text(font, color, x, y, flags, text);
}

void FadeGamestate(struct Game *game, bool in) {
	//TODO: reimplement
/*	ALLEGRO_BITMAP* bitmap = al_create_bitmap(game->viewportWidth, game->viewportHeight);
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	float fadeloop;
	if (in) {
		fadeloop = 255;
	} else {
		fadeloop = 0;
	}
	while ((in && fadeloop>=0) || (!in && fadeloop<255)) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(game->event_queue, &ev);
		if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == game->timer)) {
			LogicGamestates(game);
			if (in) {
				fadeloop-=10;
			} else {
				fadeloop+=10;
			}
		}
		if (al_is_event_queue_empty(game->event_queue)) {
			DrawGamestates(game);
			al_draw_tinted_bitmap(bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
			DrawConsole(game);
			al_flip_display();
		}
	}
	al_destroy_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	if (in) {
		DrawGamestates(game);
	}*/
}

/*! \brief Scales bitmap using software linear filtering method to current target. */
void ScaleBitmap(ALLEGRO_BITMAP* source, int width, int height) {
	if ((al_get_bitmap_width(source)==width) && (al_get_bitmap_height(source)==height)) {
		al_draw_bitmap(source, 0, 0, 0);
		return;
	}
	int x, y;
	al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
	al_lock_bitmap(source, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

	/* linear filtering code written by SiegeLord */

	ALLEGRO_COLOR interpolate(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, float frac) {
		return al_map_rgba_f(c1.r + frac * (c2.r - c1.r),
												 c1.g + frac * (c2.g - c1.g),
												 c1.b + frac * (c2.b - c1.b),
												 c1.a + frac * (c2.a - c1.a));
	}

	for (y = 0; y < height; y++) {
		float pixy = ((float)y / height) * ((float)al_get_bitmap_height(source) - 1);
		float pixy_f = floor(pixy);
		for (x = 0; x < width; x++) {
			float pixx = ((float)x / width) * ((float)al_get_bitmap_width(source) - 1);
			float pixx_f = floor(pixx);

			ALLEGRO_COLOR a = al_get_pixel(source, pixx_f, pixy_f);
			ALLEGRO_COLOR b = al_get_pixel(source, pixx_f + 1, pixy_f);
			ALLEGRO_COLOR c = al_get_pixel(source, pixx_f, pixy_f + 1);
			ALLEGRO_COLOR d = al_get_pixel(source, pixx_f + 1, pixy_f + 1);

			ALLEGRO_COLOR ab = interpolate(a, b, pixx - pixx_f);
			ALLEGRO_COLOR cd = interpolate(c, d, pixx - pixx_f);
			ALLEGRO_COLOR result = interpolate(ab, cd, pixy - pixy_f);

			al_put_pixel(x, y, result);
		}
	}
	al_unlock_bitmap(al_get_target_bitmap());
	al_unlock_bitmap(source);
}

ALLEGRO_BITMAP* LoadScaledBitmap(struct Game *game, char* filename, int width, int height) {
	bool memoryscale = !atoi(GetConfigOptionDefault(game, "SuperDerpy", "GPU_scaling", "1"));
	ALLEGRO_BITMAP *source, *target = al_create_bitmap(width, height);
	al_set_target_bitmap(target);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	char* origfn = GetDataFilePath(filename);
	void GenerateBitmap() {
		if (memoryscale) al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

		source = al_load_bitmap( origfn );
		if (memoryscale) {
			al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
			ScaleBitmap(source, width, height);
		}
		else {
			al_draw_scaled_bitmap(source, 0, 0, al_get_bitmap_width(source), al_get_bitmap_height(source), 0, 0, width, height, 0);
		}
		/*al_save_bitmap(cachefn, target);
		PrintConsole(game, "Cache bitmap %s generated.", filename);*/
		al_destroy_bitmap(source);
	}

	/*source = al_load_bitmap( cachefn );
	if (source) {
		if ((al_get_bitmap_width(source)!=width) || (al_get_bitmap_height(source)!=height)) {
			al_destroy_bitmap(source);*/
	GenerateBitmap();
	free(origfn);
	return target;
	/*	}
		return source;
	} else GenerateBitmap();
	return target;*/
}


char* GetDataFilePath(char* filename) {

	//TODO: support for current game

	char *result = 0;

	if (al_filename_exists(filename)) {
		return strdup(filename);
	}

	char origfn[255] = "data/";
	strcat(origfn, filename);

	if (al_filename_exists(origfn)) {
		return strdup(origfn);
	}

	void TestPath(char* subpath) {
		ALLEGRO_PATH *tail = al_create_path(filename);
		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		ALLEGRO_PATH *data = al_create_path(subpath);
		al_join_paths(path, data);
		al_join_paths(path, tail);
		//printf("Testing for %s\n", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		if (al_filename_exists(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
			result = strdup(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		}
		al_destroy_path(tail);
		al_destroy_path(data);
		al_destroy_path(path);
	}
	TestPath("../share/superderpy/data/");
	TestPath("../data/");
	TestPath("../Resources/data/");
	TestPath("data/");

	if (!result) {
		printf("FATAL: Could not find data file: %s!\n", filename);
		exit(1);
	}
	return result;
}


void PrintConsole(struct Game *game, char* format, ...) {
	va_list vl;
	va_start(vl, format);
	char text[1024] = {};
	vsprintf(text, format, vl);
	va_end(vl);
	if (game->config.debug) { printf("%s\n", text); fflush(stdout); }
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->_priv.console), al_get_bitmap_height(game->_priv.console));
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->_priv.console, 0, al_get_bitmap_height(game->_priv.console)*0.2, al_get_bitmap_width(game->_priv.console), al_get_bitmap_height(game->_priv.console)*0.8, 0, 0, 0);
	al_draw_text(game->_priv.font_console, al_map_rgb(255,255,255), game->viewport.width*0.005, al_get_bitmap_height(game->_priv.console)*0.81, ALLEGRO_ALIGN_LEFT, text);
	al_set_target_bitmap(game->_priv.console);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap(con, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(con);
}
