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
#include "levels/dodger/actions.h"
#include "levels/moonwalk.h"
#include "levels/dodger.h"
#include "config.h"
#include "pause.h"
#include "level.h"
#include "timeline.h"

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
			game->level.sheet_successor = tmp->successor;
			if (game->level.derpy) al_destroy_bitmap(game->level.derpy);
			game->level.derpy = al_create_bitmap((game->viewportHeight*0.25)*tmp->aspect*tmp->scale, (game->viewportHeight*0.25)*tmp->scale);
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
	sprintf(filename, "levels/derpy/%s.ini", name);
	ALLEGRO_CONFIG *config = al_load_config_file(GetDataFilePath(filename));
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
	s->successor=NULL;
	const char* successor = al_get_config_value(config, "", "successor");
	if (successor) {
		s->successor = malloc(255*sizeof(char));
		strcpy(s->successor, successor);
	}
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

void Level_Logic(struct Game *game) {
	if (game->level.current_level==1) {
		Dodger_Logic(game);
	} else {
		Moonwalk_Logic(game);
	}

	if ((game->level.sheet_speed) && (game->level.sheet_speed_modifier)) {
		game->level.sheet_tmp+=1;
		if (game->level.sheet_tmp >= (game->level.sheet_speed/game->level.speed_modifier)/game->level.sheet_speed_modifier) {
			game->level.sheet_pos++;
			game->level.sheet_tmp = 0;
		}
		if (game->level.sheet_pos>=game->level.sheet_cols*game->level.sheet_rows-game->level.sheet_blanks) {
			game->level.sheet_pos=0;
			if (game->level.sheet_successor) {
				SelectDerpySpritesheet(game, game->level.sheet_successor);
			}
		}
	}

	if (game->level.speed > 0) {
		game->level.cl_pos += game->level.speed*game->level.speed_modifier * 0.2;
		game->level.bg_pos += game->level.speed*game->level.speed_modifier * 0.6;
		game->level.st_pos += game->level.speed*game->level.speed_modifier * 1;
		game->level.fg_pos += game->level.speed*game->level.speed_modifier * 1.75;
		if (game->level.bg_pos >= 1) game->level.bg_pos=game->level.bg_pos-1;
		if (game->level.st_pos >= 1) game->level.st_pos=game->level.st_pos-1;
		if (game->level.fg_pos >= 1) game->level.fg_pos=game->level.fg_pos-1;
	}
	game->level.cl_pos += 0.00005;
	if (game->level.cl_pos >= 1) game->level.cl_pos=game->level.cl_pos-1;

	TM_Process();
}

void Level_Resume(struct Game *game) {
	al_set_sample_instance_position(game->level.music, game->level.music_pos);
	al_set_sample_instance_playing(game->level.music, true);
	if (game->level.current_level==1) {
		TM_Resume();
	}
}

void Level_Pause(struct Game *game) {
	game->level.music_pos = al_get_sample_instance_position(game->level.music);
	al_set_sample_instance_playing(game->level.music, false);
	if (game->level.current_level==1) {
		TM_Pause();
	}
}

