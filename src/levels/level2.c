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
#include "../gamestates/level.h"
#include "modules/moonwalk.h"
#include "../timeline.h"
#include "actions.h"
#include "level2.h"

void Gamestate_Load(struct Game *game) {
	Moonwalk_Load(game);
	TM_Init(game);
	TM_AddAction(&DoMoonwalk, NULL, "moonwalk");
	TM_AddAction(&PassLevel, NULL, "passlevel");
	//FadeGameState(game, true);
}

void Gamestate_Unload(struct Game *game) {
	Moonwalk_Unload(game);
}

void Gamestate_UnloadBitmaps(struct Game *game) {
	Moonwalk_UnloadBitmaps(game);
}

void Gamestate_Start(struct Game *game) {
	Moonwalk_Start(game);
}

void Gamestate_Stop(struct Game *game) {
	Moonwalk_Stop(game);
}

void Gamestate_LoadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	//PROGRESS_INIT(Level2_PreloadSteps());
	Moonwalk_LoadBitmaps(game, progress);
}

void Gamestate_Draw(struct Game *game) {
	Moonwalk_Draw(game);
}

void Gamestate_Logic(struct Game *game) {
	Moonwalk_Logic(game);
}

void Gamestate_ProcessEvent(struct Game *game, void* data, ALLEGRO_EVENT *ev) {
	Moonwalk_ProcessEvent(game, ev);
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			SwitchGamestate(game, "level2", "menu");
		}
	}
}

void Gamestate_Resume(struct Game *game) {
	Moonwalk_Resume(game);
}

void Gamestate_Pause(struct Game *game) {
	Moonwalk_Pause(game);
}

void Gamestate_Reload(struct Game *game) {}

int Gamestate_ProgressCount = 0;
