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
#ifndef TIMELINE_H
#define TIMELINE_H

#include "main.h"

/*! \brief State of the TM_Action. */
enum TM_ActionState {
	TM_ACTIONSTATE_INIT,
	TM_ACTIONSTATE_RUNNING,
	TM_ACTIONSTATE_DRAW,
	TM_ACTIONSTATE_DESTROY
};

/*! \brief Arguments for TM_Action. */
struct TM_Arguments {
		void *value; /*!< Value of argument. */
		struct TM_Arguments *next; /*!< Pointer to next argument. */
};

/*! \brief Timeline action. */
struct TM_Action {
		bool (*function)(struct Game*, struct TM_Action*, enum TM_ActionState); /*!< Function callback of the action. */
		struct TM_Arguments *arguments; /*!< Arguments of the action. */
		ALLEGRO_TIMER *timer; /*!< Delay timer. */
		bool active; /*!< If false, then this action is waiting for it's delay to finish. */
		int delay; /*!< Number of miliseconds to delay before action is started. */
		struct TM_Action *next; /*!< Pointer to next action in queue. */
		unsigned int id; /*!< ID of the action. */
		char* name; /*!< "User friendly" name of the action. */
};

/*! \brief Init timeline. */
void TM_Init(struct Game* game);
/*! \brief Process current timeline actions. */
void TM_Process();
/*! \brief Ask current timeline actions to draw. */
void TM_Draw();
/*! \brief Handle timer events. */
void TM_HandleEvent(ALLEGRO_EVENT *ev);
/*! \brief Add new action to main queue. */
struct TM_Action* TM_AddAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, char* name);
/*! \brief Add new action to background queue. */
struct TM_Action* TM_AddBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name);
/*! \brief Add new action to main queue, which adds specified action into background queue. */
struct TM_Action* TM_AddQueuedBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name);
/*! \brief Add delay to main queue. */
void TM_AddDelay(float delay);
/*! \brief Pause timers. */
void TM_Pause(bool pause);
/*! \brief Destroy timeline. */
void TM_Destroy();
/*! \brief Add data to TM_Arguments queue. */
struct TM_Arguments* TM_AddToArgs(struct TM_Arguments* args, void* arg);
/*! \brief Destroy TM_Arguments queue. */
void TM_DestroyArgs(struct TM_Arguments* args);
/*! \brief Check if timeline is initialised. */
bool TM_Initialized();

#endif
