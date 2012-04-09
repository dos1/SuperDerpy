/*! \file level.c
 *  \brief Playable Level code.
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
#include <math.h>
#include "level.h"
#include "moonwalk.h"

void Moonwalk_Draw(struct Game *game) {
	al_set_target_bitmap(game->level.derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(game->level.derpy_walkcycle,al_get_bitmap_width(game->level.derpy)*(game->level.moonwalk.derpy_frame%6),al_get_bitmap_height(game->level.derpy)*(game->level.moonwalk.derpy_frame/6),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	game->level.moonwalk.derpy_pos=game->level.moonwalk.derpy_pos+tps(game, 60*0.00092);
	if (game->level.moonwalk.derpy_pos>1) { UnloadGameState(game);
		Level_Passed(game);
		if (game->level.current_level<6) {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_MAP;
			//LoadGameState(game);
		} else {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_ABOUT;
			//LoadGameState(game);
		}
		return; }
	int i;
	for (i = 0; i < tps(game, 60); i++ ) {
	game->level.moonwalk.derpy_frame_tmp++;
	if (game->level.moonwalk.derpy_frame_tmp%3==0) {
		if (game->level.moonwalk.derpy_frame_tmp%5==0) game->level.moonwalk.derpy_frame++;
                if (game->level.moonwalk.derpy_frame_tmp%22==21) game->level.moonwalk.derpy_frame--;
		game->level.moonwalk.derpy_frame++;
		if (game->level.moonwalk.derpy_frame>=24) game->level.moonwalk.derpy_frame=0;
	}
	}
	al_draw_scaled_bitmap(game->level.moonwalk.image,0,0,al_get_bitmap_width(game->level.moonwalk.image),al_get_bitmap_height(game->level.moonwalk.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap(game->level.derpy, game->level.moonwalk.derpy_pos*al_get_display_width(game->display), al_get_display_height(game->display)-al_get_bitmap_height(game->level.derpy), ALLEGRO_FLIP_HORIZONTAL);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
}

void Moonwalk_Load(struct Game *game) {
	game->level.moonwalk.derpy_frame = 0;
	game->level.moonwalk.derpy_frame_tmp = 0;
	game->level.moonwalk.derpy_pos = -0.2;
	ALLEGRO_EVENT ev;
	int fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.moonwalk.fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.moonwalk.fade_bitmap);
	Moonwalk_Draw(game);
}

int Moonwalk_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	return 0;
}

void Moonwalk_PreloadBitmaps(struct Game *game) {
	game->level.moonwalk.image =LoadScaledBitmap("table.png", al_get_display_width(game->display), al_get_display_height(game->display));
	game->level.derpy_walkcycle = LoadScaledBitmap("derpcycle.png", al_get_display_width(game->display)*0.1953125*6, al_get_display_height(game->display)*0.25*4);

	game->level.derpy = al_create_bitmap(al_get_display_width(game->display)*0.1953125, al_get_display_height(game->display)*0.25);
	
	game->level.moonwalk.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.moonwalk.fade_bitmap);
	al_draw_bitmap(game->level.moonwalk.image,0,0,0);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
	al_set_target_bitmap(al_get_backbuffer(game->display));
}

void Moonwalk_Preload(struct Game *game) {
	PrintConsole(game, "Initializing level %d...", game->level.current_level);
	game->level.sample = al_load_sample( "data/moonwalk.flac" );
	game->level.music = al_create_sample_instance(game->level.sample);
	al_attach_sample_instance_to_mixer(game->level.music, game->audio.music);
	al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

	if (!game->level.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
}

void Moonwalk_UnloadBitmaps(struct Game *game) {
	al_destroy_bitmap(game->level.moonwalk.image);
}

void Moonwalk_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	game->level.moonwalk.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.moonwalk.fade_bitmap);
	al_draw_bitmap(game->level.moonwalk.image,0,0,0);
	al_draw_bitmap(game->level.derpy, game->level.moonwalk.derpy_pos*al_get_display_width(game->display), al_get_display_height(game->display)-al_get_bitmap_height(game->level.derpy), ALLEGRO_FLIP_HORIZONTAL);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	int fadeloop;
	for(fadeloop=255; fadeloop>=0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.moonwalk.fade_bitmap, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.moonwalk.fade_bitmap);
}
