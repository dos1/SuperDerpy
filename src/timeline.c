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
	/*
       process first element from queue
	   if returns true, then delete it
    */
	if (queue) {
		if (*queue->function) {
			queue->active = true;
			if ((*queue->function)(game, queue, TM_ACTIONSTATE_RUNNING)) {
				PrintConsole(game, "Timeline Manager: queue: destroy action (%d - %s)", queue->id, queue->name);
				queue->active=false;
				(*queue->function)(game, queue, TM_ACTIONSTATE_DESTROY);
				struct TM_Action *tmp = queue;
				queue = queue->next;
				free(tmp->name);
				free(tmp);
				if (queue) PrintConsole(game, "Timeline Manager: queue: run action (%d - %s)", queue->id, queue->name);
			}
		} else {
			/* delay handling */
			if (queue->active) {
				struct TM_Action *tmp = queue;
				queue = queue->next;
				free(tmp->name);
				free(tmp);
				if (queue) PrintConsole(game, "Timeline Manager: queue: run action (%d - %s)", queue->id, queue->name);
			} else {
				al_start_timer(queue->timer);
			}
		}
	}
	/* process all elements from background marked as active */
	struct TM_Action *tmp, *tmp2, *pom = background;
	tmp = NULL;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) {
				if ((*pom->function)(game, pom, TM_ACTIONSTATE_RUNNING)) {
					pom->active=false;
					PrintConsole(game, "Timeline Manager: background: destroy action (%d - %s)", pom->id, pom->name);
					(*pom->function)(game, pom, TM_ACTIONSTATE_DESTROY);
					if (tmp) {
						tmp->next = pom->next;
					} else {
						background = pom->next;
					}
				}
			} else {
				/* delay handling */
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
			free(pom->name);
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
			queue->active=true;
			al_destroy_timer(queue->timer);
			queue->timer = NULL;
			if (queue->function) { 
				PrintConsole(game, "Timeline Manager: queue: init action (%d - %s)", queue->id, queue->name);
				(*queue->function)(game, queue, TM_ACTIONSTATE_INIT);
			} else {
				PrintConsole(game, "Timeline Manager: queue: delay reached (%d - %s)", queue->id, queue->name);
			}
			return;
		}
	}
	struct TM_Action *pom = background;
	while (pom) {
		if (ev->timer.source == pom->timer) {
			PrintConsole(game, "Timeline Manager: background: delay reached, init and run action (%d - %s)", pom->id, pom->name);
			pom->active=true;
			al_destroy_timer(pom->timer);
			pom->timer = NULL;
			(*pom->function)(game, pom, TM_ACTIONSTATE_INIT);
			return;
		}
		pom=pom->next;
	}
}

struct TM_Action* TM_AddAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, char* name) {
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
	action->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(action->name, name);
	action->timer = NULL;
	action->active = false;
	action->delay = 0;
	action->id = ++lastid;
	if (action->function) { 
		PrintConsole(game, "Timeline Manager: queue: init action (%d - %s)", action->id, action->name);
		(*action->function)(game, action, TM_ACTIONSTATE_INIT);
	}
	return action;
}

struct TM_Action* TM_AddBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name) {
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
	action->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(action->name, name);
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
		PrintConsole(game, "Timeline Manager: background: init and run action (%d - %s)", action->id, action->name);
		(*action->function)(game, action, TM_ACTIONSTATE_INIT);
	}
	return action;
}

/*! \brief Predefined action used by TM_AddQueuedBackgroundAction */
bool runinbackground(struct Game* game, struct TM_Action* action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	float* delay = (float*) action->arguments->next->value;
	char* name = (char*) action->arguments->next->next->value;
	TM_AddBackgroundAction(action->arguments->value, action->arguments->next->next->next, *delay, name);
	free(name);
	return true;
}

struct TM_Action* TM_AddQueuedBackgroundAction(bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, float delay, char* name) {
	struct TM_Arguments* arguments = TM_AddToArgs(NULL, (void*) func);
	arguments = TM_AddToArgs(arguments, malloc(sizeof(float)));
	arguments = TM_AddToArgs(arguments, NULL);
	*(float*)(arguments->next->value) = delay;
	arguments->next->next->value = malloc((strlen(name)+1)*sizeof(char));
	strcpy(arguments->next->next->value, name);

	arguments->next->next->next = args;
	return TM_AddAction(*runinbackground, arguments, "TM_BackgroundAction");
}

void TM_AddDelay(float delay) {
	/*int *tmp;
	tmp = malloc(sizeof(int));
	*tmp = delay;
	TM_AddAction(NULL, TM_AddToArgs(NULL, tmp));*/
	struct TM_Action* tmp = TM_AddAction(NULL, NULL, "TM_Delay");
	PrintConsole(game, "Timeline Manager: queue: adding delay %f ms (%d)", delay, tmp->id);
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
			free(pom->name);
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
			free(pom->name);
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
	struct TM_Arguments* tmp = args;
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

bool TM_Initialized() {
	if (game) return true;
	return false;
}
