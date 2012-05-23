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
#include <math.h>
#include "levels/actions.h"
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
			int x = (tmp->x/100.0)*al_get_display_width(game->display);
			int y = (tmp->y/100.0)*al_get_display_height(game->display);
			int w = 0, h = 0;
			if (tmp->bitmap) {
				w = al_get_bitmap_width(*(tmp->bitmap))/tmp->cols;
				h = al_get_bitmap_height(*(tmp->bitmap))/tmp->rows;
			}
			if (x > -w) {
				/*if (!tmp->hit)*/
					if ((((x>=derpyx+0.38*derpyw+derpyo) && (x<=derpyx+0.94*derpyw+derpyo)) || ((x+w>=derpyx+0.38*derpyw+derpyo) && (x+w<=derpyx+0.94*derpyw+derpyo)) || ((x<=derpyx+0.38*derpyw+derpyo) && (x+w>=derpyx+0.94*derpyw+derpyo))) &&
						(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)) || ((y<=derpyy+0.26*derpyh) && (y+h>=derpyy+0.76*derpyh)))) {
							tmp->hit=true;
					}

				if (tmp->bitmap) {
					ALLEGRO_BITMAP* subbitmap = al_create_sub_bitmap(*(tmp->bitmap),w*(tmp->pos%tmp->cols), h*(tmp->pos/tmp->cols),w,h);
					al_draw_rotated_bitmap(subbitmap,w/2.0, h/2.0, x+w/2.0,y+h/2.0, tmp->angle, 0);
					al_destroy_bitmap(subbitmap);
				}

				if (tmp->anim_speed) {
					tmp->tmp_pos+=tps(game, 60);
					if (tmp->tmp_pos >= tmp->anim_speed) {
						tmp->pos++;
						tmp->tmp_pos = 0;
					}
					if (tmp->pos>=tmp->cols*tmp->rows-tmp->blanks) tmp->pos=0;
				}

				/*al_draw_bitmap(*(tmp->bitmap), x, y, 0);*/
				if (game->level.debug_show_sprite_frames) al_draw_rectangle(x, y, x+w, y+h, al_map_rgba(255,0,0,255), 3);

				if (tmp->hit) {
					if (tmp->points<0) colision = true;
					else tmp->bitmap = NULL;
					game->level.hp+=tps(game, 60*0.0002*tmp->points*(((1-game->level.speed_modifier)/2.0)+1));
					if (game->level.hp>1) game->level.hp=1;
					PrintConsole(game, "POINTS: %d, %f", tmp->points, tps(game, 60*0.0002*tmp->points*game->level.speed_modifier));
					if ((game->level.hp<=0) && (!game->level.failed)) {
						game->level.failed = true;
						game->level.handle_input = false;
						game->level.speed_modifier = 1;
						TM_AddBackgroundAction(&LevelFailed, NULL, 0, "levelfailed");
					}
				}
				tmp->x -= tps(game, game->level.speed*game->level.speed_modifier*60*tmp->speed)*100*al_get_bitmap_width(game->level.stage)/(float)al_get_display_width(game->display);
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
		/*if (colision) game->level.hp-=tps(game, 60*0.002);*/

		al_set_target_bitmap(game->level.derpy);
		al_clear_to_color(al_map_rgba(0,0,0,0));
		al_draw_bitmap_region(*(game->level.derpy_sheet),al_get_bitmap_width(game->level.derpy)*(game->level.sheet_pos%game->level.sheet_cols),al_get_bitmap_height(game->level.derpy)*(game->level.sheet_pos/game->level.sheet_cols),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
		if ((game->level.sheet_speed) && (game->level.sheet_speed_modifier)) {
			game->level.sheet_tmp+=tps(game, 60);
			if (game->level.sheet_tmp >= (game->level.sheet_speed/game->level.speed_modifier)/game->level.sheet_speed_modifier) {
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
			game->level.cl_pos += tps(game, 60*game->level.speed*game->level.speed_modifier) * 0.2;
			game->level.bg_pos += tps(game, 60*game->level.speed*game->level.speed_modifier) * 0.6;
			game->level.st_pos += tps(game, 60*game->level.speed*game->level.speed_modifier) * 1;
			game->level.fg_pos += tps(game, 60*game->level.speed*game->level.speed_modifier) * 1.75;
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

void Level_Load(struct Game *game) {
	game->level.failed=false;
	game->level.hp=1;
	game->level.cl_pos=0;
	game->level.bg_pos=0;
	game->level.fg_pos=0.2;
	game->level.st_pos=0.1;
	game->level.speed = 0;
	game->level.speed_modifier = 1;
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
		TM_AddDelay(500);
		/* first part gameplay goes here */

		/* actions for generating obstacles should go here
		* probably as regular actions. When one ends, harder one
		* begins. After last one part with muffins starts. */
		TM_AddAction(&GenerateObstacles, NULL, "obstacles");
		TM_AddDelay(3*1000);
		/* wings disappear, deccelerate */
		TM_AddAction(&Run, NULL, "run");
		TM_AddDelay(3*1000);
		/* show Fluttershy's house

		// second part gameplay goes here

		// cutscene goes here */
		TM_AddAction(&PassLevel, NULL, "passlevel");

		struct Obstacle *obst = malloc(sizeof(struct Obstacle));
		obst->prev = NULL;
		obst->next = NULL;
		obst->x = 83.5;
		obst->y = 55;
		obst->speed = 1;
		obst->points = 0;
		obst->hit = false;
		obst->rows = 1;
		obst->cols = 1;
		obst->pos = 0;
		obst->blanks = 0;
		obst->anim_speed = 0;
		obst->tmp_pos = 0;
		obst->callback = NULL;
		obst->data = NULL;
		obst->bitmap = &(game->level.owl);
		game->level.obstacles = obst;
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
	else if (game->level.handle_input) {
		if (ev->keyboard.keycode==ALLEGRO_KEY_LEFT) {
			game->level.speed_modifier = 0.75;
		} else if (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) {
			game->level.speed_modifier = 1.3;
		}
	}
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	}
	return 0;
}

void Level_ProcessLogic(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.handle_input) {
		if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) {
			game->level.speed_modifier = 1;
			struct ALLEGRO_KEYBOARD_STATE keyboard;
			al_get_keyboard_state(&keyboard);
			if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) {
				game->level.speed_modifier = 1.3;
			}
		} else if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) {
			game->level.speed_modifier = 1;
			struct ALLEGRO_KEYBOARD_STATE keyboard;
			al_get_keyboard_state(&keyboard);
			if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT)) {
				game->level.speed_modifier = 0.75;
			}
		}
	}
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
		al_destroy_bitmap(game->level.letter);
		al_destroy_bitmap(game->level.owl);
		al_destroy_bitmap(game->level.obst_bmps.pie1);
		al_destroy_bitmap(game->level.obst_bmps.pie2);
		al_destroy_bitmap(game->level.obst_bmps.pig);
		al_destroy_bitmap(game->level.obst_bmps.cherry);
		al_destroy_bitmap(game->level.obst_bmps.muffin);
		al_destroy_bitmap(game->level.obst_bmps.badmuffin);
		al_destroy_bitmap(game->level.obst_bmps.screwball);
		al_destroy_bitmap(game->level.meter_bmp);
		al_destroy_bitmap(game->level.meter_image);
	}
}