void Level_Draw(struct Game *game) {
	al_draw_bitmap(game->level.clouds, (-game->level.cl_pos)*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.clouds, (1+(-game->level.cl_pos))*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.background, (-game->level.bg_pos)*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.background, (1+(-game->level.bg_pos))*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.stage, (-game->level.st_pos)*al_get_bitmap_width(game->level.stage), 0 ,0);
	al_draw_bitmap(game->level.stage, (1+(-game->level.st_pos))*al_get_bitmap_width(game->level.stage), 0 ,0);

	if (game->level.current_level==1) Dodger_Draw(game);
	else Moonwalk_Draw(game);

	if (game->level.unloading) return;

	al_draw_bitmap(game->level.foreground, (-game->level.fg_pos)*al_get_bitmap_width(game->level.foreground), 0 ,0);
	al_draw_bitmap(game->level.foreground, (1+(-game->level.fg_pos))*al_get_bitmap_width(game->level.foreground), 0 ,0);

	al_set_target_bitmap(game->level.meter_bmp);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_filled_rounded_rectangle(al_get_bitmap_width(game->level.meter_bmp)*0.1, al_get_bitmap_height(game->level.meter_bmp)*0.34, al_get_bitmap_width(game->level.meter_bmp)*0.993, al_get_bitmap_height(game->level.meter_bmp)*0.66,
																	 6,6, al_map_rgb(232,234,239));
	al_draw_horizontal_gradient_rect(al_get_bitmap_width(game->level.meter_bmp)-game->viewportWidth*0.215, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2, game->viewportWidth*0.215*0.975, game->viewportHeight*0.025, al_map_rgb(150,159,182), al_map_rgb(130,139,162));
	al_draw_filled_rectangle(al_get_bitmap_width(game->level.meter_bmp)-game->viewportWidth*0.215, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2, al_get_bitmap_width(game->level.meter_bmp)-game->viewportWidth*0.215+(game->viewportWidth*0.215*0.975)*game->level.hp, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2+game->viewportHeight*0.025, al_map_rgb(214,172,55));
	al_draw_bitmap(game->level.meter_image, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_bitmap(game->level.meter_bmp, al_map_rgba(game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha), game->viewportWidth*0.95-al_get_bitmap_width(game->level.meter_bmp), game->viewportHeight*0.975-al_get_bitmap_height(game->level.meter_bmp), 0);

	TM_Draw();
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
	game->level.derpy_angle = 0;
	game->level.sheet_speed = 2.4;
	game->level.sheet_tmp = 0;
	game->level.handle_input = false;
	game->level.meter_alpha=0;
	game->level.debug_show_sprite_frames=false;
	al_clear_to_color(al_map_rgb(0,0,0));
	TM_Init(game);
	if (game->level.current_level!=1) Moonwalk_Load(game);
	else {
		Dodger_Load(game);
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

		// init level specific obstacle for Dodger module
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
		obst->angle = 0;
		obst->callback = NULL;
		obst->data = NULL;
		obst->bitmap = &(game->level.owl);
		game->level.dodger.obstacles = obst;
	}
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F2)) {
		game->level.hp -= 0.1;
		if (game->level.hp <= 0) game->level.hp=0.001;
	} else if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F3)) {
		game->level.hp += 0.1;
		if (game->level.hp > 1) game->level.hp=1;
	} else if ((game->debug) && (ev->keyboard.keycode==ALLEGRO_KEY_F4)) {
		game->level.debug_show_sprite_frames = !game->level.debug_show_sprite_frames;
	}
	if (game->level.current_level==1) Dodger_Keydown(game, ev);
	else Moonwalk_Keydown(game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	}
	return 0;
}

void Level_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level==1)	Dodger_ProcessEvent(game, ev);
	else Moonwalk_ProcessEvent(game, ev);
	TM_HandleEvent(ev);
}

void Level_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	game->level.current_level = game->level.input.current_level;
	game->level.derpy_sheets = NULL;
	game->level.derpy = NULL;
	game->level.unloading = false;
	Pause_Preload(game);
	RegisterDerpySpritesheet(game, "stand");
	if (game->level.current_level!=1) {
		RegisterDerpySpritesheet(game, "walk");
		Moonwalk_Preload(game, progress);
	}
	else {
		RegisterDerpySpritesheet(game, "walk");
		RegisterDerpySpritesheet(game, "fly");
		RegisterDerpySpritesheet(game, "run");

		Dodger_Preload(game, progress);

		game->level.sample = al_load_sample( GetDataFilePath("levels/1/music.flac") );
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
	if (game->level.unloading) return;
	game->level.unloading = true;
	Pause_Unload_Real(game);
	FadeGameState(game, false);
	al_destroy_sample_instance(game->level.music);
	al_destroy_sample(game->level.sample);
	Level_UnloadBitmaps(game);
	if (game->level.current_level!=1) Moonwalk_Unload(game);
	else Dodger_Unload(game);
	TM_Destroy();
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
		Dodger_UnloadBitmaps(game);
		al_destroy_font(game->level.letter_font);
		al_destroy_bitmap(game->level.letter);
		al_destroy_bitmap(game->level.owl);
	}
	al_destroy_bitmap(game->level.foreground);
	al_destroy_bitmap(game->level.background);
	al_destroy_bitmap(game->level.clouds);
	al_destroy_bitmap(game->level.stage);
	al_destroy_bitmap(game->level.meter_bmp);
	al_destroy_bitmap(game->level.meter_image);
	al_destroy_bitmap(game->level.welcome);
}

