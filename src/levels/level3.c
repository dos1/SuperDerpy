/*! \file Level3.c
 *  \brief Level 3 code.
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
#include "moonwalk.h"

void Level3_Load(struct Game *game) {
	Moonwalk_Load(game);
}

void Level3_Unload(struct Game *game) {
	Moonwalk_Unload(game);
}

void Level3_UnloadBitmaps(struct Game *game) {
	Moonwalk_UnloadBitmaps(game);
}

void Level3_Preload(struct Game *game) {
	Moonwalk_Preload(game);
}

inline int Level3_PreloadSteps() {
	return 0+Moonwalk_PreloadSteps();
}

void Level3_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	//PROGRESS_INIT(Level3_PreloadSteps());
	Moonwalk_PreloadBitmaps(game, progress);
}

void Level3_Draw(struct Game *game) {
	Moonwalk_Draw(game);
}

void Level3_Logic(struct Game *game) {
	Moonwalk_Logic(game);
}

void Level3_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_Keydown(game, ev);
}

void Level3_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	Moonwalk_ProcessEvent(game, ev);
}

void Level3_Resume(struct Game *game) {
	Moonwalk_Resume(game);
}

void Level3_Pause(struct Game *game) {
	Moonwalk_Pause(game);
}
