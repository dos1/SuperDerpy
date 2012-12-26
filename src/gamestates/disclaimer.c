/*! \file disclaimer.c
 *  \brief Disclaimer about early release preview.
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
//#include <stdio.h>
#include "../utils.h"

struct {
		ALLEGRO_FONT *font, *font_small;
} res;
// FIXME: bad bad bad bad!

void Gamestate_Logic(struct Game *game) {
}

void Gamestate_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	DrawTextWithShadow(res.font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.3, ALLEGRO_ALIGN_CENTRE, "This is an early development preview of the game.");
	DrawTextWithShadow(res.font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.4, ALLEGRO_ALIGN_CENTRE, "It's not supposed to be complete!");
	DrawTextWithShadow(res.font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Keep in mind that everything may be changed");
	DrawTextWithShadow(res.font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "and many things surely will change.");
	DrawTextWithShadow(res.font_small, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.9, ALLEGRO_ALIGN_CENTRE, "Press any key to continue...");
}

void Gamestate_Start(struct Game *game) {
	FadeGamestate(game, true);
}

int Gamestate_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	StopGamestate(game, "disclaimer");
	UnloadGamestate(game, "disclaimer");
	LoadGamestate(game, "intro");
	StartGamestate(game, "intro");
	return 0;
}

void Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	res.font_small = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.05,0 );
	res.font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.065,0 );
	//PrintConsole(game, "Preloading GAMESTATE_INTRO...");
	//Intro_Preload(game, progress);
}

void Gamestate_Stop(struct Game *game) {
	FadeGamestate(game, false);
}

void Gamestate_Unload(struct Game *game) {
	al_destroy_font(res.font);
	al_destroy_font(res.font_small);
}

void Gamestate_Reload(struct Game *game) {}

void Gamestate_Resume(struct Game *game) {}
void Gamestate_Pause(struct Game *game) {}
void Gamestate_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev) {}

int Gamestate_ProgressCount = 0;
