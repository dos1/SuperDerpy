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
#include "utils.h"
#include "timeline.h"

struct Timeline* TM_Init(struct Game* g, char* name) {
	PrintConsole(g, "Timeline Manager[%s]: init", name);
	struct Timeline* timeline = malloc(sizeof(struct Timeline));
	timeline->game = g;
	timeline->lastid = 0;
	timeline->queue = NULL;
	timeline->background = NULL;
	timeline->name = strdup(name);
	return timeline;
}

void TM_Process(struct Timeline* timeline) {
	/* process first element from queue
		 if returns true, delete it */
	if (timeline->queue) {
		if (*timeline->queue->function) {
			if (!timeline->queue->active) {
				PrintConsole(timeline->game, "Timeline Manager[%s]: queue: run action (%d - %s)", timeline->name, timeline->queue->id, timeline->queue->name);
				(*timeline->queue->function)(timeline->game, timeline->queue, TM_ACTIONSTATE_START);
			}
			timeline->queue->active = true;
			if ((*timeline->queue->function)(timeline->game, timeline->queue, TM_ACTIONSTATE_RUNNING)) {
				PrintConsole(timeline->game, "Timeline Manager[%s]: queue: destroy action (%d - %s)", timeline->name, timeline->queue->id, timeline->queue->name);
				timeline->queue->active=false;
				struct TM_Action *tmp = timeline->queue;
				timeline->queue = timeline->queue->next;
				(*tmp->function)(timeline->game, tmp, TM_ACTIONSTATE_DESTROY);
				free(tmp->name);
				free(tmp);
			}
		} else {
			/* delay handling */
			if (timeline->queue->active) {
				struct TM_Action *tmp = timeline->queue;
				timeline->queue = timeline->queue->next;
				free(tmp->name);
				free(tmp);
			} else {
				if (!al_get_timer_started(timeline->queue->timer)) {
					PrintConsole(timeline->game, "Timeline Manager[%s]: queue: delay started %d ms (%d - %s)", timeline->name, timeline->queue->delay, timeline->queue->id, timeline->queue->name);
					al_start_timer(timeline->queue->timer);
				}
			}
		}
	}
	/* process all elements from background marked as active */
	struct TM_Action *tmp, *tmp2, *pom = timeline->background;
	tmp = NULL;
	while (pom!=NULL) {
		bool destroy = false;
		if (pom->active) {
			if (*pom->function) {
				if ((*pom->function)(timeline->game, pom, TM_ACTIONSTATE_RUNNING)) {
					pom->active=false;
					PrintConsole(timeline->game, "Timeline Manager[%s]: background: destroy action (%d - %s)", timeline->name, pom->id, pom->name);
					(*pom->function)(timeline->game, pom, TM_ACTIONSTATE_DESTROY);
					if (tmp) {
						tmp->next = pom->next;
					} else {
						timeline->background = pom->next;
					}
					destroy = true;
				}
			} else {
				/* delay handling */
				if (tmp) {
					tmp->next = pom->next;
				} else {
					timeline->background = pom->next;
				}
				destroy = true;
			}
		}

		if (!destroy) {
			tmp = pom;
			pom = pom->next;
		} else {
			free(pom->name);
			free(pom);
			tmp2 = tmp;
			if (!tmp) {
				if (timeline->background) {
					pom=timeline->background->next;
				} else {
					pom=NULL;
				}
			}
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
}

void PauseTimers(struct Timeline* timeline, bool pause) {
	if (timeline->queue) {
		if (timeline->queue->timer) {
			if (pause) {
				al_stop_timer(timeline->queue->timer);
			} else if (!timeline->queue->active) al_start_timer(timeline->queue->timer);
		}
	}
	struct TM_Action* tmp = timeline->background;
	while (tmp) {
		if (tmp->timer) {
			if (pause) {
				al_stop_timer(tmp->timer);
			} else if (!tmp->active) al_start_timer(tmp->timer);
		}
		tmp = tmp->next;
	}
}

void Propagate(struct Timeline* timeline, enum TM_ActionState action) {
	if (timeline->queue) {
		if ((*timeline->queue->function) && (timeline->queue->active)) {
			(*timeline->queue->function)(timeline->game, timeline->queue, action);
		}
	}
	/* process all elements from background marked as active */
	struct TM_Action *pom = timeline->background;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) {
				(*pom->function)(timeline->game, pom, action);
			}
		}
		pom = pom->next;
	}
}

