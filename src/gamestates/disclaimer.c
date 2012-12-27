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

#include <allegro5/allegro_ttf.h>
#include "../utils.h"
#include "disclaimer.h"

int Gamestate_ProgressCount = 2;

void Gamestate_Logic(struct Game *game, struct DisclaimerResources* data) {}

void Gamestate_Draw(struct Game *game, struct DisclaimerResources* data) {
	al_clear_to_color(al_map_rgb(0,0,0));
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.3, ALLEGRO_ALIGN_CENTRE, "This is an early development preview of the game.");
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.4, ALLEGRO_ALIGN_CENTRE, "It's not supposed to be complete!");
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Keep in mind that everything may be changed");
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "and many things surely will change.");
	DrawTextWithShadow(data->font_small, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.9, ALLEGRO_ALIGN_CENTRE, "Press any key to continue...");
}

void Gamestate_Start(struct Game *game, struct DisclaimerResources* data) {
	FadeGamestate(game, true);
}

void Gamestate_ProcessEvent(struct Game *game, struct DisclaimerResources* data, ALLEGRO_EVENT *ev) {
	if (ev->type==ALLEGRO_EVENT_KEY_DOWN) {
		SwitchGamestate(game, "disclaimer", "intro");
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	struct DisclaimerResources *data = malloc(sizeof(struct DisclaimerResources));
	data->font_small = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.05,0 );
	(*progress)(game);
	data->font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.065,0 );
	(*progress)(game);
	return data;
}

void Gamestate_Stop(struct Game *game, struct DisclaimerResources* data) {
	FadeGamestate(game, false);
}

void Gamestate_Unload(struct Game *game, struct DisclaimerResources* data) {
	al_destroy_font(data->font);
	al_destroy_font(data->font_small);
	free(data);
}

void Gamestate_Reload(struct Game *game, struct DisclaimerResources* data) {}

void Gamestate_Resume(struct Game *game, struct DisclaimerResources* data) {}
void Gamestate_Pause(struct Game *game, struct DisclaimerResources* data) {}
