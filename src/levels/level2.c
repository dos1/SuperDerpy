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

void Level2_Load(struct Game *game) {
	Moonwalk_Load(game);
	TM_AddAction(&DoMoonwalk, NULL, "moonwalk");
	TM_AddAction(&PassLevel, NULL, "passlevel");
	FadeGameState(game, true);
}

void Level2_Unload(struct Game *game) {
	Moonwalk_Unload(game);
}

void Level2_UnloadBitmaps(struct Game *game) {
	Moonwalk_UnloadBitmaps(game);
}

void Level2_Preload(struct Game *game) {
	Moonwalk_Preload(game);
}

inline int Level2_PreloadSteps() {
	return 0+Moonwalk_PreloadSteps();
}

void Level2_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	//PROGRESS_INIT(Level2_PreloadSteps());
	Moonwalk_PreloadBitmaps(game, progress);
}

void Level2_Draw(struct Game *game) {
	Moonwalk_Draw(game);
}

void Level2_Logic(struct Game *game) {
	Moonwalk_Logic(game);
}

void Level2_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_Keydown(game, ev);
}

void Level2_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_ProcessEvent(game, ev);
}

void Level2_Resume(struct Game *game) {
	Moonwalk_Resume(game);
}

void Level2_Pause(struct Game *game) {
	Moonwalk_Pause(game);
}
