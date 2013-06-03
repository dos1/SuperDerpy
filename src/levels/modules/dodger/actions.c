/*! \file actions.c
 *  \brief Dodger Level module actions for Timeline Manager.
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

#include "actions.h"
#include "callbacks.h"
#include "../../actions.h"
#include "../../../gamestates/level.h"

void SelectDerpySpritesheet(void* a, void* b) {

}

// TODO: make it configurable and move to generic actions
bool Accelerate(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	//game->level.speed+=0.000015;
	//if (game->level.speed>=0.0025) return true;
	return false;
}

// TODO: make it configurable and move to generic actions
bool Walk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_START) SelectDerpySpritesheet(game, "walk");
	else if (state != TM_ACTIONSTATE_RUNNING) return false;
	//game->level.derpy_x+=(0.00125*1280)/(float)game->viewportWidth;
	//if (game->level.derpy_x>=(0.05*1280)/(float)game->viewportWidth) return true;
	return false;
}

// TODO: make it configurable and move to generic actions
bool Move(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
//	game->level.speed=0.000345;
//	if (game->level.st_pos>=0.275) return true;
	return false;
}

bool Fly(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_START) {
		SelectDerpySpritesheet(game, "fly");
//		game->level.derpy_angle = -0.15;
		TM_AddBackgroundAction(&ShowMeter, NULL, 0, "showmeter");
	}
	else if (state == TM_ACTIONSTATE_DESTROY) {
//		game->level.handle_input = true;
	}
	else if (state != TM_ACTIONSTATE_RUNNING) return false;
//	game->level.derpy_y-=0.004;
//	if (game->level.derpy_y<=0.2) return true;
	return false;
}

bool Run(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_START) {
//		game->level.handle_input=false;
//		game->level.speed_modifier=1;
	}
	else if (state == TM_ACTIONSTATE_DESTROY) {
//		game->level.derpy_angle = 0;
		SelectDerpySpritesheet(game, "run");
	}
	else if (state != TM_ACTIONSTATE_RUNNING) return false;
//	game->level.derpy_y+=0.0042;
//	if (game->level.derpy_angle > 0) { game->level.derpy_angle -= 0.02; if (game->level.derpy_angle < 0) game->level.derpy_angle = 0; }
//	if (game->level.derpy_angle < 0) { game->level.derpy_angle += 0.02; if (game->level.derpy_angle > 0) game->level.derpy_angle = 0; }
//	if (game->level.derpy_y>=0.65) return true;
	return false;
}

bool GenerateObstacles(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	int* count;
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(int)));
	}
	count = (int*)action->arguments->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*count = 0;
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
/*		if (rand()%(10000/(int)(85*game->level.speed_modifier))<=3) {
			PrintConsole(game, "OBSTACLE %d", *count);
			(*count)++;
			struct Obstacle *obst = malloc(sizeof(struct Obstacle));
			obst->prev = NULL;
			obst->x = 100;
			obst->y = (rand()%91)-1;
			obst->speed = 1;
			obst->points = -10;
			obst->hit = false;
			obst->rows = 1;
			obst->cols = 1;
			obst->pos = 0;
			obst->blanks = 0;
			obst->anim_speed = 0;
			obst->tmp_pos = 0;
			obst->angle = 0;
			if (rand()%100<=50) {
				obst->callback= NULL;
				obst->data = NULL;
				obst->points = -5;
				obst->bitmap = &(game->level.dodger.obst_bmps.badmuffin);
			} else if (rand()%100<=12) {
				obst->callback= &Obst_RotateSin;
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0;
				obst->points = 8;
				obst->bitmap = &(game->level.dodger.obst_bmps.muffin);
			} else if (rand()%100<=12) {
				obst->callback= &Obst_RotateSin;
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0;
				obst->points = 4;
				obst->bitmap = &(game->level.dodger.obst_bmps.cherry);
			} else if (rand()%100<=65) {
				obst->callback= &Obst_MoveUp;
				if (rand()%100<=80) obst->bitmap = &(game->level.dodger.obst_bmps.pie1);
				else {
					obst->bitmap = &(game->level.dodger.obst_bmps.pie2);
					obst->points = -12;
				}
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0.25+(rand()%50/100.0);
				obst->y*=1.8;
				obst->angle = ((rand()%50)/100.0)-0.25;
			} else if (rand()%100<=80) {
				obst->callback = &Obst_MoveSin;
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0;
				obst->bitmap = &(game->level.dodger.obst_bmps.pig);
				obst->rows = 3;
				obst->cols = 3;
				obst->speed = 1.2;
				obst->anim_speed = 2;
				obst->points = -20;
			} else {
				obst->callback = &Obst_MoveUpDown;
				obst->bitmap = &(game->level.dodger.obst_bmps.screwball);
				obst->data = malloc(sizeof(bool));
				*((bool*)obst->data) = rand()%2;
				obst->rows = 4;
				obst->cols = 4;
				obst->speed = 1.1;
				obst->anim_speed = 2;
				obst->points = -25;
			}
			if (game->level.dodger.obstacles) {
				game->level.dodger.obstacles->prev = obst;
				obst->next = game->level.dodger.obstacles;
			} else {
				obst->next = NULL;
			}
			game->level.dodger.obstacles = obst;
			if (*count > 128) return true;
		}*/
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		free(action->arguments->value);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}
