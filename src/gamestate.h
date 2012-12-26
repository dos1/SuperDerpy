/*! \file allegro_utils.h
 *  \brief Headers of helper functions for Allegro.
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
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "allegro5/allegro.h"

struct Game;

struct Gamestate {
	char* name;
	void* handle;
	bool loaded, pending_load;
	bool started, pending_start;
	bool paused;
	bool fade; // TODO: or maybe should it be in API?
	unsigned char fade_counter;
	char** after; // TODO: and this one too?
	struct Gamestate* next;
	void* data;
	struct {
			void (*Gamestate_Draw)(struct Game *game, void* data);
			void (*Gamestate_Logic)(struct Game *game, void* data);

			void* (*Gamestate_Load)(struct Game *game, void (*progress)(struct Game *game));
			void (*Gamestate_Start)(struct Game *game, void* data);
			void (*Gamestate_Pause)(struct Game *game, void* data);
			void (*Gamestate_Resume)(struct Game *game, void* data);
			void (*Gamestate_Stop)(struct Game *game, void* data);
			void (*Gamestate_Unload)(struct Game *game, void* data);

			void (*Gamestate_ProcessEvent)(struct Game *game, void* data, ALLEGRO_EVENT *ev);
			void (*Gamestate_Keydown)(struct Game *game, void* data, ALLEGRO_EVENT *ev); // TODO: rly?
			void (*Gamestate_Reload)(struct Game *game, void* data);

			int *Gamestate_ProgressCount;
	} api;
};

void LoadGamestate(struct Game *game, const char* name);
void UnloadGamestate(struct Game *game, const char* name);
void StartGamestate(struct Game *game, const char* name);
void StopGamestate(struct Game *game, const char* name);
void PauseGamestate(struct Game *game, const char* name);
void ResumeGamestate(struct Game *game, const char* name);
void SwitchGamestate(struct Game *game, const char* current, const char* n);

#endif
