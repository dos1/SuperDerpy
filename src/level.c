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

/* TODO: check if spritesheet speed still depends on FPS */

void SelectDerpySpritesheet(struct Game *game, char* name) {
	struct Spritesheet *tmp = game->level.derpy_sheets;
	PrintConsole(game, "Selecting Derpy spritesheet: %s", name);
	if (!tmp) {
		PrintConsole(game, "ERROR: No spritesheets registered for Derpy!");
		return;
	}
	while (tmp) {
		if (!strcmp(tmp->name, name)) {
			game->level.derpy_sheet = &(tmp->bitmap);
			game->level.sheet_rows = tmp->rows;
			game->level.sheet_cols = tmp->cols;
			game->level.sheet_blanks = tmp->blanks;
			game->level.sheet_speed_modifier = tmp->speed;
			game->level.sheet_pos = 0;
			game->level.sheet_scale = tmp->scale;
			if (game->level.derpy) al_destroy_bitmap(game->level.derpy);
			game->level.derpy = al_create_bitmap((al_get_display_height(game->display)*0.25)*tmp->aspect*tmp->scale, (al_get_display_height(game->display)*0.25)*tmp->scale);
			PrintConsole(game, "SUCCESS: Derpy spritesheet activated: %s (%dx%d)", name, al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy));
			return;
		}
		tmp = tmp->next;
	}
	PrintConsole(game, "ERROR: No spritesheets registered for Derpy with given name: %s", name);
	return;
}

void RegisterDerpySpritesheet(struct Game *game, char* name) {
	char filename[255] = { };
	sprintf(filename, "data/levels/derpy/%s.ini", name);
	ALLEGRO_CONFIG *config = al_load_config_file(filename);
	struct Spritesheet *s = malloc(sizeof(struct Spritesheet));
	s->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(s->name, name);
	s->bitmap = NULL;
	s->cols = atoi(al_get_config_value(config, "", "cols"));
	s->rows = atoi(al_get_config_value(config, "", "rows"));
	s->blanks = atoi(al_get_config_value(config, "", "blanks"));
	s->speed = atof(al_get_config_value(config, "", "speed"));
	s->aspect = atof(al_get_config_value(config, "", "aspect"));
	s->scale = atof(al_get_config_value(config, "", "scale"));
	s->next = game->level.derpy_sheets;
	game->level.derpy_sheets = s;
	al_destroy_config(config);
	PrintConsole(game, "Registering Derpy spritesheet: %s", name);
}

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

bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool LevelFailed(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) {
		TM_AddBackgroundAction(&FadeOut, NULL, 3000, "fadeout");
	} else if (state == TM_ACTIONSTATE_RUNNING) {
		al_draw_filled_rectangle(0, 0, al_get_display_width(game->display), al_get_display_height(game->display), al_map_rgba(0,0,0,100));
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.4, ALLEGRO_ALIGN_CENTRE, "Failed!");
		game->level.speed-=0.00001;
		return false;
	}
	return true;
}

bool Accelerate(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed+=0.000015;
	if (game->level.speed<0.0025) return false;
	return true;
}

bool Walk(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) action->arguments = NULL;
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	if (!(action->arguments)) SelectDerpySpritesheet(game, "walk");
	action->arguments++;
	game->level.derpy_x+=tps(game, 60*0.001);
	if (game->level.derpy_x<0.05) return false;
	return true;
}

bool Move(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed=0.00035;
	if (game->level.st_pos<0.275) return false;
	return true;
}

bool ShowMeter(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.meter_alpha+=tps(game, 60*4);
	if (game->level.meter_alpha>=255) {
		game->level.meter_alpha=255;
		return true;
	}
	return false;
}

bool Fly(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) action->arguments = NULL;
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	if (!(action->arguments)) {
		SelectDerpySpritesheet(game, "fly");
		/*game->level.gg = true;*/
		TM_AddBackgroundAction(&ShowMeter, NULL, 0, "showmeter");
	}
	action->arguments++;
	game->level.derpy_y-=tps(game, 60*0.004);
	if (game->level.derpy_y>0.2) return false;
	game->level.handle_input=true;
	return true;
}

