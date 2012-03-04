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
	if ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) || ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->pause.options==0) && (game->pause.selected==0))) {
		Pause_Unload_Real(game);
		game->gamestate = game->loadstate;
	}
	else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->pause.options==0) && (game->pause.selected==1)) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MAP;
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->pause.selected--;
		if (game->fx) al_play_sample(game->menu.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->pause.selected++;
		if (game->fx) al_play_sample(game->menu.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	} else if ((!game->pause.options) && (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->pause.selected==3)) || (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) {
		if (game->fx) al_play_sample(game->menu.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		return 1;
	} else if (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->pause.options) && (game->pause.selected==2)) || (((game->pause.options) && ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) || (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->pause.selected==3))))) {
		if (game->fx) al_play_sample(game->menu.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		game->pause.options=!game->pause.options;
		game->pause.selected=0;
		PrintConsole(game, "options state changed %d", game->pause.options);
	} else if ((game->pause.options) && (game->pause.selected==2)) {
		al_stop_samples();
		if ((game->music) && (game->fx)) { game->music=0; SetConfigOption("[SuperDerpy]", "music", "0"); }
		else if (game->fx) { game->music=1; game->fx=0; SetConfigOption("[SuperDerpy]", "music", "1"); SetConfigOption("[SuperDerpy]", "fx", "0"); }
		else if (game->music) { game->music=0; SetConfigOption("[SuperDerpy]", "music", "0"); }
		else { game->music=1; game->fx=1; SetConfigOption("[SuperDerpy]", "music", "1"); SetConfigOption("[SuperDerpy]", "fx", "1"); }
		if (game->fx) al_play_sample(game->menu.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		//play_samples(game);
	}
	if (game->pause.selected==-1) game->pause.selected=3;
	if (game->pause.selected==4) game->pause.selected=0;
	return 0;
}

void Pause_Preload(struct Game* game) {
	game->pause.bitmap = NULL;
	game->pause.derpy = LoadScaledBitmap("derpy_pause.png", al_get_display_width(game->display)*0.53, al_get_display_height(game->display)*0.604);
	/*(game->pause.derpy = al_create_bitmap(al_get_display_width(game->display)*0.53, al_get_display_height(game->display)*0.604);

	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	ALLEGRO_BITMAP *derpy = al_load_bitmap( "data/derpy_pause.png" );
	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

	al_set_target_bitmap(game->pause.derpy);
	ScaleBitmap(derpy, al_get_display_width(game->display)*0.53, al_get_display_height(game->display)*0.604);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_destroy_bitmap(derpy);*/
}

void Pause_Load(struct Game* game) {
	PrintConsole(game,"Pause loaded.");
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
	game->pause.selected=0;
	game->pause.options=0;
}

void Pause_Draw(struct Game* game) {
	al_draw_bitmap(game->pause.bitmap, 0, 0, 0);
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Game paused.");

	ALLEGRO_FONT *font;
	char* text;
	font = game->menu.font; if (game->pause.selected==0) font = game->menu.font_selected;
	text = "Resume game"; if (game->pause.options) text="Control settings";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->pause.selected==1) font = game->menu.font_selected;
	text = "Return to map"; if (game->pause.options) text="Video settings";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->pause.selected==2) font = game->menu.font_selected;
	text = "Options"; if (game->pause.options) {
		if ((game->music) && (game->fx))
			text="Sounds: all";
		else if (game->music)
			text="Sounds: music only";
		else if (game->fx)
			text="Sounds: fx only";
		else
			text="Sounds: none";
	}
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->pause.selected==3) font = game->menu.font_selected;
	text = "Exit"; if (game->pause.options) text="Back";
	al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, text);

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