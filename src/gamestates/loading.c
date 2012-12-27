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
#include <stdio.h>
#include "../utils.h"
#include "loading.h"

void Progress(struct Game *game, float p) {
	if (game->debug) { printf("%f\n", p); fflush(stdout); }
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_draw_bitmap(game->loading.loading_bitmap,0,0,0);
	al_draw_filled_rectangle(0, game->viewportHeight*0.985, p*game->viewportWidth, game->viewportHeight, al_map_rgba(255,255,255,255));
	DrawConsole(game);
	al_flip_display();
}

void Loading_Draw(struct Game *game) {
	float fadeloop=0;
	while (fadeloop<256) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(game->event_queue, &ev);
		if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == game->timer)) {
			fadeloop+=10;
		}
		if (al_is_event_queue_empty(game->event_queue)) {
			al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			DrawConsole(game);
			al_flip_display();
		}
	}

	al_draw_bitmap(game->loading.loading_bitmap,0,0,0);

	al_stop_timer(game->timer);

	LoadGamestate(game, "");

	al_wait_for_vsync();
	al_start_timer(game->timer);

	fadeloop=0;
	while (fadeloop<256) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(game->event_queue, &ev);
		if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == game->timer)) {
			fadeloop+=10;
		}
		if (al_is_event_queue_empty(game->event_queue)) {
			al_draw_bitmap(game->loading.loading_bitmap,0,0,0);
			al_draw_filled_rectangle(0, game->viewportHeight*0.985, game->viewportWidth, game->viewportHeight, al_map_rgba(255,255,255,255));
			al_draw_filled_rectangle(0, 0, game->viewportWidth, game->viewportHeight, al_map_rgba_f(0,0,0,fadeloop/255.0));

			DrawConsole(game);
			al_flip_display();
		}
	}
	al_clear_to_color(al_map_rgb(0,0,0));
	DrawConsole(game);
	al_flip_display();
	StartGamestate(game, "");
}

void Loading_Load(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));

	game->loading.loading_bitmap = al_create_bitmap(game->viewportWidth, game->viewportHeight);

	game->loading.image = LoadScaledBitmap("loading.png", game->viewportHeight*2, game->viewportHeight);

	al_set_target_bitmap(game->loading.loading_bitmap);
	al_clear_to_color(al_map_rgb(193,225,218));
	al_draw_bitmap(game->loading.image, game->viewportWidth-al_get_bitmap_width(game->loading.image), 0, 0);
	al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), game->viewportWidth*0.0234, game->viewportHeight*0.84, ALLEGRO_ALIGN_LEFT, "Loading...");
	al_draw_filled_rectangle(0, game->viewportHeight*0.985, game->viewportWidth, game->viewportHeight, al_map_rgba(128,128,128,128));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(game->loading.image);
}

int Loading_Keydown(struct Game *game, ALLEGRO_EVENT *ev) { return 0; }
void Loading_Preload(struct Game *game, void (*progress)(struct Game*, float)) {}
void Loading_Unload(struct Game *game) { al_destroy_bitmap(game->loading.loading_bitmap); }