void Level_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(18);
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
		game->level.obst_bmps.pie1 = LoadScaledBitmap("levels/pie1.png", al_get_display_width(game->display)*0.1, al_get_display_height(game->display)*0.08);
		PROGRESS;
		game->level.obst_bmps.pie2 = LoadScaledBitmap("levels/pie2.png", al_get_display_width(game->display)*0.1, al_get_display_height(game->display)*0.08);
		PROGRESS;
		game->level.obst_bmps.pig = LoadScaledBitmap("levels/pig.png", (int)(al_get_display_width(game->display)*0.15)*3, (int)(al_get_display_height(game->display)*0.2)*3);
		PROGRESS;
		game->level.obst_bmps.screwball = LoadScaledBitmap("levels/screwball.png", (int)(al_get_display_height(game->display)*0.2)*4*1.4, (int)(al_get_display_height(game->display)*0.2)*4);
		PROGRESS;
		game->level.obst_bmps.muffin = LoadScaledBitmap("levels/muffin.png", al_get_display_width(game->display)*0.07, al_get_display_height(game->display)*0.1);
		PROGRESS;
		game->level.obst_bmps.cherry = LoadScaledBitmap("levels/cherry.png", al_get_display_width(game->display)*0.03, al_get_display_height(game->display)*0.08);
		PROGRESS;
		game->level.obst_bmps.badmuffin = LoadScaledBitmap("levels/badmuffin.png", al_get_display_width(game->display)*0.07, al_get_display_height(game->display)*0.1);
		PROGRESS;
		game->level.owl = LoadScaledBitmap("levels/owl.png", al_get_display_width(game->display)*0.08, al_get_display_width(game->display)*0.08);
		PROGRESS;
		game->level.letter = LoadScaledBitmap("levels/letter.png", al_get_display_height(game->display), al_get_display_height(game->display));
		al_set_target_bitmap(game->level.letter);
		al_draw_text(game->menu.font, al_map_rgb(0,0,0), al_get_bitmap_width(game->level.letter)*0.45, al_get_display_height(game->display)*0.45, ALLEGRO_ALIGN_CENTRE, "Letter from Twilight");
		al_set_target_bitmap(al_get_backbuffer(game->display));
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
