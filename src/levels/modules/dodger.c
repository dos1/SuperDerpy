/*! \file dodger.c
 *  \brief Dodger Level module code.
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
#include "../../gamestates/level.h"
#include "../actions.h"
#include "dodger.h"
#include "dodger/actions.h"

void Dodger_Logic(struct Game *game) {
	struct ALLEGRO_KEYBOARD_STATE keyboard;
	al_get_keyboard_state(&keyboard);
	if (game->level.handle_input) {
		if (game->level.derpy_angle > 0) { game->level.derpy_angle -= 0.02; if (game->level.derpy_angle < 0) game->level.derpy_angle = 0; }
		if (game->level.derpy_angle < 0) { game->level.derpy_angle += 0.02; if (game->level.derpy_angle > 0) game->level.derpy_angle = 0; }
		if (al_key_down(&keyboard, ALLEGRO_KEY_UP)) {
			game->level.derpy_y -= 0.005;
			game->level.derpy_angle -= 0.03;
			if (game->level.derpy_angle < -0.15) game->level.derpy_angle = -0.15;
			/*PrintConsole(game, "Derpy Y position: %f", game->level.derpy_y);*/
		}
		if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN)) {
			game->level.derpy_y += 0.005;
			game->level.derpy_angle += 0.03;
			if (game->level.derpy_angle > 0.15) game->level.derpy_angle = 0.15;
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

		game->level.derpy_y += game->level.derpy_angle / 30;
	}

	int derpyx = game->level.derpy_x*game->viewportHeight*1.6;
	int derpyy = game->level.derpy_y*game->viewportHeight;
	int derpyw = al_get_bitmap_width(game->level.derpy);
	int derpyh = al_get_bitmap_height(game->level.derpy);
	int derpyo = game->viewportHeight*1.6*0.1953125-al_get_bitmap_width(game->level.derpy); /* offset */
	struct Obstacle *tmp = game->level.dodger.obstacles;
	while (tmp) {
		/*PrintConsole(game, "DRAWING %f %f", tmp->x, tmp->y);*/
		int x = (tmp->x/100.0)*game->viewportWidth;
		int y = (tmp->y/100.0)*game->viewportHeight;
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

			if (tmp->anim_speed) {
				tmp->tmp_pos+=1;
				if (tmp->tmp_pos >= tmp->anim_speed) {
					tmp->pos++;
					tmp->tmp_pos = 0;
				}
				if (tmp->pos>=tmp->cols*tmp->rows-tmp->blanks) tmp->pos=0;
			}

			if (tmp->hit) {
				if (tmp->points>0) tmp->bitmap = NULL;
				game->level.hp+=0.0002*tmp->points*(((1-game->level.speed_modifier)/2.0)+1);
				if (game->level.hp>1) game->level.hp=1;
				//PrintConsole(game, "POINTS: %d, %f", tmp->points, tps(game, 60*0.0002*tmp->points*game->level.speed_modifier));
				if ((game->level.hp<=0) && (!game->level.failed)) {
					game->level.failed = true;
					game->level.handle_input = false;
					game->level.speed_modifier = 1;
					TM_AddBackgroundAction(&LevelFailed, NULL, 0, "levelfailed");
				}
			}
			tmp->x -= game->level.speed*game->level.speed_modifier*tmp->speed*100*al_get_bitmap_width(game->level.stage)/(float)game->viewportWidth;
			if (tmp->callback) tmp->callback(game, tmp);
			tmp = tmp->next;
		} else {
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				game->level.dodger.obstacles = tmp->next;
			struct Obstacle *t = tmp;
			tmp = tmp->next;
			free(t);
		}
	}
	/*if (colision) game->level.hp-=tps(game, 60*0.002);*/

}

