/*! \file timeline.c
 *  \brief Timeline Manager framework code.
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
#include <allegro5/allegro.h>
#include <stdio.h>
#include "timeline.h"

unsigned int lastid;
struct Game* game = NULL;
struct TM_Action *queue, *background;
bool paused;

void TM_Init(struct Game* g) {
	game = g;
	lastid = 0;
	queue = NULL;
	background = NULL;
	paused = false;
}

void TM_Process() {
	if (!game) return;
	if (paused) return;
	// process first element from queue
	//  if returns true, then delete it
	if (queue) {
		if (*queue->function) {
			queue->active = true;
			if ((*queue->function)(game, queue, TM_ACTIONSTATE_RUNNING)) {
				queue->active=false;
				(*queue->function)(game, queue, TM_ACTIONSTATE_DESTROY);
				struct TM_Action *tmp = queue;
				queue = queue->next;
				free(tmp);
			}
		} else {
			// TODO: handle delay
		}
	}
	// process all elements from background marked as active
	struct TM_Action *tmp, *tmp2, *pom = background;
	tmp = NULL;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) {
				if ((*pom->function)(game, pom, TM_ACTIONSTATE_RUNNING)) {
					pom->active=false;
					(*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
					if (tmp) {
						tmp->next = pom->next;
					} else {
						background = pom->next;
					}
				}
			} else {
				// TODO: handle delay
			}
		}
		if ((!tmp) || (tmp->next==pom)) {
			tmp = pom;
			pom = pom->next;
		} else {
			free(pom);
			tmp2 = tmp;
			if (!tmp) pom=background->next;
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
}

void TM_HandleEvent(ALLEGRO_EVENT *ev) {
	if (!game) return;
	if (paused) return;
	// TODO: find proper action and mark it as active
	
}

void TM_AddAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args) {
	struct TM_Action *action = malloc(sizeof(struct TM_Action));
	if (queue) {
		struct TM_Action *pom = queue;
		while (pom->next!=NULL) {
			pom=pom->next;
		}
		pom->next = action;
	} else {
		queue = action;
	}
	action->next = NULL;
	action->function = func;
	action->arguments = args;
	action->timer = NULL;
	action->active = false;
	action->delay = 0;
	(*action->function)(game, action, TM_ACTIONSTATE_INIT);
}

void TM_AddBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, int delay) {
	struct TM_Action *action = malloc(sizeof(struct TM_Action));
	if (background) {
		struct TM_Action *pom = background;
		while (pom->next!=NULL) {
			pom=pom->next;
		}
		pom->next = action;
	} else {
		background = action;
	}
	action->next = NULL;
	action->function = func;
	action->arguments = args;
	action->timer = NULL; // TODO
	action->active = true; // TODO: false here, true when delay
	action->delay = delay;
	(*action->function)(game, action, TM_ACTIONSTATE_INIT); // TODO: move to TM_HandleEvent
}

void TM_AddDelay(int delay) {
	TM_AddAction(NULL, NULL);
}

void TM_Pause(bool pause) {
	paused = pause;
	// TODO: pause timers
}

void TM_Destroy() {
	if (!game) return;
	// TODO: delete everything from queues
	// maybe delete all args too?
	struct TM_Action *tmp, *tmp2, *pom = queue;
	tmp = NULL;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) {
				(*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
				if (tmp) {
					tmp->next = pom->next;
				} else {
					background = pom->next;
				}
			} else {
			// TODO: handle delay
			}
		}
		if ((!tmp) || (tmp->next==pom)) {
			tmp = pom;
			pom = pom->next;
		} else {
			free(pom);
			tmp2 = tmp;
			if (!tmp) pom=background->next;
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
	tmp = NULL;
	pom=background;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) {
				(*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
				if (tmp) {
					tmp->next = pom->next;
				} else {
					background = pom->next;
				}
			} else {
			// TODO: handle delay
			}
		}
		if ((!tmp) || (tmp->next==pom)) {
			tmp = pom;
			pom = pom->next;
		} else {
			free(pom);
			tmp2 = tmp;
			if (!tmp) pom=background->next;
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
	
	game = NULL;
}

struct TM_Arguments* TM_AddToArgs(struct TM_Arguments* args, void* arg) {
	// TODO
	return NULL;
}

void TM_DestroyArgs(struct TM_Arguments* args) {
	struct TM_Arguments *pom;
	while (args) {
		pom = args->next;
		free(args);
		args = pom;
	}
}
