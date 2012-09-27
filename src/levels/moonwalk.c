/*! \file moonwalk.c
 *  \brief Moonwalk Level module code.
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
#include "../level.h"
#include "moonwalk.h"

void Moonwalk_Logic(struct Game *game) {
	game->level.moonwalk.derpy_pos=game->level.moonwalk.derpy_pos+0.00092;
	game->level.moonwalk.derpy_frame_tmp++;
	if (game->level.moonwalk.derpy_frame_tmp%3==0) {
		if (game->level.moonwalk.derpy_frame_tmp%5==0) game->level.moonwalk.derpy_frame++;
		if (game->level.moonwalk.derpy_frame_tmp%22==21) game->level.moonwalk.derpy_frame--;
		game->level.moonwalk.derpy_frame++;
		if (game->level.moonwalk.derpy_frame>=24) game->level.moonwalk.derpy_frame=0;
	}
}

void Moonwalk_Draw(struct Game *game) {
	al_set_target_bitmap(game->level.derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(*(game->level.derpy_sheet),al_get_bitmap_width(game->level.derpy)*(game->level.moonwalk.derpy_frame%6),al_get_bitmap_height(game->level.derpy)*(game->level.moonwalk.derpy_frame/6),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	if (game->level.moonwalk.derpy_pos>1) {
		game->level.moonwalk.derpy_pos=-1;
		Level_Passed(game);
		UnloadGameState(game);
		if (game->level.current_level<6) {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_MAP;
		} else {
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_ABOUT;
		}
		return;
	}
	al_draw_scaled_bitmap(game->level.stage,0,0,al_get_bitmap_width(game->level.stage),al_get_bitmap_height(game->level.stage),0,0,game->viewportWidth, game->viewportHeight,0);
	al_draw_bitmap(game->level.derpy, game->level.moonwalk.derpy_pos*game->viewportWidth, game->viewportHeight*0.95-al_get_bitmap_height(game->level.derpy), ALLEGRO_FLIP_HORIZONTAL);
	al_draw_textf(game->font, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
}

void Moonwalk_Load(struct Game *game) {
	SelectDerpySpritesheet(game, "walk");
	game->level.moonwalk.derpy_frame = 0;
	game->level.moonwalk.derpy_frame_tmp = 0;
	game->level.moonwalk.derpy_pos = -0.2;
	al_play_sample_instance(game->level.music);
	FadeGameState(game, true);
}

int Moonwalk_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	return 0;
}

void Moonwalk_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	/*game->level.derpy_sheet = LoadScaledBitmap("levels/derpcycle.png", game->viewportWidth*0.1953125*6, game->viewportHeight*0.25*4);*/

	// nasty hack: overwrite level backgrounds
	al_destroy_bitmap(game->level.background);
	al_destroy_bitmap(game->level.foreground);
	al_destroy_bitmap(game->level.stage);
	al_destroy_bitmap(game->level.clouds);
	game->level.background=al_create_bitmap(0,0);
	game->level.foreground=al_create_bitmap(0,0);
	game->level.clouds=al_create_bitmap(0,0);
	game->level.stage = LoadScaledBitmap("levels/disco.jpg", game->viewportWidth, game->viewportHeight);

	al_set_target_bitmap(al_get_backbuffer(game->display));
}

void Moonwalk_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PrintConsole(game, "Initializing level %d...", game->level.current_level);
	game->level.sample = al_load_sample( GetDataFilePath("levels/moonwalk.flac") );
	//TODO: make music handling global
	game->level.music = al_create_sample_instance(game->level.sample);
	al_attach_sample_instance_to_mixer(game->level.music, game->audio.music);
	al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

	if (!game->level.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
}

void Moonwalk_UnloadBitmaps(struct Game *game) {
}

void Moonwalk_Unload(struct Game *game) {
	//FadeGameState(game, false);
}

void Moonwalk_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
}
