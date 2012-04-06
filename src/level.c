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
#include "moonwalk.h"
#include "config.h"
#include "pause.h"
#include "level.h"

void Level_Passed(struct Game *game) {
	if (game->level.current_level<6) {
		int available = atoi(GetConfigOptionDefault("MuffinAttack", "level", "1"));
		available++;
		if ((available<2) || (available>7)) available=1;
		if (available==(game->level.current_level+1)) {
			char* text = malloc(2*sizeof(char));
			sprintf(text, "%d", available);
			SetConfigOption("MuffinAttack", "level", text);
			free(text);
		}
	} else {
		SetConfigOption("MuffinAttack", "completed", "1");
	}
}

void Level_Draw(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_Draw(game);
	else {
		al_clear_to_color(al_map_rgb(0,0,0));
	}
}

void Level_Load(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_Load(game);
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level!=1) Moonwalk_Keydown(game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	}
	return 0;
}

void Level_Preload(struct Game *game) {
	Pause_Preload(game);
	if (game->level.current_level!=1) Moonwalk_Preload(game);
}

void Level_Unload(struct Game *game) {
	Pause_Unload_Real(game);
	if (game->level.current_level!=1) Moonwalk_Unload(game);
	else Level_Passed(game);
}

void Level_UnloadBitmaps(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_UnloadBitmaps(game);
}

void Level_PreloadBitmaps(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game);
}