void Obst_MoveUpDown(struct Game *game, struct Obstacle *obstacle) {
	if (obstacle->data) {
		obstacle->y -= 0.5;
		if (obstacle->y<=0) {
			obstacle->data=NULL;
		}
	} else {
		obstacle->y += 0.5;
		if (obstacle->y>=100) {
			obstacle->data++;
		}
	}
}

bool GenerateObstacles(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	/*float* tmp; bool* in;*/
	int* count;
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(int)));
		/* action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(bool))); */
	}
	count = (int*)action->arguments->value;
	/*tmp = (float*)action->arguments->value;
	in = (bool*)action->arguments->next->value;*/
	if (state == TM_ACTIONSTATE_INIT) {
		*count = 0;
		/* *tmp = 0;
		*in = true;*/
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		if (rand()%(10000/(int)tps(game, 60*100))<=1) {
			PrintConsole(game, "OBSTACLE %d", *count);
			(*count)++;
			struct Obstacle *obst = malloc(sizeof(struct Obstacle));
			obst->prev = NULL;
			obst->x = 100;
			obst->y = (rand()%91)-1;
			obst->speed = 0;
			obst->points = -10;
			obst->hit = false;
			if (rand()%100<=50) {
				obst->callback=Obst_MoveUpDown;
				obst->bitmap = &(game->level.obst_bmps.pie);
			} else {
				obst->callback = NULL;
				obst->bitmap = &(game->level.obst_bmps.muffin);
			}
			obst->data = (void*)(rand()%2);
			if (game->level.obstacles) {
				game->level.obstacles->prev = obst;
				obst->next = game->level.obstacles;
			} else {
				obst->next = NULL;
			}
			game->level.obstacles = obst;
			if (*count > 64) return true;
		}
	} else {
		free(action->arguments->value);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool Stop(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.speed=0;
	SelectDerpySpritesheet(game, "stand");
	return true;
}

bool Letter(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.45, ALLEGRO_ALIGN_CENTRE, "Letter from Twilight");
	struct ALLEGRO_KEYBOARD_STATE keyboard;
	al_get_keyboard_state(&keyboard);
	if (al_key_down(&keyboard, ALLEGRO_KEY_ENTER)) {
		return true;
	}
	return false;
}

