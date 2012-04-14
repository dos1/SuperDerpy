/*! \file loading.c
 *  \brief Loading screen.
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
#include "loading.h"

void Loading_Draw(struct Game *game) {
	ALLEGRO_EVENT ev;
	int fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}

	al_draw_bitmap(game->loading.loading_bitmap,0,0,0);

	al_stop_timer(game->timer);

	PreloadGameState(game);

	al_start_timer(game->timer);

	for(fadeloop=255; fadeloop>0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_clear_to_color(al_map_rgb(0,0,0));
	DrawConsole(game);
	al_flip_display();
	LoadGameState(game);
}

void Loading_Load(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));

	game->loading.loading_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));

	game->loading.image = LoadScaledBitmap("loading.png", al_get_display_width(game->display), al_get_display_height(game->display));

	al_set_target_bitmap(game->loading.loading_bitmap);
	al_draw_bitmap(game->loading.image, 0, 0, 0);
	al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.0234, al_get_display_height(game->display)*0.85, ALLEGRO_ALIGN_LEFT, "Loading...");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(game->loading.image);
}

int Loading_Keydown(struct Game *game, ALLEGRO_EVENT *ev) { return 0; }
void Loading_Preload(struct Game *game) {}
void Loading_Unload(struct Game *game) { al_destroy_bitmap(game->loading.loading_bitmap); }
