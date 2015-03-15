/*! \file gamestate.c
 *  \brief Gamestate management.
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
#include "gamestate.h"

struct Gamestate* AddNewGamestate(struct Game *game) {
	struct Gamestate *tmp = game->_priv.gamestates;
	if (!tmp) {
		game->_priv.gamestates = malloc(sizeof(struct Gamestate));
		tmp = game->_priv.gamestates;
	} else {
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = malloc(sizeof(struct Gamestate));
		tmp = tmp->next;
	}
	tmp->name = NULL;
	tmp->fade = false;
	tmp->fade_counter = 0;
	tmp->handle = NULL;
	tmp->loaded = false;
	tmp->paused = false;
	tmp->started = false;
	tmp->pending_load = false;
	tmp->pending_start = false;
	tmp->after = NULL;
	tmp->next = NULL;
	return tmp;
}

struct Gamestate* FindGamestate(struct Game *game, const char* name) {
	struct Gamestate *tmp = game->_priv.gamestates;
	while (tmp) {
		if (!strcmp(name, tmp->name)) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

void LoadGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (gs->loaded) {
			PrintConsole(game, "Gamestate \"%s\" already loaded.", name);
			return;
		}
		gs->pending_load = true;
	} else {
		gs = AddNewGamestate(game);
		gs->name = strdup(name);
		gs->fade = true;
		gs->fade_counter = 0;
		gs->pending_load = true;
		gs->showLoading = true;
	}
	PrintConsole(game, "Gamestate \"%s\" marked to be LOADED.", name);
}

void UnloadGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (!gs->loaded) {
			PrintConsole(game, "Gamestate \"%s\" already unloaded.", name);
			return;
		}
		if (gs->started) gs->pending_start=true;
		gs->pending_load = true;
		PrintConsole(game, "Gamestate \"%s\" marked to be UNLOADED.", name);
	} else {
		PrintConsole(game, "Tried to unload nonexisitent gamestate \"%s\"", name);
	}
}

void StartGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (gs->started) {
			PrintConsole(game, "Gamestate \"%s\" already started.", name);
			return;
		}
		gs->pending_start = true;
		PrintConsole(game, "Gamestate \"%s\" marked to be STARTED.", name);
	} else {
		PrintConsole(game, "Tried to start nonexisitent gamestate \"%s\"", name);
	}
}

void StopGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (!gs->started) {
			PrintConsole(game, "Gamestate \"%s\" already stopped.", name);
			return;
		}
		gs->pending_start = true;
		PrintConsole(game, "Gamestate \"%s\" marked to be STOPPED.", name);
	} else {
		PrintConsole(game, "Tried to stop nonexisitent gamestate \"%s\"", name);
	}
}

void PauseGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (!gs->started) {
			PrintConsole(game, "Tried to pause gamestate \"%s\" which is not started.", name);
			return;
		}
		if (gs->paused) {
			PrintConsole(game, "Gamestate \"%s\" already paused.", name);
			return;
		}
		gs->paused = true;
		PrintConsole(game, "Gamestate \"%s\" paused.", name);
	} else {
		PrintConsole(game, "Tried to pause nonexisitent gamestate \"%s\"", name);
	}
}

void ResumeGamestate(struct Game *game, const char* name) {
	struct Gamestate *gs = FindGamestate(game, name);
	if (gs) {
		if (!gs->started) {
			PrintConsole(game, "Tried to resume gamestate \"%s\" which is not started.", name);
			return;
		}
		if (!gs->paused) {
			PrintConsole(game, "Gamestate \"%s\" already resumed.", name);
			return;
		}
		gs->paused = false;
		PrintConsole(game, "Gamestate \"%s\" resumed.", name);
	} else {
		PrintConsole(game, "Tried to resume nonexisitent gamestate \"%s\"", name);
	}
}

void SwitchGamestate(struct Game *game, const char* current, const char* n) {
	StopGamestate(game, current);
	UnloadGamestate(game, current);
	LoadGamestate(game, n);
	StartGamestate(game, n);
}