void Level_Draw(struct Game *game) {
	if (!al_get_sample_instance_playing(game->level.music) && (game->loadstate==GAMESTATE_LEVEL)) {
		al_set_sample_instance_playing(game->level.music, true);
		al_set_sample_instance_position(game->level.music, game->level.music_pos);
	}
	if (game->level.current_level!=1) Moonwalk_Draw(game);
	else {

		struct ALLEGRO_KEYBOARD_STATE keyboard;
		al_get_keyboard_state(&keyboard);
		if (game->level.handle_input) {
			if (al_key_down(&keyboard, ALLEGRO_KEY_UP)) {
				game->level.derpy_y -= tps(game, 60*0.0075);
				/*PrintConsole(game, "Derpy Y position: %f", game->level.derpy_y);*/
			}
			if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN)) {
				game->level.derpy_y += tps(game, 60*0.0075);
				/*PrintConsole(game, "Derpy Y position: %f", game->level.derpy_y);*/
			}
			/*if ((game->level.derpy_y > 0.6) && (game->level.flying)) {
				SelectDerpySpritesheet(game, "run");
				game->level.flying = false;
				game->level.sheet_speed = tps(game, 60*0.0020/game->level.speed);
			}
			else if ((game->level.derpy_y <= 0.6) && (!game->level.flying)) {
				SelectDerpySpritesheet(game, "fly");
				game->level.flying = true;
				game->level.sheet_speed = tps(game, 60*2.4);
			}
			if (!game->level.flying) game->level.sheet_speed = tps(game, 60*0.0020/game->level.speed); */
			if (game->level.derpy_y < 0) game->level.derpy_y=0;
			else if (game->level.derpy_y > 0.8) game->level.derpy_y=0.8;
		}

		al_draw_bitmap(game->level.clouds, (-game->level.cl_pos)*al_get_bitmap_width(game->level.clouds), 0, 0);
		al_draw_bitmap(game->level.clouds, (1+(-game->level.cl_pos))*al_get_bitmap_width(game->level.clouds), 0, 0);
		al_draw_bitmap(game->level.background, (-game->level.bg_pos)*al_get_bitmap_width(game->level.background), 0, 0);
		al_draw_bitmap(game->level.background, (1+(-game->level.bg_pos))*al_get_bitmap_width(game->level.background), 0, 0);
		al_draw_bitmap(game->level.stage, (-game->level.st_pos)*al_get_bitmap_width(game->level.stage), 0 ,0);
		al_draw_bitmap(game->level.stage, (1+(-game->level.st_pos))*al_get_bitmap_width(game->level.stage), 0 ,0);

		int derpyx = game->level.derpy_x*al_get_display_width(game->display);
		int derpyy = game->level.derpy_y*al_get_display_height(game->display);
		int derpyw = al_get_bitmap_width(game->level.derpy);
		int derpyh = al_get_bitmap_height(game->level.derpy);
		int derpyo = al_get_display_width(game->display)*0.1953125-al_get_bitmap_width(game->level.derpy); /* offset */
		bool colision = false;
		struct Obstacle *tmp = game->level.obstacles;
		while (tmp) {
			/*PrintConsole(game, "DRAWING %f %f", tmp->x, tmp->y);*/
			if (tmp->x > -10) {
				int x = (tmp->x/100.0)*al_get_display_width(game->display);
				int y = (tmp->y/100.0)*al_get_display_height(game->display);
				int w = al_get_bitmap_width(*(tmp->bitmap));
				int h = al_get_bitmap_height(*(tmp->bitmap));
				if (!tmp->hit)
					if ((((x>=derpyx+0.38*derpyw+derpyo) && (x<=derpyx+0.94*derpyw+derpyo)) || ((x+w>=derpyx+0.38*derpyw+derpyo) && (x+w<=derpyx+0.94*derpyw+derpyo))) &&
						(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)))) {
							tmp->hit=true;
							game->level.hp+=tps(game, 60*0.0002*tmp->points);
							if ((game->level.hp<=0) && (!game->level.failed)) {
								game->level.failed = true;
								TM_AddBackgroundAction(&LevelFailed, NULL, 0, "levelfailed");
							}
					}
				al_draw_bitmap(*(tmp->bitmap), x, y, 0);
				if (game->level.debug_show_sprite_frames) al_draw_rectangle(x, y, x+w, y+h, al_map_rgba(255,0,0,255), 3);

				if (tmp->hit) colision = true;
				tmp->x -= tps(game, game->level.speed*60)*310;
				if (tmp->callback) tmp->callback(game, tmp);
				tmp = tmp->next;
			} else {
				if (tmp->next)
					tmp->next->prev = tmp->prev;
				if (tmp->prev)
					tmp->prev->next = tmp->next;
				else
					game->level.obstacles = tmp->next;
				struct Obstacle *t = tmp;
				tmp = tmp->next;
				free(t);
			}
		}
		if (colision) game->level.hp-=tps(game, 60*0.002);

		al_set_target_bitmap(game->level.derpy);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_bitmap_region(*(game->level.derpy_sheet),al_get_bitmap_width(game->level.derpy)*(game->level.sheet_pos%game->level.sheet_cols),al_get_bitmap_height(game->level.derpy)*(game->level.sheet_pos/game->level.sheet_cols),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
		if ((game->level.sheet_speed) && (game->level.sheet_speed_modifier)) {
			game->level.sheet_tmp+=tps(game, 60);
			if (game->level.sheet_tmp >= game->level.sheet_speed/game->level.sheet_speed_modifier) {
				game->level.sheet_pos++;
				game->level.sheet_tmp = 0;
			}
			if (game->level.sheet_pos>=game->level.sheet_cols*game->level.sheet_rows-game->level.sheet_blanks) game->level.sheet_pos=0;
		}
		al_set_target_bitmap(al_get_backbuffer(game->display));
		
		al_draw_tinted_bitmap(game->level.derpy, al_map_rgba(255,255-colision*255,255-colision*255,255), derpyx+al_get_display_width(game->display)*0.1953125-al_get_bitmap_width(game->level.derpy), derpyy, 0);

/*		if ((((x>=derpyx+0.36*derpyw) && (x<=derpyx+0.94*derpyw)) || ((x+w>=derpyx+0.36*derpyw) && (x+w<=derpyx+0.94*derpyw))) &&
			(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)))) {
	*/
		if (game->level.debug_show_sprite_frames) {
			al_draw_rectangle(derpyx+derpyo, derpyy, derpyx+derpyw+derpyo, derpyy+derpyh, al_map_rgba(0,255,0,255), 3);
			al_draw_rectangle(derpyx+0.38*derpyw+derpyo, derpyy+0.26*derpyh, derpyx+0.94*derpyw+derpyo, derpyy+0.76*derpyh, al_map_rgba(0,0,255,255), 3);
		}

		al_draw_bitmap(game->level.foreground, (-game->level.fg_pos)*al_get_bitmap_width(game->level.foreground), 0 ,0);
		al_draw_bitmap(game->level.foreground, (1+(-game->level.fg_pos))*al_get_bitmap_width(game->level.foreground), 0 ,0);

		if (game->level.speed > 0) {
			game->level.cl_pos += tps(game, 60*game->level.speed) * 0.2;
			game->level.bg_pos += tps(game, 60*game->level.speed) * 0.6;
			game->level.st_pos += tps(game, 60*game->level.speed) * 1;
			game->level.fg_pos += tps(game, 60*game->level.speed) * 1.75;
			if (game->level.bg_pos >= 1) game->level.bg_pos=game->level.bg_pos-1;
			if (game->level.st_pos >= 1) game->level.st_pos=game->level.st_pos-1;
			if (game->level.fg_pos >= 1) game->level.fg_pos=game->level.fg_pos-1;
		}
		game->level.cl_pos += tps(game, 60*0.00005);
		if (game->level.cl_pos >= 1) game->level.cl_pos=game->level.cl_pos-1;

		al_set_target_bitmap(game->level.meter_bmp);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_filled_rounded_rectangle(al_get_bitmap_width(game->level.meter_bmp)*0.1, al_get_bitmap_height(game->level.meter_bmp)*0.34, al_get_bitmap_width(game->level.meter_bmp)*0.993, al_get_bitmap_height(game->level.meter_bmp)*0.66,
																		 6,6, al_map_rgb(232,234,239));
		al_draw_horizontal_gradient_rect(al_get_bitmap_width(game->level.meter_bmp)-al_get_display_width(game->display)*0.215, (al_get_bitmap_height(game->level.meter_bmp)-al_get_display_height(game->display)*0.025)/2, al_get_display_width(game->display)*0.215*0.975, al_get_display_height(game->display)*0.025, al_map_rgb(150,159,182), al_map_rgb(130,139,162));
		al_draw_filled_rectangle(al_get_bitmap_width(game->level.meter_bmp)-al_get_display_width(game->display)*0.215, (al_get_bitmap_height(game->level.meter_bmp)-al_get_display_height(game->display)*0.025)/2, al_get_bitmap_width(game->level.meter_bmp)-al_get_display_width(game->display)*0.215+(al_get_display_width(game->display)*0.215*0.975)*game->level.hp, (al_get_bitmap_height(game->level.meter_bmp)-al_get_display_height(game->display)*0.025)/2+al_get_display_height(game->display)*0.025, al_map_rgb(214,172,55));
		al_draw_bitmap(game->level.meter_image, 0, 0, 0);
		al_set_target_bitmap(al_get_backbuffer(game->display));

		al_draw_tinted_bitmap(game->level.meter_bmp, al_map_rgba(game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha), al_get_display_width(game->display)*0.95-al_get_bitmap_width(game->level.meter_bmp), al_get_display_height(game->display)*0.975-al_get_bitmap_height(game->level.meter_bmp), 0);

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
		al_play_sample_instance(game->level.music);
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
		/*PrintConsole(game, "WELCOME INIT");*/
	}
	else if (state == TM_ACTIONSTATE_RUNNING) {
		/*PrintConsole(game, "WELCOME RUNNING FADE=%f, IN=%d", *in); */
		float fade = *tmp;
		if (fade>255) fade=255;
		if (*tmp > 2048) { *tmp=255; *in=false; }
		al_draw_tinted_bitmap(game->level.welcome, al_map_rgba_f(fade/255.0,fade/255.0,fade/255.0,fade/255.0), 0, 0, 0);
		if (*in) {
			*tmp+=tps(game, 600);
		} else {
			*tmp-=tps(game, 600);
			if (*tmp<=0) { return true; }
		}
	} else {
		free(action->arguments->value);
		free(action->arguments->next->value);
		TM_DestroyArgs(action->arguments);
		action->arguments = NULL;
	}
	return false;
}

