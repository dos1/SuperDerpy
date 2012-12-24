/*! \file allegro_utils.c
 *  \brief Helper functions for Allegro.
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

#include "utils.h"

void LoadGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "load finished %s", name);
}

void UnloadGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "unload finished %s", name);
}

void StartGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "start finished %s", name);
}

void StopGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "stop finished %s", name);
}

void PauseGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "pause finished %s", name);
}

void ResumeGamestate(struct Game *game, const char* name) {
	PrintConsole(game, "resume finished %s", name);
}

void SwitchGamestate(struct Game *game, const char* current, const char* n) {
	StopGamestate(game, current);
	UnloadGamestate(game, current);
	LoadGamestate(game, n);
	StartGamestate(game, n);
}
