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
#include "../config.h"
#include "pause.h"
#include "menu.h"
#include "level.h"
#include "loading.h"

int Pause_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((game->menu.menustate==MENUSTATE_OPTIONS) && ((ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) || ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)))) {
		al_play_sample_instance(game->menu.click);
		ChangeMenuState(game,MENUSTATE_PAUSE);
	} else if ((game->menu.menustate==MENUSTATE_VIDEO) && ((ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) || ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)))) {
		al_play_sample_instance(game->menu.click);
		ChangeMenuState(game,MENUSTATE_OPTIONS);
		if (game->menu.options.fullscreen!=game->fullscreen) {
			al_toggle_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, game->menu.options.fullscreen);
			al_clear_to_color(al_map_rgb(0,0,0));
			al_flip_display();
			game->fullscreen = game->menu.options.fullscreen;
			if (game->fullscreen) al_hide_mouse_cursor(game->display);
			else al_show_mouse_cursor(game->display);
			Shared_Unload(game);
			al_clear_to_color(al_map_rgb(0,0,0));
			al_flip_display();
			SetupViewport(game);
			Shared_Load(game);

			void Progress(struct Game *game, float p) {
				al_set_target_bitmap(al_get_backbuffer(game->display));
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), game->viewportWidth*0.0234, game->viewportHeight*0.84, ALLEGRO_ALIGN_LEFT, "Loading...");
				al_draw_filled_rectangle(0, game->viewportHeight*0.985, game->viewportWidth, game->viewportHeight, al_map_rgba(128,128,128,128));
				al_draw_filled_rectangle(0, game->viewportHeight*0.985, p*game->viewportWidth, game->viewportHeight, al_map_rgba(255,255,255,255));
				al_flip_display();
			}
			Progress(game, 0);

			Loading_Unload(game);
			Loading_Load(game);
			Menu_Unload(game);
			Menu_Preload(game, NULL);
			Level_UnloadBitmaps(game);
			Level_PreloadBitmaps(game, &Progress);
			Pause_Unload_Real(game);
			Pause_Preload(game);
			Pause_Load(game);
		}
	} else return Menu_Keydown(game, ev);
	return 0;
}

void Pause_Preload(struct Game* game) {
	game->pause.bitmap = NULL;
	game->pause.derpy = LoadScaledBitmap("levels/derpy_pause.png", game->viewportHeight*1.6*0.53, game->viewportHeight*0.604);
	PrintConsole(game,"Pause preloaded.");
	if (!game->menu.loaded) {
		PrintConsole(game,"Pause: Preloading GAMESTATE_MENU...");
		Menu_Preload(game, NULL);
	}
}

void Pause_Load(struct Game* game) {
	ALLEGRO_BITMAP *fade = al_create_bitmap(game->viewportWidth, game->viewportHeight);
	al_set_target_bitmap(fade);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	game->pause.bitmap = fade;
	ChangeMenuState(game,MENUSTATE_PAUSE);
	PrintConsole(game,"Game paused.");
	al_play_sample_instance(game->menu.click);
}

void Pause_Draw(struct Game* game) {
	game->gamestate=game->loadstate;
	game->loadstate=GAMESTATE_PAUSE;
	DrawGameState(game);
	game->loadstate=game->gamestate;
	game->gamestate=GAMESTATE_PAUSE;
	al_draw_tinted_bitmap(game->pause.bitmap,al_map_rgba_f(1,1,1,0.75),0,0,0);
	al_draw_bitmap(game->pause.derpy, game->viewportWidth-al_get_bitmap_width(game->pause.derpy), game->viewportHeight*0.4, 0);
	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.275, ALLEGRO_ALIGN_CENTRE, "Game paused.");
	DrawMenuState(game);
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