void TM_Draw(struct Timeline* timeline) {
	Propagate(timeline, TM_ACTIONSTATE_DRAW);
}

void TM_Pause(struct Timeline* timeline) {
	PrintConsole(timeline->game, "Timeline Manager[%s]: Pause.", timeline->name);
	PauseTimers(timeline, true);
	Propagate(timeline, TM_ACTIONSTATE_PAUSE);
}

void TM_Resume(struct Timeline* timeline) {
	PrintConsole(timeline->game, "Timeline Manager[%s]: Resume.", timeline->name);
	Propagate(timeline, TM_ACTIONSTATE_RESUME);
	PauseTimers(timeline, false);
}

void TM_HandleEvent(struct Timeline* timeline, ALLEGRO_EVENT *ev) {
	if (ev->type != ALLEGRO_EVENT_TIMER) return;
	if (timeline->queue) {
		if (ev->timer.source == timeline->queue->timer) {
			timeline->queue->active=true;
			al_destroy_timer(timeline->queue->timer);
			timeline->queue->timer = NULL;
			if (timeline->queue->function) {
				PrintConsole(timeline->game, "Timeline Manager[%s]: queue: run action (%d - %s)", timeline->name, timeline->queue->id, timeline->queue->name);
				(*timeline->queue->function)(timeline->game, timeline->queue, TM_ACTIONSTATE_START);
			} else {
				PrintConsole(timeline->game, "Timeline Manager[%s]: queue: delay reached (%d - %s)", timeline->name, timeline->queue->id, timeline->queue->name);
			}
			return;
		}
	}
	struct TM_Action *pom = timeline->background;
	while (pom) {
		if (ev->timer.source == pom->timer) {
			PrintConsole(timeline->game, "Timeline Manager[%s]: background: delay reached, run action (%d - %s)", timeline->name, pom->id, pom->name);
			pom->active=true;
			al_destroy_timer(pom->timer);
			pom->timer = NULL;
			(*pom->function)(timeline->game, pom, TM_ACTIONSTATE_START);
			return;
		}
		pom=pom->next;
	}
}

struct TM_Action* TM_AddAction(struct Timeline* timeline, bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, char* name) {
	struct TM_Action *action = malloc(sizeof(struct TM_Action));
	if (timeline->queue) {
		struct TM_Action *pom = timeline->queue;
		while (pom->next!=NULL) {
			pom=pom->next;
		}
		pom->next = action;
	} else {
		timeline->queue = action;
	}
	action->next = NULL;
	action->function = func;
	action->arguments = args;
	action->name = strdup(name);
	action->timer = NULL;
	action->active = false;
	action->delay = 0;
	action->id = ++timeline->lastid;
	if (action->function) {
		PrintConsole(timeline->game, "Timeline Manager[%s]: queue: init action (%d - %s)", timeline->name, action->id, action->name);
		(*action->function)(timeline->game, action, TM_ACTIONSTATE_INIT);
	}
	return action;
}

