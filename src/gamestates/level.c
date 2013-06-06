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
#include "../utils.h"
#include "../timeline.h"
#include "../config.h"
#include "level.h"

#include <stdio.h>

// TODO: move this file to better place
// it's now a place for level related utilities - certainly not a gamestate

void SelectSpritesheet(struct Game *game, struct Character *character, char* name) {
	struct Spritesheet *tmp = character->spritesheets;
	PrintConsole(game, "Selecting spritesheet for %s: %s", character->name, name);
	if (!tmp) {
		PrintConsole(game, "ERROR: No spritesheets registered for %s!", character->name);
		return;
	}
	while (tmp) {
		if (!strcmp(tmp->name, name)) {
			character->spritesheet = tmp;
			//game->level.sheet_rows = tmp->rows;
			//game->level.sheet_cols = tmp->cols;
			//game->level.sheet_blanks = tmp->blanks;
			//game->level.sheet_speed_modifier = tmp->speed;
			character->pos = 0;
			//game->level.sheet_scale = tmp->scale;
			//game->level.sheet_successor = tmp->successor;
			if (character->bitmap) al_destroy_bitmap(character->bitmap);
			character->bitmap = al_create_bitmap((game->viewport.height*0.25)*tmp->aspect*tmp->scale, (game->viewport.height*0.25)*tmp->scale); // FIXME: dimensions!
			PrintConsole(game, "SUCCESS: Spritesheet for %s activated: %s (%dx%d)", character->name, name, al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap));
			return;
		}
		tmp = tmp->next;
	}
	PrintConsole(game, "ERROR: No spritesheets registered for %s with given name: %s", character->name, name);
	return;
}

void LoadSpritesheets(struct Game *game, struct Character *character) {
	PrintConsole(game, "Loading spritesheets for character %s...", character->name);
	struct Spritesheet *tmp = character->spritesheets;
	while (tmp) {
		if (!tmp->bitmap) {
			char filename[255] = { };
			snprintf(filename, 255, "levels/%s/%s.png", character->name, tmp->name);
			tmp->bitmap = LoadScaledBitmap(game, filename, (int)(game->viewport.height*0.25*tmp->aspect*tmp->scale)*tmp->cols, (int)(game->viewport.height*0.25*tmp->scale)*tmp->rows);
		}
		tmp = tmp->next;
	}
}

void UnloadSpritesheets(struct Game *game, struct Character *character) {
	PrintConsole(game, "Unloading spritesheets for character %s...", character->name);
	struct Spritesheet *tmp = character->spritesheets;
	while (tmp) {
		if (tmp->bitmap) al_destroy_bitmap(tmp->bitmap);
		tmp->bitmap = NULL;
		tmp = tmp->next;
	}
}

void RegisterSpritesheet(struct Game *game, struct Character *character, char* name) {
	struct Spritesheet *s = character->spritesheets;
	while (s) {
		if (!strcmp(s->name, name)) {
			//PrintConsole(game, "%s spritesheet %s already registered!", character->name, name);
			return;
		}
		s = s->next;
	}
	PrintConsole(game, "Registering %s spritesheet: %s", character->name, name);
	char filename[255] = { };
	snprintf(filename, 255, "levels/%s/%s.ini", character->name, name);
	ALLEGRO_CONFIG *config = al_load_config_file(GetDataFilePath(game, filename));
	s = malloc(sizeof(struct Spritesheet));
	s->name = strdup(name);
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
		strncpy(s->successor, successor, 255);
	}
	s->next = character->spritesheets;
	character->spritesheets = s;
	al_destroy_config(config);
}

struct Character* CreateCharacter(struct Game *game, char* name) {
	PrintConsole(game, "Creating character %s...", name);
	struct Character *character = malloc(sizeof(struct Character));
	character->name = strdup(name);
	character->angle = 0;
	character->bitmap = NULL;
	character->data = NULL;
	character->pos = 0;
	character->pos_tmp = 0;
	character->x = -1;
	character->y = -1;
	character->spritesheets = NULL;
	character->spritesheet = NULL;
	return character;
}

void DestroyCharacter(struct Game *game, struct Character *character) {
	PrintConsole(game, "Destroying character %s...", character->name);
	UnloadSpritesheets(game, character);
	struct Spritesheet *tmp, *s = character->spritesheets;
	tmp = s;
	while (s) {
		tmp = s;
		s = s->next;
		free(tmp);
	}

	if (character->bitmap) al_destroy_bitmap(character->bitmap);
	free(character->name);
	free(character);
}