bool PassLevel(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_DESTROY) {
		Level_Passed(game);
		TM_AddBackgroundAction(&FadeOut, NULL, 0, "fadeout");
	}
	return true;
}

void Level_Load(struct Game *game) {
	game->level.failed=false;
	game->level.hp=1;
	game->level.cl_pos=0;
	game->level.bg_pos=0;
	game->level.fg_pos=0.2;
	game->level.st_pos=0.1;
	game->level.speed = 0;
	game->level.derpy_x = -0.2;
	game->level.derpy_y = 0.6;
	game->level.sheet_speed = tps(game, 60*2.4);
	game->level.sheet_tmp = 0;
	game->level.handle_input = false;
	game->level.obstacles = NULL;
	game->level.meter_alpha=0;
	game->level.debug_show_sprite_frames=false;
	al_clear_to_color(al_map_rgb(0,0,0));
	if (game->level.current_level!=1) Moonwalk_Load(game);
	else {
		TM_Init(game);
		TM_AddBackgroundAction(&FadeIn, NULL, 0, "fadein");
		TM_AddDelay(1000);
		TM_AddQueuedBackgroundAction(&Welcome, NULL, 0, "welcome");
		TM_AddDelay(1000);
		TM_AddAction(&Walk, NULL, "walk");
		TM_AddAction(&Move, NULL, "move");
		TM_AddAction(&Stop, NULL, "stop");
		TM_AddDelay(1000);
		TM_AddAction(&Letter, NULL, "letter");
		TM_AddDelay(200);
		TM_AddQueuedBackgroundAction(&Accelerate, NULL, 0, "accelerate");
		TM_AddAction(&Fly, NULL, "fly");
		/*TM_AddDelay(2*1000);*/
		/* first part gameplay goes here */

		/* actions for generating obstacles should go here
		* probably as regular actions. When one ends, harder one
		* begins. After last one part with muffins starts.
		* Should obstacles themselves be handled as objects
		* on timeline? (probably not). Hmm... */
		TM_AddAction(&GenerateObstacles, NULL, "obstacles");
		TM_AddDelay(5*1000);

		/*
        // wings disappear, deccelerate, fall down
		// run
		// show Fluttershy's house

		// second part gameplay goes here
		//

		// cutscene goes here
		//
		*/

		TM_AddAction(&PassLevel, NULL, "passlevel");
	}
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->level.music_pos = al_get_sample_instance_position(game->level.music);
		al_set_sample_instance_playing(game->level.music, false);
	} else if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F2)) {
		game->level.hp -= 0.1;
		if (game->level.hp <= 0) game->level.hp=0.001;
	} else if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F3)) {
		game->level.hp += 0.1;
		if (game->level.hp > 1) game->level.hp=1;
	} else if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F4)) {
		game->level.debug_show_sprite_frames = !game->level.debug_show_sprite_frames;
	}
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

