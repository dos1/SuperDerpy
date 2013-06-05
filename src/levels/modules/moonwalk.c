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
#include "../../gamestates/level.h"
#include "../../utils.h"
#include "moonwalk.h"

// TODO: maybe use Walk action instead
bool DoMoonwalk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct Character *derpy = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		SelectSpritesheet(game, derpy, "walk");
		SetCharacterPosition(game, derpy, -0.2, 0.71, 0);
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		MoveCharacter(game, derpy, 0.0009 , 0, 0);
		AnimateCharacter(game, derpy, 0.97);
		if (derpy->x > 1) { // FIXME: getter
			return true;
		}
	}
	return false;
}

void Moonwalk_Logic(struct Game *game, struct Moonwalk *data) {
	TM_Process();
}

void Moonwalk_Draw(struct Game *game, struct Moonwalk *data) {
	al_draw_scaled_bitmap(data->background,0,0,al_get_bitmap_width(data->background),al_get_bitmap_height(data->background),0,0,game->viewport.width, game->viewport.height, 0);
	DrawCharacter(game, data->derpy, al_map_rgba(255,255,255,255), ALLEGRO_FLIP_HORIZONTAL);
}

void Moonwalk_Start(struct Game *game, struct Moonwalk *data) {
	SelectSpritesheet(game, data->derpy, "walk");
	al_play_sample_instance(data->music);
	// TODO: find some way to restart Timeline
}


void load_bitmaps(struct Game *game, struct Moonwalk *data) {
	data->background = LoadScaledBitmap(game, "levels/moonwalk/disco.jpg", game->viewport.width, game->viewport.height);
	LoadSpritesheets(game, data->derpy);
}


void unload_bitmaps(struct Game *game, struct Moonwalk *data) {
	UnloadSpritesheets(game, data->derpy);
	al_destroy_bitmap(data->background);
}

struct Moonwalk* Moonwalk_Load(struct Game *game, int current_level) {
	struct Moonwalk *data = malloc(sizeof(struct Moonwalk));

	data->derpy = CreateCharacter(game, "derpy");

	RegisterSpritesheet(game, data->derpy, "walk");

	data->current_level = current_level;

	load_bitmaps(game, data);

	data->sample = al_load_sample( GetDataFilePath(game, "levels/moonwalk/moonwalk.flac") );

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);

	TM_Init(game);
	TM_AddAction(&DoMoonwalk, TM_AddToArgs(NULL, data->derpy), "moonwalk");

	return data;
}

void Moonwalk_Stop(struct Game *game, struct Moonwalk *data) {
	al_set_sample_instance_playing(data->music, false);
}

void Moonwalk_Unload(struct Game *game, struct Moonwalk *data) {
	unload_bitmaps(game, data);
	DestroyCharacter(game, data->derpy);
	free(data);
	TM_Destroy();
}

void Moonwalk_ProcessEvent(struct Game *game, struct Moonwalk *data, ALLEGRO_EVENT *ev) {}

void Moonwalk_Pause(struct Game *game, struct Moonwalk *data) {
	data->music_pos = al_get_sample_instance_position(data->music);
	al_set_sample_instance_playing(data->music, false);
	TM_Pause();
}

void Moonwalk_Resume(struct Game *game, struct Moonwalk *data) {
	al_set_sample_instance_position(data->music, data->music_pos);
	al_set_sample_instance_playing(data->music, true);
	TM_Resume();
}

void Moonwalk_Reload(struct Game *game, struct Moonwalk *data) {
	unload_bitmaps(game, data);
	load_bitmaps(game, data);
}
