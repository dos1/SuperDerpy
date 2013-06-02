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
#include "../gamestates/level.h"
#include "actions.h"
#include "modules/dodger.h"
#include "modules/dodger/actions.h"
#include "level1.h"

void Level1_Load(struct Game *game) {
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
	obst->bitmap = &(game->level.level1.owl);
	game->level.dodger.obstacles = obst;
}

void Level1_Unload(struct Game *game) {
	Dodger_Unload(game);
}

void Level1_UnloadBitmaps(struct Game *game) {
	Dodger_UnloadBitmaps(game);
	al_destroy_font(game->level.letter_font);
	al_destroy_bitmap(game->level.letter);
	al_destroy_bitmap(game->level.level1.owl);
}

void Level1_Preload(struct Game *game) {
	Dodger_Preload(game);
}

inline int Level1_PreloadSteps(void) {
	return 4+Dodger_PreloadSteps();
}

void Level1_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(Level1_PreloadSteps());
	game->level.level1.owl = LoadScaledBitmap("levels/1/owl.png", game->viewport.height*0.1275, game->viewport.height*0.1275);
	PROGRESS;
	game->level.letter_font = al_load_ttf_font(GetDataFilePath("fonts/DejaVuSans.ttf"),game->viewport.height*0.0225,0 );
	PROGRESS;
	game->level.letter = LoadScaledBitmap("levels/1/letter.png", game->viewport.height*1.3, game->viewport.height*1.2);
	al_set_target_bitmap(game->level.letter);
	float y = 0.20;
	float x = 0.19;
	void draw_text(char* text) {
		al_draw_text(game->level.letter_font, al_map_rgb(0,0,0), al_get_bitmap_width(game->level.letter)*x, game->viewport.height*y, ALLEGRO_ALIGN_LEFT, text);
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

	al_set_target_bitmap(game->level.welcome);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.1, ALLEGRO_ALIGN_CENTRE, "Level 1");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.275, ALLEGRO_ALIGN_CENTRE, "Fluttershy");
	PROGRESS;
	al_set_target_bitmap(al_get_backbuffer(game->display));

	Dodger_PreloadBitmaps(game, progress);
}

void Level1_Draw(struct Game *game) {
	Dodger_Draw(game);
}

void Level1_Logic(struct Game *game) {
	Dodger_Logic(game);
}

void Level1_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	Dodger_Keydown(game, ev);
}

void Level1_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {
	Dodger_ProcessEvent(game, ev);
}

void Level1_Resume(struct Game *game) {
	Dodger_Resume(game);
}

void Level1_Pause(struct Game *game) {
	Dodger_Pause(game);
}
