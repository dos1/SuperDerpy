/*! \file level1.c
 *  \brief Level 1 code.
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
#include "allegro5/allegro_ttf.h"
#include "../timeline.h"
#include "../gamestates/level.h"
#include "actions.h"
#include "../utils.h"
#include "modules/dodger.h"
#include "modules/dodger/actions.h"
#include "level1.h"

int Gamestate_ProgressCount = 2;

void Gamestate_Start(struct Game *game, struct Level1Resources* data) {
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

	TM_WrapArg(int, level, 1);
	TM_AddAction(&PassLevel, TM_AddToArgs(NULL, 2, strdup("level1"), level), "passlevel");

	// init level specific obstacle (owl) for Dodger module
	struct Obstacle *obst = malloc(sizeof(struct Obstacle));
	obst->prev = NULL;
	obst->next = NULL;
	obst->x = (game->viewport.height*1.33625)/game->viewport.width*100;
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
	obst->bitmap = &(data->owl);
	data->dodger->obstacles = obst;
}

void Gamestate_Reload(struct Game *game, struct Level1Resources* data) {}

void Gamestate_Stop(struct Game *game, struct Level1Resources* data) {

}

void Gamestate_Unload(struct Game *game, struct Level1Resources* data) {
	Dodger_Unload(game, data->dodger);
}

void Gamestate_UnloadBitmaps(struct Game *game, struct Level1Resources* data) {
	Dodger_UnloadBitmaps(game, data->dodger);
	al_destroy_font(data->letter_font);
	al_destroy_bitmap(data->letter);
	al_destroy_bitmap(data->owl);
}


void Gamestate_PreloadBitmaps(struct Game *game, struct Level1Resources* data) {
	data->welcome = al_create_bitmap(game->viewport.width, game->viewport.height/2);

	data->font_title = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.16,0 );
	data->font_subtitle = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.08,0 );
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.05,0 );

	data->owl = LoadScaledBitmap(game, "levels/1/owl.png", game->viewport.height*0.1275, game->viewport.height*0.1275);
	data->letter_font = al_load_ttf_font(GetDataFilePath(game, "fonts/DejaVuSans.ttf"),game->viewport.height*0.0225,0 );
	data->letter = LoadScaledBitmap(game, "levels/1/letter.png", game->viewport.height*1.3, game->viewport.height*1.2);
	al_set_target_bitmap(data->letter);
	float y = 0.20;
	float x = 0.19;
	void draw_text(char* text) {
		al_draw_text(data->letter_font, al_map_rgb(0,0,0), al_get_bitmap_width(data->letter)*x, game->viewport.height*y, ALLEGRO_ALIGN_LEFT, text);
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
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), al_get_bitmap_width(data->letter)*0.5, al_get_bitmap_height(data->letter)*0.8, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...");
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_set_target_bitmap(data->welcome);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	DrawTextWithShadow(data->font_title, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.1, ALLEGRO_ALIGN_CENTRE, "Level 1");
	DrawTextWithShadow(data->font_subtitle, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.275, ALLEGRO_ALIGN_CENTRE, "Fluttershy");

	al_set_target_bitmap(al_get_backbuffer(game->display));

	Dodger_PreloadBitmaps(game, data->dodger);
}

struct Level1Resources* Gamestate_Load(struct Game *game) {
	TM_Init(game);

	struct Level1Resources *data = malloc(sizeof(struct Level1Resources));
	struct Character *character = CreateCharacter(game, "derpy");
	RegisterSpritesheet(game, character, "walk");
	RegisterSpritesheet(game, character, "stand");
	RegisterSpritesheet(game, character, "fly");
	RegisterSpritesheet(game, character, "run");
	SelectSpritesheet(game, character, "run");
	SetCharacterPosition(game, character, 0.1, 0.7, 0);
	data->dodger = Dodger_Load(game, character);

	data->failed=false;
	data->cl_pos=0;
	data->bg_pos=0;
	data->fg_pos=0.2;
	data->st_pos=0.1;
	data->handle_input = false;
	data->meter_alpha=0;

	Gamestate_PreloadBitmaps(game, data);
	LoadSpritesheets(game, character);
	return data;
}


void Gamestate_Draw(struct Game *game, struct Level1Resources* data) {
	Dodger_Draw(game, data->dodger);
}

void Gamestate_Logic(struct Game *game, struct Level1Resources* data) {
	TM_Process();
	Dodger_Logic(game, data->dodger);
}

void Gamestate_ProcessEvent(struct Game *game, struct Level1Resources* data, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(ev);
	Dodger_ProcessEvent(game, data->dodger, ev);
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			SwitchGamestate(game, "level1", "map");
		}
	}
}

void Gamestate_Resume(struct Game *game, struct Level1Resources* data) {
	Dodger_Resume(game, data->dodger);
}

void Gamestate_Pause(struct Game *game, struct Level1Resources* data) {
	Dodger_Pause(game, data->dodger);
}
