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

bool Accelerate(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed+=0.000005;
	if (game->level.speed<0.0020) return false;
	return true;
}

bool Walk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.derpy_x+=0.001;
	if (game->level.derpy_x<0.05) return false;
	return true;
}

bool Move(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed=0.00035;
	if (game->level.st_pos<0.275) return false;
	return true;
}

bool Fly(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) { action->arguments=NULL; return false; }
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	if (!action->arguments) {
		action->arguments++;
		TM_AddBackgroundAction(&Accelerate, NULL, 0);
	}
	game->level.derpy_y-=0.004;
	if (game->level.derpy_y>0.2) return false;
	game->level.handle_input=true;
	return true;
}


bool Stop(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed=0;
	game->level.sheet_speed = 0;
	//game->level.sheet_pos = 0;
	return true;
}

void Level_Draw(struct Game *game) {
	if (game->level.current_level!=1) Moonwalk_Draw(game);
	else {
		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_bitmap(game->level.clouds, (-game->level.cl_pos)*al_get_bitmap_width(game->level.clouds), 0, 0);
		al_draw_bitmap(game->level.clouds, (1+(-game->level.cl_pos))*al_get_bitmap_width(game->level.clouds), 0, 0);
		al_draw_bitmap(game->level.background, (-game->level.bg_pos)*al_get_bitmap_width(game->level.background), 0, 0);
		al_draw_bitmap(game->level.background, (1+(-game->level.bg_pos))*al_get_bitmap_width(game->level.background), 0, 0);
		al_draw_bitmap(game->level.stage, (-game->level.st_pos)*al_get_bitmap_width(game->level.stage), 0 ,0);
		al_draw_bitmap(game->level.stage, (1+(-game->level.st_pos))*al_get_bitmap_width(game->level.stage), 0 ,0);

		al_set_target_bitmap(game->level.derpy);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_bitmap_region(game->level.derpy_walkcycle,al_get_bitmap_width(game->level.derpy)*(game->level.sheet_pos%game->level.sheet_cols),al_get_bitmap_height(game->level.derpy)*(game->level.sheet_pos/game->level.sheet_cols),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
		if (game->level.sheet_speed) {
			game->level.sheet_tmp++;
			if (game->level.sheet_tmp%game->level.sheet_speed) game->level.sheet_pos++;
			if (game->level.sheet_pos>=game->level.sheet_cols*game->level.sheet_rows) game->level.sheet_pos=0;
		}
		al_set_target_bitmap(al_get_backbuffer(game->display));

		al_draw_bitmap(game->level.derpy, game->level.derpy_x*al_get_display_width(game->display), game->level.derpy_y*al_get_display_height(game->display), 0);
		
		al_draw_bitmap(game->level.foreground, (-game->level.fg_pos)*al_get_bitmap_width(game->level.foreground), 0 ,0);
		al_draw_bitmap(game->level.foreground, (1+(-game->level.fg_pos))*al_get_bitmap_width(game->level.foreground), 0 ,0);
		if (game->level.speed > 0) {
			game->level.cl_pos += game->level.speed * 0.6;
			game->level.bg_pos += game->level.speed * 0.6;
			game->level.st_pos += game->level.speed * 1;
			game->level.fg_pos += game->level.speed * 1.75;
			if (game->level.cl_pos >= 1) game->level.cl_pos=1-game->level.cl_pos;
			if (game->level.bg_pos >= 1) game->level.bg_pos=1-game->level.bg_pos;
			if (game->level.st_pos >= 1) game->level.st_pos=1-game->level.st_pos;
			if (game->level.fg_pos >= 1) game->level.fg_pos=1-game->level.fg_pos;
		}
		game->level.cl_pos += 0.0001;
		TM_Process();
	}
}

bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) { 
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display)));
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
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display)));
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

bool Welcome(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	float* tmp; bool* in;
	if (!action->arguments) { 
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(bool)));
	}
	tmp = (float*)action->arguments->value;
	in = (bool*)action->arguments->next->value;
	if (state == TM_ACTIONSTATE_INIT) {
		*tmp = 0;
		*in = true;
		//PrintConsole(game, "WELCOME INIT");
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		//al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), *tmp, 20, 0, "Level 1");
		//PrintConsole(game, "WELCOME RUNNING FADE=%f, IN=%d", *in);
		int fade = *tmp;
		if (fade>255) fade=255;
		if (*tmp > 1500) { *tmp=255; *in=false; }
		al_draw_tinted_bitmap(game->level.welcome, al_map_rgba_f(fade/255.0,fade/255.0,fade/255.0,fade/255.0), 0, 0, 0);
		if (*in) {
			*tmp+=tps(game, 600);
			//if (*tmp>=256) { return false; }
		} else {
			*tmp-=tps(game, 600);
			if (*tmp<=0) { return true; }
		}
	} else {
		free(action->arguments->value);
		free(action->arguments->next->value);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
		//TM_AddBackgroundAction(&Accelerate, NULL, 1000);
	}
	return false;
}