void AnimateCharacter(struct Game *game, struct Character *character, float speed_modifier) {
	if ((character->spritesheet->speed) && (speed_modifier)) {
		character->pos_tmp+=character->spritesheet->speed*speed_modifier;
		while (character->pos_tmp >= 1) {
			character->pos++;
			character->pos_tmp--;
		}
		if (character->pos>=character->spritesheet->cols*character->spritesheet->rows-character->spritesheet->blanks) {
			character->pos=0;
			if (character->spritesheet->successor) {
				SelectSpritesheet(game, character, character->spritesheet->successor);
			}
		}
	}
}

void MoveCharacter(struct Game *game, struct Character *character, float x, float y, float angle) {
	character->x += x;
	character->y += y;
	character->angle += angle;
}

void SetCharacterPosition(struct Game *game, struct Character *character, float x, float y, float angle) {
	character->x = x;
	character->y = y;
	character->angle = angle;
}

void AdvanceLevel(struct Game *game, int current_level, bool last) {
	if (!last) {
		int available = atoi(GetConfigOptionDefault(game, "MuffinAttack", "level", "1"));
		available++;
		if ((available<2) || (available>7)) available=1;
		if (available==(current_level+1)) {
			char* text = malloc(255*sizeof(char));
			snprintf(text, 255, "%d", available);
			SetConfigOption(game, "MuffinAttack", "level", text);
			free(text);
		}
	} else {
		SetConfigOption(game, "MuffinAttack", "completed", "1");
	}
}

/*char* GetLevelFilename(struct Game *game, char* filename) {
	// FIXME: it should work with larger numbers too
	char* name = strdup(filename);
	char* ch = strchr(name, '?');
	ch[0] = '0' + game->level.current_level;
	return name;
}*/

