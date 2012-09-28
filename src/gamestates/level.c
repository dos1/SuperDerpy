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
#include "../levels/moonwalk.h"
#include "../levels/level1.h"
#include "../config.h"
#include "pause.h"
#include "level.h"
#include "../timeline.h"

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
	struct Spritesheet *s = game->level.derpy_sheets;
	while (s) {
		if (!strcmp(s->name, name)) {
			//PrintConsole(game, "Derpy spritesheet %s already registered!", name);
			return;
		}
		s = s->next;
	}
	char filename[255] = { };
	sprintf(filename, "levels/derpy/%s.ini", name);
	ALLEGRO_CONFIG *config = al_load_config_file(GetDataFilePath(filename));
	s = malloc(sizeof(struct Spritesheet));
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
		Level1_Logic(game);
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
	if (game->level.current_level==1)	Level1_Resume(game);
	else Moonwalk_Resume(game);
	TM_Resume();
}

void Level_Pause(struct Game *game) {
	game->level.music_pos = al_get_sample_instance_position(game->level.music);
	al_set_sample_instance_playing(game->level.music, false);
	if (game->level.current_level==1)	Level1_Pause(game);
	else Moonwalk_Pause(game);
	TM_Pause();
}

void Level_Draw(struct Game *game) {
	al_draw_bitmap(game->level.clouds, (-game->level.cl_pos)*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.clouds, (1+(-game->level.cl_pos))*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.background, (-game->level.bg_pos)*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.background, (1+(-game->level.bg_pos))*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.stage, (-game->level.st_pos)*al_get_bitmap_width(game->level.stage), 0 ,0);
	al_draw_bitmap(game->level.stage, (1+(-game->level.st_pos))*al_get_bitmap_width(game->level.stage), 0 ,0);

	if (game->level.current_level==1) Level1_Draw(game);
	else Moonwalk_Draw(game);

	if (!game->level.foreground) return;

	al_draw_bitmap(game->level.foreground, (-game->level.fg_pos)*al_get_bitmap_width(game->level.foreground), 0 ,0);
	al_draw_bitmap(game->level.foreground, (1+(-game->level.fg_pos))*al_get_bitmap_width(game->level.foreground), 0 ,0);

	al_set_target_bitmap(game->level.meter_bmp);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_filled_rounded_rectangle(al_get_bitmap_width(game->level.meter_bmp)*0.1, al_get_bitmap_height(game->level.meter_bmp)*0.34, al_get_bitmap_width(game->level.meter_bmp)*0.993, al_get_bitmap_height(game->level.meter_bmp)*0.66, 6,6, al_map_rgb(232,234,239));
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
		Level1_Load(game);
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
	if (game->level.current_level==1) Level1_Keydown(game, ev);
	else Moonwalk_Keydown(game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		PauseGameState(game);
		Pause_Load(game);
	}
	return 0;
}

void Level_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	if (game->level.current_level==1)	Level1_ProcessEvent(game, ev);
	else Moonwalk_ProcessEvent(game, ev);
	TM_HandleEvent(ev);
}

void Level_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PrintConsole(game, "Initializing level %d...", game->level.input.current_level);

	game->level.current_level = game->level.input.current_level;
	game->level.derpy_sheets = NULL;
	game->level.derpy = NULL;
	game->level.unloading = false;
	Pause_Preload(game);
	RegisterDerpySpritesheet(game, "stand"); // default

	//TODO: load proper music file for each level
	game->level.sample = al_load_sample( GetDataFilePath("levels/1/music.flac") );

	if (game->level.current_level==1) Level1_Preload(game);
	else Moonwalk_Preload(game);

	Level_PreloadBitmaps(game, progress);

	game->level.music = al_create_sample_instance(game->level.sample);
	al_attach_sample_instance_to_mixer(game->level.music, game->audio.music);
	al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

	if (!game->level.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

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
	else Level1_Unload(game);
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
	else Level1_UnloadBitmaps(game);
	al_destroy_bitmap(game->level.foreground);
	al_destroy_bitmap(game->level.background);
	al_destroy_bitmap(game->level.clouds);
	al_destroy_bitmap(game->level.stage);
	al_destroy_bitmap(game->level.meter_bmp);
	al_destroy_bitmap(game->level.meter_image);
	al_destroy_bitmap(game->level.welcome);
	game->level.foreground = NULL;
}

int Level_PreloadSteps(struct Game *game) {
	if (game->level.current_level==1) return Level1_PreloadSteps();
	else return Moonwalk_PreloadSteps();
}

void Level_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	int x = 0;

	struct Spritesheet *tmp = game->level.derpy_sheets;
	while (tmp) {
		x++;
		tmp = tmp->next;
	}

	PROGRESS_INIT(8+x+Level_PreloadSteps(game));

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
	
	//TODO: load proper bitmap files for each level
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

	void ChildProgress(struct Game* game, float p) {
		if (progress) (*progress)(game, load_p+=1/load_a);
	}
	if (game->level.current_level!=1) Moonwalk_PreloadBitmaps(game, &ChildProgress);
	else Level1_PreloadBitmaps(game, &ChildProgress);
}
