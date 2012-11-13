/*! \file Level3.h
 *  \brief Level 3 headers.
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
#include "../main.h"

void Level3_Load(struct Game *game);
void Level3_Unload(struct Game *game);
void Level3_UnloadBitmaps(struct Game *game);
void Level3_Preload(struct Game *game);
void Level3_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float));
inline int Level3_PreloadSteps(void);
void Level3_Draw(struct Game *game);
void Level3_Logic(struct Game *game);
void Level3_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
void Level3_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev);
void Level3_Resume(struct Game *game);
void Level3_Pause(struct Game *game);