void Level_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	game->level.derpy_sheets = NULL;
	game->level.derpy = NULL;
	Pause_Preload(game);
	RegisterDerpySpritesheet(game, "walk");
	RegisterDerpySpritesheet(game, "fly");
	RegisterDerpySpritesheet(game, "run");
	RegisterDerpySpritesheet(game, "stand");
	if (game->level.current_level!=1) Moonwalk_Preload(game);
	else {
		game->level.sample = al_load_sample( "data/levels/1/music.flac" );
		game->level.music = al_create_sample_instance(game->level.sample);
		al_attach_sample_instance_to_mixer(game->level.music, game->audio.music);
		al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

		if (!game->level.sample){
			fprintf(stderr, "Audio clip sample not loaded!\n" );
			exit(-1);
		}
	}
	Level_PreloadBitmaps(game, progress);
}

void Level_Unload(struct Game *game) {
	Pause_Unload_Real(game);
	al_destroy_sample_instance(game->level.music);
	al_destroy_sample(game->level.sample);
	if (game->level.current_level!=1) Moonwalk_Unload(game);
	else {
		TM_Destroy();
	}
	struct Obstacle *t = game->level.obstacles;
	if (t) {
		while (t->next) {
			if (t->prev) free(t->prev);
			t = t->next;
		}
		free(t);
	}
	Level_UnloadBitmaps(game);
	struct Spritesheet *tmp, *s = game->level.derpy_sheets;
	tmp = s;
	while (s) {
		tmp = s;
		s = s->next;
		free(tmp);
	}
}