struct TM_Action* TM_AddBackgroundAction(struct Timeline* timeline, bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, int delay, char* name) {
	struct TM_Action *action = malloc(sizeof(struct TM_Action));
	if (timeline->background) {
		struct TM_Action *pom = timeline->background;
		while (pom->next!=NULL) {
			pom=pom->next;
		}
		pom->next = action;
	} else {
		timeline->background = action;
	}
	action->next = NULL;
	action->function = func;
	action->arguments = args;
	action->name = strdup(name);
	action->delay = delay;
	action->id = ++timeline->lastid;
	if (delay) {
		PrintConsole(timeline->game, "Timeline Manager[%s]: background: init action with delay %d ms (%d - %s)", timeline->name, delay, action->id, action->name);
		(*action->function)(timeline->game, action, TM_ACTIONSTATE_INIT);
		action->active = false;
		action->timer = al_create_timer(delay/1000.0);
		al_register_event_source(timeline->game->_priv.event_queue, al_get_timer_event_source(action->timer));
		al_start_timer(action->timer);
	} else {
		PrintConsole(timeline->game, "Timeline Manager[%s]: background: init action (%d - %s)", timeline->name, action->id, action->name);
		(*action->function)(timeline->game, action, TM_ACTIONSTATE_INIT);
		action->timer = NULL;
		action->active = true;
		PrintConsole(timeline->game, "Timeline Manager[%s]: background: run action (%d - %s)", timeline->name, action->id, action->name);
		(*action->function)(timeline->game, action, TM_ACTIONSTATE_START);
	}
	return action;
}

/*! \brief Predefined action used by TM_AddQueuedBackgroundAction */
bool runinbackground(struct Game* game, struct TM_Action* action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	int* delay = (int*) action->arguments->next->value;
	char* name = (char*) action->arguments->next->next->value;
	struct Timeline *timeline = (struct Timeline*) action->arguments->next->next->next->value;
	TM_AddBackgroundAction(timeline, action->arguments->value, action->arguments->next->next->next->next, *delay, name);
	free(name);
	return true;
}

struct TM_Action* TM_AddQueuedBackgroundAction(struct Timeline* timeline, bool (*func)(struct Game*, struct TM_Action*, enum TM_ActionState), struct TM_Arguments* args, int delay, char* name) {
	TM_WrapArg(int, del, delay);
	struct TM_Arguments* arguments = TM_AddToArgs(NULL, 4, (void*) func, del, strdup(name), (void*) timeline);
	arguments->next->next->next->next = args;
	return TM_AddAction(timeline, *runinbackground, arguments, "TM_BackgroundAction");
}

void TM_AddDelay(struct Timeline* timeline, int delay) {
	/*int *tmp;
	tmp = malloc(sizeof(int));
	*tmp = delay;
	TM_AddAction(NULL, TM_AddToArgs(NULL, tmp));*/
	struct TM_Action* tmp = TM_AddAction(timeline, NULL, NULL, "TM_Delay");
	PrintConsole(timeline->game, "Timeline Manager[%s]: queue: adding delay %d ms (%d)", timeline->name, delay, tmp->id);
	tmp->delay = delay;
	tmp->timer = al_create_timer(delay/1000.0);
	al_register_event_source(timeline->game->_priv.event_queue, al_get_timer_event_source(tmp->timer));
}

void TM_Destroy(struct Timeline* timeline) {
	PrintConsole(timeline->game, "Timeline Manager[%s]: destroy", timeline->name);
	struct TM_Action *tmp, *tmp2, *pom = timeline->queue;
	tmp = NULL;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) (*pom->function)(timeline->game, pom, TM_ACTIONSTATE_DESTROY);
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
			if (!tmp) pom=timeline->background->next;
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
	tmp = NULL;
	pom=timeline->background;
	while (pom!=NULL) {
		if (pom->active) {
			if (*pom->function) (*pom->function)(timeline->game, pom, TM_ACTIONSTATE_DESTROY);
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
			if (!tmp) pom=timeline->background->next;
			else pom=tmp->next;
			tmp = tmp2;
		}
	}
	
	free(timeline->name);
	free(timeline);
}

struct TM_Arguments* TM_AddToArgs(struct TM_Arguments* args, int num, ...) {

	va_list ap;
	int i;
	va_start(ap, num);
	struct TM_Arguments* tmp = args;
	for(i = 0; i < num; i++) {
		if (!tmp) {
			tmp = malloc(sizeof(struct TM_Arguments));
			tmp->value = va_arg(ap, void*);
			tmp->next = NULL;
			args = tmp;
		} else {
			while (tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = malloc(sizeof(struct TM_Arguments));
			tmp->next->value = va_arg(ap, void*);
			tmp->next->next = NULL;
		}
	}
	va_end(ap);
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
