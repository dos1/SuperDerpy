/*! \file level4.c
 *  \brief Level 4 code.
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
#include "level4.h"

void Level4_Load(struct Game *game) {
	Moonwalk_Load(game);
	TM_AddAction(&DoMoonwalk, NULL, "moonwalk");
	TM_AddAction(&PassLevel, NULL, "passlevel");
	TM_AddBackgroundAction(&ShowMeter, NULL, 0, "showmeter");
	FadeGameState(game, true);
}

void Level4_Unload(struct Game *game) {
	Moonwalk_Unload(game);
}

void Level4_UnloadBitmaps(struct Game *game) {
	Moonwalk_UnloadBitmaps(game);
}

void Level4_Preload(struct Game *game) {
	Moonwalk_Preload(game);
}

inline int Level4_PreloadSteps(void) {
	return 0+Moonwalk_PreloadSteps();
}

void Level4_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	//PROGRESS_INIT(Level4_PreloadSteps());
	Moonwalk_PreloadBitmaps(game, progress);
}

void Level4_Draw(struct Game *game) {
	Moonwalk_Draw(game);
}

void Level4_Logic(struct Game *game) {
	Moonwalk_Logic(game);
}

void Level4_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_Keydown(game, ev);
}

void Level4_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_ProcessEvent(game, ev);
}

void Level4_Resume(struct Game *game) {
	Moonwalk_Resume(game);
}

void Level4_Pause(struct Game *game) {
	Moonwalk_Pause(game);
}