bool PassLevel(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_DESTROY) {
		Level_Passed(game);
		TM_AddBackgroundAction(&FadeOut, NULL, 0);
	}
	return true;
}

void Level_Load(struct Game *game) {
	game->level.cl_pos=0;
	game->level.bg_pos=0;
	game->level.fg_pos=0.2;
	game->level.st_pos=0.1;
	game->level.speed = 0;
	game->level.derpy_x = -0.2;
	game->level.derpy_y = 0.6;
	game->level.sheet_rows = 4;
	game->level.sheet_cols = 6;
	game->level.sheet_pos = 0;
	game->level.sheet_speed = 2;
	game->level.sheet_tmp = 0;
	game->level.handle_input = false;
	al_clear_to_color(al_map_rgb(0,0,0));
	if (game->level.current_level!=1) Moonwalk_Load(game);
	else {
		TM_Init(game);
		TM_AddBackgroundAction(&FadeIn, NULL, 0);
		TM_AddDelay(1000);
		TM_AddAction(&Welcome, NULL);
		TM_AddAction(&Walk, NULL);
		TM_AddAction(&Move, NULL);
		TM_AddAction(&Stop, NULL);
		TM_AddDelay(5*1000);
		TM_AddAction(&Fly, NULL);
		// Derpy walks in... (background - owl)
		// Derpy reads a letter
		// Letter on screen
		// Derpy: fly! (background - accelerate)

		// first part gameplay goes here
		TM_AddDelay(60*1000);

		// wings disappear, deccelerate, fall down
		// run
		// show Fluttershy's house
		
		// second part gameplay goes here
		//
		
		// cutscene goes here
		//
		
		TM_AddAction(&PassLevel, NULL);
	}
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level!=1) Moonwalk_Keydown(game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	} else {
		if (game->level.handle_input) {
			if (ev->keyboard.keycode == ALLEGRO_KEY_UP) {
				game->level.derpy_y -= 0.05;
			} else if (ev->keyboard.keycode == ALLEGRO_KEY_DOWN) {
				game->level.derpy_y += 0.05;
			}
			if (game->level.derpy_y > 0.5) game->level.sheet_speed=2;
			else game->level.sheet_speed=0;
		}
	}
	return 0;
}

void Level_ProcessLogic(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level==1) TM_HandleEvent(ev);
}

void Level_Preload(struct Game *game) {
	Pause_Preload(game);
	if (game->level.current_level!=1) Moonwalk_Preload(game);
	Level_PreloadBitmaps(game);
}

void Level_Unload(struct Game *game) {
	Pause_Unload_Real(game);
	if (game->level.current_level!=1) Moonwalk_Unload(game);
	else {
		TM_Destroy();
	}
	Level_UnloadBitmaps(game);
}

void Level_UnloadBitmaps(struct Game *game) {
	al_destroy_bitmap(game->level.derpy);
	al_destroy_bitmap(game->level.derpy_walkcycle);
	if (game->level.current_level!=1) Moonwalk_UnloadBitmaps(game);
	else {
		al_destroy_bitmap(game->level.foreground);
		al_destroy_bitmap(game->level.background);
		al_destroy_bitmap(game->level.stage);
		al_destroy_bitmap(game->level.clouds);
		al_destroy_bitmap(game->level.welcome);
	}
}

void Level_PreloadBitmaps(struct Game *game) {
	game->level.derpy_walkcycle = LoadScaledBitmap("derpcycle.png", al_get_display_width(game->display)*0.1953125*6, al_get_display_height(game->display)*0.25*4);
	game->level.derpy = al_create_bitmap(al_get_display_width(game->display)*0.1953125, al_get_display_height(game->display)*0.25);
	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game);
	else {
		// TODO: handle strange display aspects
		game->level.clouds = LoadScaledBitmap("levels/1/clouds.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		game->level.foreground = LoadScaledBitmap("levels/1/foreground.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		game->level.background = LoadScaledBitmap("levels/1/background.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		game->level.stage = LoadScaledBitmap("levels/1/stage.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		game->level.welcome = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
		al_set_target_bitmap(game->level.welcome);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Level 1");
		al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Fluttershy");
		al_set_target_bitmap(al_get_backbuffer(game->display));
	}
}
