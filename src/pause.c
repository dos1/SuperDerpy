/*! \file pause.c
 *  \brief Pause state.
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
#include "config.h"
#include "pause.h"
#include "menu.h"

int Pause_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) || ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.menustate==MENUSTATE_PAUSE) && (game->menu.selected==0))) {
		al_play_sample_instance(game->menu.click);
		PrintConsole(game,"Game resumed.");
		al_destroy_bitmap(game->pause.bitmap);
		game->pause.bitmap = NULL;
		game->gamestate = game->loadstate;
	}
	else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.menustate==MENUSTATE_PAUSE) && (game->menu.selected==1)) {
		al_play_sample_instance(game->menu.click);
		game->gamestate=game->loadstate;
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
		al_play_sample_instance(game->menu.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
		al_play_sample_instance(game->menu.click);
	} else if ((game->menu.menustate==MENUSTATE_PAUSE) && (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)) || (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) {
		al_play_sample_instance(game->menu.click);
		return 1;
	} else if (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.menustate==MENUSTATE_PAUSE) && (game->menu.selected==2)) || (((game->menu.menustate==MENUSTATE_OPTIONS) && ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) || (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3))))) {
		al_play_sample_instance(game->menu.click);
		if (game->menu.menustate==MENUSTATE_PAUSE) game->menu.menustate=MENUSTATE_OPTIONS;
		else game->menu.menustate=MENUSTATE_PAUSE;
		game->menu.selected=0;
		PrintConsole(game, "options state changed %d", game->menu.menustate);
	} else if ((game->menu.menustate==MENUSTATE_OPTIONS) && (game->menu.selected==2)) {
		if ((game->music) && (game->fx)) { game->music=0; SetConfigOption("SuperDerpy", "music", "0");
			al_detach_mixer(game->audio.music);
		}
		else if (game->fx) { game->music=1; game->fx=0; SetConfigOption("SuperDerpy", "music", "1"); SetConfigOption("SuperDerpy", "fx", "0");
			al_attach_mixer_to_mixer(game->audio.music, game->audio.mixer);
			al_detach_mixer(game->audio.fx);
		}
		else if (game->music) { game->music=0; SetConfigOption("SuperDerpy", "music", "0");
			al_detach_mixer(game->audio.music);
		}
		else { game->music=1; game->fx=1; SetConfigOption("SuperDerpy", "music", "1"); SetConfigOption("SuperDerpy", "fx", "1");
			al_attach_mixer_to_mixer(game->audio.fx, game->audio.mixer);
			al_attach_mixer_to_mixer(game->audio.music, game->audio.mixer);
		}
		al_play_sample_instance(game->menu.click);
	}
	if (game->menu.selected==-1) game->menu.selected=3;
	if (game->menu.selected==4) game->menu.selected=0;
	return 0;
}

void Pause_Preload(struct Game* game) {
	game->pause.bitmap = NULL;
	game->pause.derpy = LoadScaledBitmap("derpy_pause.png", al_get_display_width(game->display)*0.53, al_get_display_height(game->display)*0.604);
	PrintConsole(game,"Pause preloaded.");
	if (!game->menu.loaded) Menu_Preload(game);
}

void Pause_Load(struct Game* game) {
	game->gamestate=game->loadstate;
	game->loadstate=GAMESTATE_PAUSE;
	DrawGameState(game);
	game->loadstate=game->gamestate;
	game->gamestate=GAMESTATE_PAUSE;
	ALLEGRO_BITMAP *fade = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(fade);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_draw_tinted_bitmap(fade,al_map_rgba_f(1,1,1,0.75),0,0,0);
	//al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255),  al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE,"Game paused!");
	game->pause.bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->pause.bitmap);
	al_draw_bitmap(al_get_backbuffer(game->display), 0, 0, 0);
	al_draw_bitmap(game->pause.derpy, 0.47*al_get_display_width(game->display), al_get_display_height(game->display)*0.396, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(fade);
	game->menu.selected=0;
	game->menu.menustate = MENUSTATE_PAUSE;
	PrintConsole(game,"Game paused.");
	al_play_sample_instance(game->menu.click);
}

void Pause_Draw(struct Game* game) {
	al_draw_bitmap(game->pause.bitmap, 0, 0, 0);
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Game paused.");

	DrawMenuState(game);

	DrawConsole(game);
}

void Pause_Unload_Real(struct Game* game) {
	PrintConsole(game,"Pause unloaded.");
	if (game->pause.bitmap) al_destroy_bitmap(game->pause.bitmap);
	al_destroy_bitmap(game->pause.derpy);
}

void Pause_Unload(struct Game* game) {
	game->gamestate=game->loadstate;
	UnloadGameState(game);
	PrintConsole(game, "Pause: Unloading GAMESTATE_MENU...");
	Menu_Unload(game);
}