void Dodger_Draw(struct Game *game) {
	int derpyx = game->level.derpy_x*game->viewportHeight*1.6;
	int derpyy = game->level.derpy_y*game->viewportHeight;
	int derpyw = al_get_bitmap_width(game->level.derpy);
	int derpyh = al_get_bitmap_height(game->level.derpy);
	int derpyo = game->viewportHeight*1.6*0.1953125-al_get_bitmap_width(game->level.derpy); /* offset */
	bool colision = false;
	struct Obstacle *tmp = game->level.dodger.obstacles;
	while (tmp) {
		/*PrintConsole(game, "DRAWING %f %f", tmp->x, tmp->y);*/
		int x = (tmp->x/100.0)*game->viewportWidth;
		int y = (tmp->y/100.0)*game->viewportHeight;
		int w = 0, h = 0;
		if (tmp->bitmap) {
			w = al_get_bitmap_width(*(tmp->bitmap))/tmp->cols;
			h = al_get_bitmap_height(*(tmp->bitmap))/tmp->rows;
		}
		if (x > -w) {
			if ((tmp->hit) && (tmp->points<0)) {
				colision = true;
			}

			if (tmp->bitmap) {
				ALLEGRO_BITMAP* subbitmap = al_create_sub_bitmap(*(tmp->bitmap),w*(tmp->pos%tmp->cols), h*(tmp->pos/tmp->cols),w,h);
				al_draw_rotated_bitmap(subbitmap,w/2.0, h/2.0, x+w/2.0,y+h/2.0, tmp->angle, 0);
				al_destroy_bitmap(subbitmap);
			}

			/*al_draw_bitmap(*(tmp->bitmap), x, y, 0);*/
			if (game->level.debug_show_sprite_frames) al_draw_rectangle(x, y, x+w, y+h, al_map_rgba(255,0,0,255), 3);

			tmp = tmp->next;
		} else {
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				game->level.dodger.obstacles = tmp->next;
			struct Obstacle *t = tmp;
			tmp = tmp->next;
			free(t);
		}
	}
	/*if (colision) game->level.hp-=tps(game, 60*0.002);*/

	al_set_target_bitmap(game->level.derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(*(game->level.derpy_sheet),al_get_bitmap_width(game->level.derpy)*(game->level.sheet_pos%game->level.sheet_cols),al_get_bitmap_height(game->level.derpy)*(game->level.sheet_pos/game->level.sheet_cols),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_rotated_bitmap(game->level.derpy, al_map_rgba(255,255-colision*255,255-colision*255,255), al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy)/2, derpyx+game->viewportHeight*1.6*0.1953125, derpyy + al_get_bitmap_height(game->level.derpy)/2, game->level.derpy_angle, 0);

	/*		if ((((x>=derpyx+0.36*derpyw) && (x<=derpyx+0.94*derpyw)) || ((x+w>=derpyx+0.36*derpyw) && (x+w<=derpyx+0.94*derpyw))) &&
		(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)))) {
*/
	if (game->level.debug_show_sprite_frames) {
		al_draw_rectangle(derpyx+derpyo, derpyy, derpyx+derpyw+derpyo, derpyy+derpyh, al_map_rgba(0,255,0,255), 3);
		al_draw_rectangle(derpyx+0.38*derpyw+derpyo, derpyy+0.26*derpyh, derpyx+0.94*derpyw+derpyo, derpyy+0.76*derpyh, al_map_rgba(0,0,255,255), 3);
	}
}

void Dodger_Load(struct Game *game) {
	game->level.dodger.obstacles = NULL;
}

void Dodger_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.handle_input) {
		if (ev->keyboard.keycode==ALLEGRO_KEY_LEFT) {
			game->level.speed_modifier = 0.75;
		} else if (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) {
			game->level.speed_modifier = 1.3;
		}
	}
}

void Dodger_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
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
}

inline int Dodger_PreloadSteps(void) {
	return 7;
}

void Dodger_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(Dodger_PreloadSteps());
	game->level.dodger.obst_bmps.pie1 = LoadScaledBitmap("levels/dodger/pie1.png", game->viewportHeight*1.6*0.1, game->viewportHeight*0.08);
	PROGRESS;
	game->level.dodger.obst_bmps.pie2 = LoadScaledBitmap("levels/dodger/pie2.png", game->viewportHeight*1.6*0.1, game->viewportHeight*0.08);
	PROGRESS;
	game->level.dodger.obst_bmps.pig = LoadScaledBitmap("levels/dodger/pig.png", (int)(game->viewportHeight*1.6*0.15)*3, (int)(game->viewportHeight*0.2)*3);
	PROGRESS;
	game->level.dodger.obst_bmps.screwball = LoadScaledBitmap("levels/dodger/screwball.png", (int)(game->viewportHeight*0.2)*4*1.4, (int)(game->viewportHeight*0.2)*4);
	PROGRESS;
	game->level.dodger.obst_bmps.muffin = LoadScaledBitmap("levels/dodger/muffin.png", game->viewportHeight*1.6*0.07, game->viewportHeight*0.1);
	PROGRESS;
	game->level.dodger.obst_bmps.cherry = LoadScaledBitmap("levels/dodger/cherry.png", game->viewportHeight*1.6*0.03, game->viewportHeight*0.08);
	PROGRESS;
	game->level.dodger.obst_bmps.badmuffin = LoadScaledBitmap("levels/dodger/badmuffin.png", game->viewportHeight*1.6*0.07, game->viewportHeight*0.1);
	PROGRESS;
}

void Dodger_Preload(struct Game *game) {
	RegisterDerpySpritesheet(game, "walk");
	RegisterDerpySpritesheet(game, "stand");
	RegisterDerpySpritesheet(game, "fly");
	RegisterDerpySpritesheet(game, "run");
}

void Dodger_UnloadBitmaps(struct Game *game) {
	al_destroy_bitmap(game->level.dodger.obst_bmps.pie1);
	al_destroy_bitmap(game->level.dodger.obst_bmps.pie2);
	al_destroy_bitmap(game->level.dodger.obst_bmps.pig);
	al_destroy_bitmap(game->level.dodger.obst_bmps.cherry);
	al_destroy_bitmap(game->level.dodger.obst_bmps.muffin);
	al_destroy_bitmap(game->level.dodger.obst_bmps.badmuffin);
	al_destroy_bitmap(game->level.dodger.obst_bmps.screwball);
}

void Dodger_Unload(struct Game *game) {
	struct Obstacle *t = game->level.dodger.obstacles;
	if (t) {
		while (t->next) {
			if (t->prev) free(t->prev);
			t = t->next;
		}
		free(t);
	}
	struct Spritesheet *tmp, *s = game->level.derpy_sheets;
	tmp = s;
	while (s) {
		tmp = s;
		s = s->next;
		free(tmp);
	}
}

void Dodger_Resume(struct Game *game) {}
void Dodger_Pause(struct Game *game) {}
