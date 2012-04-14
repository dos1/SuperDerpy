/*! \file timeline.h
 *  \brief Timeline Manager framework.
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

#include "main.h"

enum TM_ActionState {
	TM_ACTIONSTATE_INIT,
	TM_ACTIONSTATE_RUNNING,
	TM_ACTIONSTATE_DESTROY
};

struct TM_Arguments {
	void *value;
	struct TM_Arguments *next;
};

struct TM_Action {
	bool (*function)(struct Game*, struct TM_Action*, enum TM_ActionState);
	struct TM_Arguments *arguments;
	ALLEGRO_TIMER *timer;
	bool active;
	int delay;
	struct TM_Action *next;
	unsigned int id;
	char* name;
};

void TM_Init(struct Game* game);
void TM_Process();
void TM_HandleEvent(ALLEGRO_EVENT *ev);
struct TM_Action* TM_AddAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, char* name);
struct TM_Action* TM_AddBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name);
struct TM_Action* TM_AddQueuedBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name);
void TM_AddDelay(float delay);
void TM_Pause(bool pause);
void TM_Destroy();
struct TM_Arguments* TM_AddToArgs(struct TM_Arguments* args, void* arg);
void TM_DestroyArgs(struct TM_Arguments* args);
bool TM_Initialized();
