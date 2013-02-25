/*! \file level2.c
 *  \brief Level 2 code.
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
#include "allegro5/allegro_ttf.h"
#include "../gamestates/level.h"
#include "modules/moonwalk.h"
#include "../timeline.h"
#include "actions.h"
#include "../utils.h"
#include "level2.h"


void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {

	struct Level2Resources *data = malloc(sizeof(struct Level2Resources));
	data->moonwalk = Moonwalk_Load(game, 2);
	(*progress)(game);

	struct TM_Arguments *args = TM_AddToArgs(NULL, strdup("level2"));
	int* level = malloc(sizeof(int));
	*level=2;
	TM_AddToArgs(args, level);
	TM_AddAction(&PassLevel, args, "passlevel");
	(*progress)(game);

	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.09,0 );
	(*progress)(game);

	return data;
}

void Gamestate_Unload(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Unload(game, data->moonwalk);
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Start(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Start(game, data->moonwalk);
}

void Gamestate_Stop(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Stop(game, data->moonwalk);
}

void Gamestate_Draw(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Draw(game, data->moonwalk);

	al_draw_textf(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", 2);
	al_draw_text(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
}

void Gamestate_Logic(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Logic(game, data->moonwalk);
}

void Gamestate_ProcessEvent(struct Game *game, struct Level2Resources* data, ALLEGRO_EVENT *ev) {
	Moonwalk_ProcessEvent(game, data->moonwalk, ev);
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			SwitchGamestate(game, "level2", "menu");
		}
	}
}

void Gamestate_Resume(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Resume(game, data->moonwalk);
}

void Gamestate_Pause(struct Game *game, struct Level2Resources* data) {
	Moonwalk_Pause(game, data->moonwalk);
}

void Gamestate_Reload(struct Game *game, struct Level2Resources* data) {}

int Gamestate_ProgressCount = 3;