void Level_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(19);
	int x = 0;
	struct Spritesheet *tmp = game->level.derpy_sheets;
	while (tmp) {
		x++;
		tmp = tmp->next;
	}
	if (game->level.current_level==1) load_a+=x;
	else load_a=9+x;

	tmp = game->level.derpy_sheets;
	while (tmp) {
		char filename[255] = { };
		sprintf(filename, "levels/derpy/%s.png", tmp->name);
		tmp->bitmap = LoadScaledBitmap(filename, (int)(game->viewportHeight*0.25*tmp->aspect*tmp->scale)*tmp->cols, (int)(game->viewportHeight*0.25*tmp->scale)*tmp->rows);
		PROGRESS;
		tmp = tmp->next;
	}
	PROGRESS;
	if (!game->level.derpy) SelectDerpySpritesheet(game, "stand");

	game->level.derpy = al_create_bitmap(al_get_bitmap_width(*(game->level.derpy_sheet))/game->level.sheet_cols, al_get_bitmap_height(*(game->level.derpy_sheet))/game->level.sheet_rows);
	
	/* TODO: maybe handle strange display aspects */
	game->level.clouds = LoadScaledBitmap("levels/1/clouds.png", game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.foreground = LoadScaledBitmap("levels/1/foreground.png", game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.background = LoadScaledBitmap("levels/1/background.png", game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.stage = LoadScaledBitmap("levels/1/stage.png", game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.meter_image = LoadScaledBitmap("levels/meter.png", game->viewportWidth*0.075, game->viewportWidth*0.075*0.96470588235294117647);
	PROGRESS;
	game->level.meter_bmp = al_create_bitmap(game->viewportWidth*0.2+al_get_bitmap_width(game->level.meter_image), al_get_bitmap_height(game->level.meter_image));
	PROGRESS;
	game->level.welcome = al_create_bitmap(game->viewportWidth, game->viewportHeight/2);
	PROGRESS;
	al_set_target_bitmap(game->level.welcome);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.1, ALLEGRO_ALIGN_CENTRE, "Level 1");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.275, ALLEGRO_ALIGN_CENTRE, "Fluttershy");
	PROGRESS;


	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game, progress);
	else {
		game->level.owl = LoadScaledBitmap("levels/owl.png", game->viewportWidth*0.08, game->viewportWidth*0.08);
		PROGRESS;
		game->level.letter_font = al_load_ttf_font(GetDataFilePath("fonts/DejaVuSans.ttf"),game->viewportHeight*0.0225,0 );
		PROGRESS;
		game->level.letter = LoadScaledBitmap("levels/letter.png", game->viewportHeight*1.3, game->viewportHeight*1.2);
		al_set_target_bitmap(game->level.letter);
		float y = 0.20;
		float x = 0.19;
		void draw_text(char* text) {
			al_draw_text(game->level.letter_font, al_map_rgb(0,0,0), al_get_bitmap_width(game->level.letter)*x, game->viewportHeight*y, ALLEGRO_ALIGN_LEFT, text);
			y+=0.028;
		}
		draw_text("Dear Derpy,");
		draw_text("");
		x = 0.20;
		draw_text("I'm glad you decided to help us! I found a few tips");
		draw_text("in my library that might be useful on your mission.");
		draw_text("I would like to share them with you.");
		draw_text("");
		x = 0.21;
		draw_text("Muffins regenerate your energy, so collect as many");
		draw_text("as you can. Cherries can help you as well. But be");
		draw_text("careful and avoid the muffinzombies - they can");
		draw_text("harm you!");
		draw_text("");
		x = 0.22;
		draw_text("Discord is not fully awake yet, but he's already");
		draw_text("causing chaos all over Equestria and his strange");
		draw_text("creatures may try to stop you. Don't let them!");
		draw_text("");
		x = 0.23;
		draw_text("Last but not least - You should be able to see the");
		draw_text("constellation Orion in the sky tonight. Be sure to");
		draw_text("take a moment to look for it if you have one to");
		draw_text("spare. It's beautiful!");
		draw_text("");
		x = 0.25;
		draw_text("The fate of Equestria rests in your hooves.");
		draw_text("Be safe and good luck!");
		draw_text("");
		x = 0.26;
		draw_text("Yours,");
		draw_text("Twilight Sparkle");
		al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), al_get_bitmap_width(game->level.letter)*0.5, al_get_bitmap_height(game->level.letter)*0.8, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...");
		al_set_target_bitmap(al_get_backbuffer(game->display));
		PROGRESS;

		Dodger_PreloadBitmaps(game, progress);
		al_set_target_bitmap(al_get_backbuffer(game->display));
	}
}
