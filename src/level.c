/*! \file level.c
 *  \brief Playable Level code.
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
#include <stdio.h>
#include "moonwalk.h"
#include "config.h"
#include "pause.h"
#include "level.h"
#include "timeline.h"

void Level_Passed(struct Game *game) {
	if (game->level.current_level<6) {
		int available = atoi(GetConfigOptionDefault("MuffinAttack", "level", "1"));
		available++;
		if ((available<2) || (available>7)) available=1;
		if (available==(game->level.current_level+1)) {
			char* text = malloc(2*sizeof(char));
			sprintf(text, "%d", available);
			SetConfigOption("MuffinAttack", "level", text);
			free(text);
		}
	} else {
		SetConfigOption("MuffinAttack", "completed", "1");
	}
}

void Level_Draw(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_Draw(game);
	else {
		al_clear_to_color(al_map_rgb(128,0,0));
		TM_Process();
	}
}

bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) { 
		action->arguments = malloc(sizeof(struct TM_Arguments));
		action->arguments->value = malloc(sizeof(float));
		action->arguments->next = malloc(sizeof(struct TM_Arguments));
		action->arguments->next->value = (void*)al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
		action->arguments->next->next = NULL;
	}
	float* fadeloop;
	ALLEGRO_BITMAP* fade_bitmap;
	fadeloop = (float*)action->arguments->value;
	fade_bitmap = (ALLEGRO_BITMAP*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*fadeloop = 255;
		al_set_target_bitmap(fade_bitmap);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_set_target_bitmap(al_get_backbuffer(game->display));
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
		*fadeloop-=tps(game, 600);
		if (*fadeloop<=0) return true;
	} else {
		al_destroy_bitmap(fade_bitmap);
		free(fadeloop);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) { 
		action->arguments = malloc(sizeof(struct TM_Arguments));
		action->arguments->value = malloc(sizeof(float));
		action->arguments->next = malloc(sizeof(struct TM_Arguments));
		action->arguments->next->value = (void*)al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
		action->arguments->next->next = NULL;
	}
	float* fadeloop;
	ALLEGRO_BITMAP* fade_bitmap;
	fadeloop = (float*)action->arguments->value;
	fade_bitmap = (ALLEGRO_BITMAP*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*fadeloop = 0;
		al_set_target_bitmap(fade_bitmap);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_set_target_bitmap(al_get_backbuffer(game->display));
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
		*fadeloop+=tps(game, 600);
		if (*fadeloop>=256) return true;
	} else {
		al_destroy_bitmap(fade_bitmap);
		free(fadeloop);
		Level_Unload(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool napis2(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) { 
		action->arguments = malloc(sizeof(struct TM_Arguments));
		action->arguments->value = malloc(sizeof(int));
		action->arguments->next = NULL;
	}
	int* tmp;
	tmp = (int*)action->arguments->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*tmp = 0;
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), *tmp, 100, 0, "lol");
		*tmp+=5;
		if (*tmp>=al_get_display_width(game->display)) { *tmp=0; return true; }
	} else {
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool napis(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) { 
		action->arguments = malloc(sizeof(struct TM_Arguments));
		action->arguments->value = malloc(sizeof(int));
		action->arguments->next = NULL;
	}
	int* tmp;
	tmp = (int*)action->arguments->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*tmp = 0;
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), *tmp, 20, 0, "wat");
		*tmp+=10;
		if (*tmp>=al_get_display_width(game->display)) { *tmp=0; return true; }
	} else {
		TM_AddBackgroundAction(&napis2, NULL, 0);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool wyjscie(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_DESTROY) {
		Level_Passed(game);
		TM_AddBackgroundAction(&FadeOut, NULL, 0);
	}
	return true;
}

void Level_Load(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	if (game->level.current_level!=1) Moonwalk_Load(game);
	else {
		TM_Init(game);
		TM_AddAction(&napis, NULL);
		TM_AddAction(&napis, NULL);
		TM_AddAction(&napis, NULL);
		TM_AddBackgroundAction(&napis2, NULL, 0);
		TM_AddAction(&wyjscie, NULL);
		TM_AddBackgroundAction(&FadeIn, NULL, 0);
	}
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level!=1) Moonwalk_Keydown(game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	}
	return 0;
}

void Level_ProcessLogic(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level==1) TM_HandleEvent(ev);
}

void Level_Preload(struct Game *game) {
	Pause_Preload(game);
	if (game->level.current_level!=1) Moonwalk_Preload(game);
}

void Level_Unload(struct Game *game) {
	Pause_Unload_Real(game);
	if (game->level.current_level!=1) Moonwalk_Unload(game);
	else {
		TM_Destroy();
	}
}

void Level_UnloadBitmaps(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_UnloadBitmaps(game);
}

void Level_PreloadBitmaps(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game);
}