void DrawCharacter(struct Game *game, struct Character *character, ALLEGRO_COLOR tilt, int flags) {
	al_set_target_bitmap(character->bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(character->spritesheet->bitmap, al_get_bitmap_width(character->bitmap)*(character->pos%character->spritesheet->cols),al_get_bitmap_height(character->bitmap)*(character->pos/character->spritesheet->cols),al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_rotated_bitmap(character->bitmap, tilt, al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap)/2, character->x*game->viewport.width + al_get_bitmap_width(character->bitmap), character->y*game->viewport.height + al_get_bitmap_height(character->bitmap)/2, character->angle, flags); // FIXME: viewport height? omg character should have its dimensions ;_;

}

/*
void Level_Logic(struct Game *game) {
	LEVELS(Logic, game);

	if ((game->level.sheet_speed) && (game->level.sheet_speed_modifier)) {
		game->level.sheet_tmp+=1;
		if (game->level.sheet_tmp >= (game->level.sheet_speed/game->level.speed_modifier)/game->level.sheet_speed_modifier) {
			game->level.sheet_pos++;
			game->level.sheet_tmp -= (game->level.sheet_speed/game->level.speed_modifier)/game->level.sheet_speed_modifier;
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
	LEVELS(Resume, game);
	TM_Resume();
}

void Level_Pause(struct Game *game) {
	game->level.music_pos = al_get_sample_instance_position(game->level.music);
	al_set_sample_instance_playing(game->level.music, false);
	LEVELS(Pause, game);
	TM_Pause();
}

void Level_Draw(struct Game *game) {
	al_draw_bitmap(game->level.clouds, (-game->level.cl_pos)*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.clouds, (1+(-game->level.cl_pos))*al_get_bitmap_width(game->level.clouds), 0, 0);
	al_draw_bitmap(game->level.background, (-game->level.bg_pos)*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.background, (1+(-game->level.bg_pos))*al_get_bitmap_width(game->level.background), 0, 0);
	al_draw_bitmap(game->level.stage, (-game->level.st_pos)*al_get_bitmap_width(game->level.stage), 0 ,0);
	al_draw_bitmap(game->level.stage, (1+(-game->level.st_pos))*al_get_bitmap_width(game->level.stage), 0 ,0);

	LEVELS(Draw, game);

	if (!game->level.foreground) return;

	al_draw_bitmap(game->level.foreground, (-game->level.fg_pos)*al_get_bitmap_width(game->level.foreground), 0 ,0);
	al_draw_bitmap(game->level.foreground, (1+(-game->level.fg_pos))*al_get_bitmap_width(game->level.foreground), 0 ,0);

	al_set_target_bitmap(game->level.meter_bmp);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_filled_rounded_rectangle(al_get_bitmap_width(game->level.meter_bmp)*0.1, al_get_bitmap_height(game->level.meter_bmp)*0.34, al_get_bitmap_width(game->level.meter_bmp)*0.993, al_get_bitmap_height(game->level.meter_bmp)*0.66, 6,6, al_map_rgb(232,234,239));
	al_draw_horizontal_gradient_rect(al_get_bitmap_width(game->level.meter_bmp)-game->viewportHeight*1.6*0.215, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2, game->viewportHeight*1.6*0.215*0.975, game->viewportHeight*0.025, al_map_rgb(150,159,182), al_map_rgb(130,139,162));
	al_draw_filled_rectangle(al_get_bitmap_width(game->level.meter_bmp)-game->viewportHeight*1.6*0.215, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2, al_get_bitmap_width(game->level.meter_bmp)-game->viewportHeight*1.6*0.215+(game->viewportHeight*1.6*0.215*0.975)*game->level.hp, (al_get_bitmap_height(game->level.meter_bmp)-game->viewportHeight*0.025)/2+game->viewportHeight*0.025, al_map_rgb(214,172,55));
	al_draw_bitmap(game->level.meter_image, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_bitmap(game->level.meter_bmp, al_map_rgba(game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha,game->level.meter_alpha), game->viewportWidth-al_get_bitmap_width(game->level.meter_bmp)*1.1, game->viewportHeight*0.975-al_get_bitmap_height(game->level.meter_bmp), 0);

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
	LEVELS(Load, game);
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
	LEVELS(Keydown, game, ev);
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		PauseGameState(game);
		Pause_Load(game);
	}
	return 0;
}

void Level_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	LEVELS(ProcessEvent, game, ev);
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

	game->level.sample = al_load_sample( GetDataFilePath(GetLevelFilename(game, "levels/?/music.flac")) );

	LEVELS(Preload, game);

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
	LEVELS(Unload, game);
	TM_Destroy();
}


void Level_UnloadBitmaps(struct Game *game) {
	al_destroy_bitmap(game->level.derpy);
	struct Spritesheet *tmp = game->level.derpy_sheets;
	while (tmp) {
		al_destroy_bitmap(tmp->bitmap);
		tmp = tmp->next;
	}
	LEVELS(UnloadBitmaps, game);
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
	switch (game->level.current_level) {
		case 1:
			return Level1_PreloadSteps(); break;
		case 2:
			return Level2_PreloadSteps(); break;
		case 3:
			return Level3_PreloadSteps(); break;
		case 4:
			return Level4_PreloadSteps(); break;
		case 5:
			return Level5_PreloadSteps(); break;
		case 6:
			return Level6_PreloadSteps(); break;
	}
	return 0;
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
	
	game->level.clouds = LoadScaledBitmap(GetLevelFilename(game, "levels/?/clouds.png"), game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.foreground = LoadScaledBitmap(GetLevelFilename(game, "levels/?/foreground.png"), game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.background = LoadScaledBitmap(GetLevelFilename(game, "levels/?/background.png"), game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.stage = LoadScaledBitmap(GetLevelFilename(game, "levels/?/stage.png"), game->viewportHeight*4.73307291666666666667, game->viewportHeight);
	PROGRESS;
	game->level.meter_image = LoadScaledBitmap("levels/meter.png", game->viewportHeight*1.6*0.075, game->viewportHeight*1.6*0.075*0.96470588235294117647);
	PROGRESS;
	game->level.meter_bmp = al_create_bitmap(game->viewportHeight*1.6*0.2+al_get_bitmap_width(game->level.meter_image), al_get_bitmap_height(game->level.meter_image));
	PROGRESS;
	game->level.welcome = al_create_bitmap(game->viewportWidth, game->viewportHeight/2);
	PROGRESS;

	void ChildProgress(struct Game* game, float p) {
		if (progress) (*progress)(game, load_p+=1/load_a);
	}
	LEVELS(PreloadBitmaps, game, &ChildProgress);
}
*/
