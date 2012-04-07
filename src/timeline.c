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
#include "main.h"
#include "timeline.h"

unsigned int lastid;
struct Game* game = NULL;
struct TM_Action *queue, *background;
bool paused;

void TM_Init(struct Game* g) {
	PrintConsole(g, "Timeline Manager: init");
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
				PrintConsole(game, "Timeline Manager: destroy action (queue) %d", queue->id);
				queue->active=false;
				(*queue->function)(game, queue, TM_ACTIONSTATE_DESTROY);
				struct TM_Action *tmp = queue;
				queue = queue->next;
				free(tmp);
			}
		} else {
			// delay
			if (queue->active) {
				struct TM_Action *tmp = queue;
				queue = queue->next;
				free(tmp);
			} else {
				al_start_timer(queue->timer);
			}
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
					PrintConsole(game, "Timeline Manager: destroy action (background) %d", pom->id);
					(*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
					if (tmp) {
						tmp->next = pom->next;
					} else {
						background = pom->next;
					}
				}
			} else {
				// delay
				if (tmp) {
					tmp->next = pom->next;
				} else {
					background = pom->next;
				}
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
	if (ev->type != ALLEGRO_EVENT_TIMER) return;
	if (!game) return;
	if (paused) return;
	if (queue) {
		if (ev->timer.source == queue->timer) {
			PrintConsole(game, "Timeline Manager: event matched (queue) %d", queue->id);
			queue->active=true;
			al_destroy_timer(queue->timer);
			queue->timer = NULL;
			if (queue->function) { 
				PrintConsole(game, "Timeline Manager: init action (queue) %d", queue->id);
				(*queue->function)(game, queue, TM_ACTIONSTATE_INIT);
			} else {
				PrintConsole(game, "Timeline Manager: delay reached (queue) %d", queue->id);
			}
			return;
		}
	}
	struct TM_Action *pom = background;
	while (pom) {
		if (ev->timer.source == pom->timer) {
			PrintConsole(game, "Timeline Manager: event matched (background) %d", pom->id);
			pom->active=true;
			al_destroy_timer(pom->timer);
			pom->timer = NULL;
			PrintConsole(game, "Timeline Manager: delay reached, init action (background) %d", pom->id);
			(*pom->function)(game, pom, TM_ACTIONSTATE_INIT);
			return;
		}
		pom=pom->next;
	}
}

struct TM_Action* TM_AddAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args) {
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
	action->id = ++lastid;
	if (action->function) { 
		PrintConsole(game, "Timeline Manager: init action (queue) %d", action->id);
		(*action->function)(game, action, TM_ACTIONSTATE_INIT);
	}
	return action;
}

void TM_AddBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay) {
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
	action->delay = delay;
	action->id = ++lastid;
	if (delay) {
		action->active = false;
		action->timer = al_create_timer(delay/1000.0);
		al_register_event_source(game->event_queue, al_get_timer_event_source(action->timer));
		al_start_timer(action->timer);
	} else {
		action->timer = NULL;
		action->active = true;
		PrintConsole(game, "Timeline Manager: init action (background) %d", action->id);
		(*action->function)(game, action, TM_ACTIONSTATE_INIT);
	}
}

void TM_AddDelay(float delay) {
	/*int *tmp;
	tmp = malloc(sizeof(int));
	*tmp = delay;
	TM_AddAction(NULL, TM_AddToArgs(NULL, tmp));*/
	struct TM_Action* tmp = TM_AddAction(NULL, NULL);
	PrintConsole(game, "Timeline Manager: adding delay %f ms %d", delay, tmp->id);
	tmp->delay = delay;
	tmp->timer = al_create_timer(delay/1000.0);
	al_register_event_source(game->event_queue, al_get_timer_event_source(tmp->timer));
}

void TM_Pause(bool pause) {
	paused = pause;
	PrintConsole(game, "Timeline Manager: pause %d", pause);
	if (queue) { 
		if (queue->timer) { 
			if (pause) { 
				al_stop_timer(queue->timer);
			} else if (!queue->active) al_start_timer(queue->timer);
		}
	}
	struct TM_Action* tmp = background;
	while (tmp) {
		if (tmp->timer) { 
			if (pause) { 
				al_stop_timer(tmp->timer); 
			} else if (!tmp->active) al_start_timer(tmp->timer);
		}
		tmp = tmp->next;
	}
}

void TM_Destroy() {
	if (!game) return;
	PrintConsole(game, "Timeline Manager: destroy");
	struct TM_Action *tmp, *tmp2, *pom = queue;
	tmp = NULL;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) (*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
			else {
				if (pom->timer) al_destroy_timer(pom->timer);
			}
		} else {
			TM_DestroyArgs(pom->arguments);
			pom->arguments = NULL;
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
			if (*pom->function) (*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
			else {
				if (pom->timer) al_destroy_timer(pom->timer);
			}
		} else {
			TM_DestroyArgs(pom->arguments);
			pom->arguments = NULL;
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
	struct TM_Arguments* tmp;
	if (!args) {
		tmp = malloc(sizeof(struct TM_Arguments));
		tmp->value = arg;
		tmp->next = NULL;
		return tmp;
	}
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = malloc(sizeof(struct TM_Arguments));
	tmp->next->value = arg;
	tmp->next->next = NULL;
	return args;
}

void TM_DestroyArgs(struct TM_Arguments* args) {
	struct TM_Arguments *pom;
	while (args) {
		pom = args->next;
		free(args);
		args = pom;
	}
}
