/*! \file actions.c
 *  \brief Level actions for Timeline Manager.
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
#include "../level.h"

bool LevelFailed(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) {
		TM_AddBackgroundAction(&FadeOut, NULL, 3000, "fadeout");
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_filled_rectangle(0, 0, game->viewportWidth, game->viewportHeight, al_map_rgba(0,0,0,100));
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "Failed!");
	} else if (state == TM_ACTIONSTATE_RUNNING) {
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

bool ShowMeter(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	game->level.meter_alpha+=4;
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
		game->level.derpy_angle = -0.15;
		/*game->level.gg = true;*/
		TM_AddBackgroundAction(&ShowMeter, NULL, 0, "showmeter");
		action->arguments++;
	}
	game->level.derpy_y-=0.004;
	if (game->level.derpy_y>0.2) return false;
	game->level.handle_input=true;
	return true;
}

bool Run(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (state == TM_ACTIONSTATE_INIT) action->arguments = NULL;
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	if (!(action->arguments)) {
		game->level.handle_input=false;
		game->level.speed_modifier=1;
		action->arguments++;
	}
	game->level.derpy_y+=0.0042;
	if (game->level.derpy_angle > 0) { game->level.derpy_angle -= 0.02; if (game->level.derpy_angle < 0) game->level.derpy_angle = 0; }
	if (game->level.derpy_angle < 0) { game->level.derpy_angle += 0.02; if (game->level.derpy_angle > 0) game->level.derpy_angle = 0; }
	if (game->level.derpy_y<0.65) return false;
	game->level.derpy_angle = 0;
	SelectDerpySpritesheet(game, "run");
	return true;
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
		if (rand()%(10000/(int)(85*game->level.speed_modifier))<=3) {
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
				obst->bitmap = &(game->level.obst_bmps.badmuffin);
			} else if (rand()%100<=12) {
				obst->callback= &Obst_RotateSin;
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0;
				obst->points = 8;
				obst->bitmap = &(game->level.obst_bmps.muffin);
			} else if (rand()%100<=12) {
				obst->callback= &Obst_RotateSin;
				obst->data = malloc(sizeof(float));
				*((float*)obst->data) = 0;
				obst->points = 4;
				obst->bitmap = &(game->level.obst_bmps.cherry);
			} else if (rand()%100<=65) {
				obst->callback= &Obst_MoveUp;
				if (rand()%100<=80) obst->bitmap = &(game->level.obst_bmps.pie1);
				else {
					obst->bitmap = &(game->level.obst_bmps.pie2);
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
				obst->bitmap = &(game->level.obst_bmps.pig);
				obst->rows = 3;
				obst->cols = 3;
				obst->speed = 1.2;
				obst->anim_speed = 2;
				obst->points = -20;
			} else {
				obst->callback = &Obst_MoveUpDown;
				obst->bitmap = &(game->level.obst_bmps.screwball);
				obst->data = (void*)(rand()%2);
				obst->rows = 4;
				obst->cols = 4;
				obst->speed = 1.1;
				obst->anim_speed = 2;
				obst->points = -25;
			}
			if (game->level.obstacles) {
				game->level.obstacles->prev = obst;
				obst->next = game->level.obstacles;
			} else {
				obst->next = NULL;
			}
			game->level.obstacles = obst;
			if (*count > 128) return true;
		}
	} else if (state == TM_ACTIONSTATE_DESTROY) {
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
	if (state == TM_ACTIONSTATE_INIT) action->arguments = NULL;
	if (state == TM_ACTIONSTATE_DESTROY) {
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)action->arguments->next->value;
		al_set_audio_stream_playing(*stream, false);
		al_destroy_audio_stream(*stream);
		free(action->arguments->next->value);
		free(action->arguments->value);
		TM_DestroyArgs(action->arguments);
	}
	if (state == TM_ACTIONSTATE_DRAW) {
		float* f = (float*)action->arguments->value;
		al_draw_tinted_bitmap(game->level.letter, al_map_rgba(*f,*f,*f,*f), (game->viewportWidth-al_get_bitmap_width(game->level.letter))/2.0, al_get_bitmap_height(game->level.letter)*-0.05, 0);
		return false;
	}
	if (state != TM_ACTIONSTATE_RUNNING) return false;
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		float* f = (float*)action->arguments->value;
		*f = 0;
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(ALLEGRO_AUDIO_STREAM*)));
		ALLEGRO_AUDIO_STREAM** stream = (ALLEGRO_AUDIO_STREAM**)action->arguments->next->value;
		*stream = al_load_audio_stream(GetDataFilePath("levels/letter.flac"), 4, 1024);
		al_attach_audio_stream_to_mixer(*stream, game->audio.voice);
		al_set_audio_stream_playing(*stream, true);
		al_set_audio_stream_gain(*stream, 2.00);
		action->arguments->next->next = NULL;
	}
	float* f = (float*)action->arguments->value;
	*f+=5;
	if (*f>255) *f=255;
	al_draw_tinted_bitmap(game->level.letter, al_map_rgba(*f,*f,*f,*f), (game->viewportWidth-al_get_bitmap_width(game->level.letter))/2.0, al_get_bitmap_height(game->level.letter)*-0.05, 0);
	struct ALLEGRO_KEYBOARD_STATE keyboard;
	al_get_keyboard_state(&keyboard);
	if (al_key_down(&keyboard, ALLEGRO_KEY_ENTER)) {
		return true;
	}
	return false;
}

bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	if (!action->arguments) {
		action->arguments = TM_AddToArgs(action->arguments, malloc(sizeof(float)));
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(game->viewportWidth, game->viewportHeight));
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
		*fadeloop-=10;
		if (*fadeloop<=0) return true;
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
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
		action->arguments = TM_AddToArgs(action->arguments, (void*)al_create_bitmap(game->viewportWidth, game->viewportHeight));
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
		*fadeloop+=10;
		if (*fadeloop>=256) return true;
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(fade_bitmap,al_map_rgba_f(1,1,1,*fadeloop/255.0),0,0,0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
		PrintConsole(game, "Leaving level with %d HP", (int)(game->level.hp*100));
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
		if (*in) {
			*tmp+=10;
		} else {
			*tmp-=10;
			if (*tmp<=0) { return true; }
		}
	} else if (state == TM_ACTIONSTATE_DRAW) {
		al_draw_tinted_bitmap(game->level.welcome, al_map_rgba_f(*tmp/255.0,*tmp/255.0,*tmp/255.0,*tmp/255.0), 0, 0, 0);
	} else if (state == TM_ACTIONSTATE_DESTROY) {
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
		Level_Unload(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
		//TM_AddBackgroundAction(&FadeOut, NULL, 0, "fadeout");
	}
	return true;
}
