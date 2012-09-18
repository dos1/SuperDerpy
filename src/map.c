/*! \file map.c
 *  \brief View with Map of levels.
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
#include "config.h"
#include "map.h"

void Map_Draw(struct Game *game) {
	al_draw_bitmap(game->map.map, 0, 0, 0);
	float x=0,y=0;
	switch (game->map.selected) {
		case 1:
			x=0.175;
			y=0.25;
			break;
		case 2:
			x=0.5;
			y=0.375;
			break;
		case 3:
			x=0.738;
			y=0.4;
			break;
		case 4:
			x=0.7;
			y=0.7625;
			break;
		case 5:
			x=0.225;
			y=0.75;
			break;
		case 6:
			x=0.425;
			y=0.65;
			break;
	}
	al_draw_scaled_bitmap(game->map.arrow, 0, 0, al_get_bitmap_width(game->map.arrow), al_get_bitmap_height(game->map.arrow), game->viewportWidth*x, game->viewportHeight*y + ((sin(game->map.arrowpos)+0.5)/20.0)*game->viewportHeight, game->viewportWidth*0.1, game->viewportHeight*0.16, 0);
	game->map.arrowpos += tps(game, 0.1*60);
}

void Map_Load(struct Game *game) {
	al_play_sample_instance(game->map.music);
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->map.map,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	Map_Draw(game);
}

int Map_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((((game->map.selected<4) || (game->map.selected==6)) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((game->map.selected>4) && (game->map.selected!=6) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((game->map.selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_UP)) || ((game->map.selected==6) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN))) {
		game->map.selected--;
		al_play_sample_instance(game->map.click);
	} else if (((game->map.selected<3) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((game->map.selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((game->map.selected==3) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) || ((game->map.selected==5) && (ev->keyboard.keycode==ALLEGRO_KEY_UP))) {
		game->map.selected++;
		al_play_sample_instance(game->map.click);
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_LEFT) || (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) || (ev->keyboard.keycode==ALLEGRO_KEY_UP) || (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) {
		al_play_sample_instance(game->map.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		al_play_sample_instance(game->map.click);
		game->level.current_level = game->map.selected;
		PrintConsole(game, "Selecting level %d...", game->map.selected);
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_LEVEL;
		return 0;
	} else if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->loadstate = GAMESTATE_MENU;
		LoadGameState(game);
		return 0;
	} else { return 0; }
	if (game->map.selected<1) game->map.selected=1;
	if (game->map.selected>game->map.available) game->map.selected=game->map.available;
	return 0;
}

void Map_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(7);

	game->map.available = atoi(GetConfigOptionDefault("MuffinAttack", "level", "1"));
	if ((game->map.available<1) || (game->map.available>6)) game->map.available=1;
	game->map.selected = game->map.available;
	PrintConsole(game, "Last level available: %d", game->map.selected);
	game->map.arrowpos = 0;

	game->map.map_bg = LoadScaledBitmap("map/background.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	char filename[30] = { };
	sprintf(filename, "map/highlight%d.png", game->map.available);
	game->map.highlight = LoadScaledBitmap(filename, game->viewportWidth, game->viewportHeight);
	PROGRESS;

	game->map.arrow = al_load_bitmap( GetDataFilePath("map/arrow.png") );
	PROGRESS;

	game->map.click_sample = al_load_sample( GetDataFilePath("menu/click.flac") );
	PROGRESS;
	game->map.sample = al_load_sample( GetDataFilePath("map/map.flac") );
	PROGRESS;

	game->map.music = al_create_sample_instance(game->map.sample);
	al_attach_sample_instance_to_mixer(game->map.music, game->audio.music);
	al_set_sample_instance_playmode(game->map.music, ALLEGRO_PLAYMODE_LOOP);

	game->map.click = al_create_sample_instance(game->map.click_sample);
	al_attach_sample_instance_to_mixer(game->map.click, game->audio.fx);
	al_set_sample_instance_playmode(game->map.click, ALLEGRO_PLAYMODE_ONCE);

	if (!game->map.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	if (!game->map.click_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	game->map.map = LoadScaledBitmap("table.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	al_set_target_bitmap(game->map.map);
	al_draw_bitmap(game->map.map_bg, 0, 0 ,0);
	al_draw_bitmap(game->map.highlight, 0, 0 ,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	PROGRESS;
}

void Map_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=255; fadeloop>=0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->map.map, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->map.map);
	al_destroy_bitmap(game->map.map_bg);
	al_destroy_bitmap(game->map.highlight);
	al_destroy_bitmap(game->map.arrow);
	al_destroy_sample_instance(game->map.music);
	al_destroy_sample(game->map.sample);
	al_destroy_sample_instance(game->map.click);
	al_destroy_sample(game->map.click_sample);
}