void Level_UnloadBitmaps(struct Game *game) {
	al_destroy_bitmap(game->level.derpy);
	struct Spritesheet *tmp = game->level.derpy_sheets;
	while (tmp) {
		al_destroy_bitmap(tmp->bitmap);
		tmp = tmp->next;
	}
	if (game->level.current_level!=1) Moonwalk_UnloadBitmaps(game);
	else {
		al_destroy_bitmap(game->level.foreground);
		al_destroy_bitmap(game->level.background);
		al_destroy_bitmap(game->level.stage);
		al_destroy_bitmap(game->level.clouds);
		al_destroy_bitmap(game->level.welcome);
		al_destroy_bitmap(game->level.obst_bmps.pie);
		al_destroy_bitmap(game->level.meter_bmp);
		al_destroy_bitmap(game->level.meter_image);
	}
}

void Level_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(11);
	int x = 0;
	struct Spritesheet *tmp = game->level.derpy_sheets;
	while (tmp) {
		x++;
		tmp = tmp->next;
	}
	if (game->level.current_level==1) load_a+=x;
	else load_a=2+x;

	tmp = game->level.derpy_sheets;
	while (tmp) {
		char filename[255] = { };
		sprintf(filename, "levels/derpy/%s.png", tmp->name);
		tmp->bitmap = LoadScaledBitmap(filename, (int)(al_get_display_height(game->display)*0.25*tmp->aspect*tmp->scale)*tmp->cols, (int)(al_get_display_height(game->display)*0.25*tmp->scale)*tmp->rows);
		PROGRESS;
		tmp = tmp->next;
	}
	PROGRESS;
	if (!game->level.derpy) SelectDerpySpritesheet(game, "stand");

	game->level.derpy = al_create_bitmap(al_get_bitmap_width(*(game->level.derpy_sheet))/game->level.sheet_cols, al_get_bitmap_height(*(game->level.derpy_sheet))/game->level.sheet_rows);
	
	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game);
	else {
		/* TODO: handle strange display aspects */
		game->level.clouds = LoadScaledBitmap("levels/1/clouds.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		PROGRESS;
		game->level.foreground = LoadScaledBitmap("levels/1/foreground.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		PROGRESS;
		game->level.background = LoadScaledBitmap("levels/1/background.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		PROGRESS;
		game->level.stage = LoadScaledBitmap("levels/1/stage.png", al_get_display_height(game->display)*4.73307291666666666667, al_get_display_height(game->display));
		PROGRESS;
		game->level.obst_bmps.pie = LoadScaledBitmap("menu/pie.png", al_get_display_width(game->display)*0.1, al_get_display_height(game->display)*0.1);
		PROGRESS;
		game->level.obst_bmps.muffin = LoadScaledBitmap("levels/muffin.png", al_get_display_width(game->display)*0.07, al_get_display_height(game->display)*0.1);
		PROGRESS;
		game->level.welcome = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display)/2);
		PROGRESS;
		al_set_target_bitmap(game->level.welcome);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Level 1");
		al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Fluttershy");
		PROGRESS;

		game->level.meter_image = LoadScaledBitmap("levels/meter.png", al_get_display_width(game->display)*0.075, al_get_display_width(game->display)*0.075*0.96470588235294117647);
		PROGRESS;
		game->level.meter_bmp = al_create_bitmap(al_get_display_width(game->display)*0.2+al_get_bitmap_width(game->level.meter_image), al_get_bitmap_height(game->level.meter_image));

		al_set_target_bitmap(al_get_backbuffer(game->display));
	}
	PROGRESS;
}
