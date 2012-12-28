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
#include "../utils.h"
#include "../config.h"
#include "map.h"

int Gamestate_ProgressCount = 10;

void Gamestate_Draw(struct Game *game, struct MapResources* data) {
	al_draw_bitmap(data->map, 0, 0, 0);
	float x=0,y=0;
	switch (data->selected) {
		case 1:
			x=0.2;
			y=0.25;
			break;
		case 2:
			x=0.495;
			y=0.35;
			break;
		case 3:
			x=0.72;
			y=0.4;
			break;
		case 4:
			x=0.7;
			y=0.77;
			break;
		case 5:
			x=0.248;
			y=0.75;
			break;
		case 6:
			x=0.425;
			y=0.675;
			break;
	}
	al_draw_scaled_bitmap(data->arrow, 0, 0, al_get_bitmap_width(data->arrow), al_get_bitmap_height(data->arrow), (game->viewport.width-game->viewport.height*1.6)/2+game->viewport.height*1.6*x, game->viewport.height*y + ((sin(data->arrowpos)+0.5)/20.0)*game->viewport.height, game->viewport.height*1.6*0.1, game->viewport.height*0.16, 0);
}

void Gamestate_Logic(struct Game *game, struct MapResources* data) {
	data->arrowpos += 0.1;
}

void Gamestate_Start(struct Game *game, struct MapResources* data) {
	al_play_sample_instance(data->music);
	FadeGamestate(game, true);
}

void Gamestate_ProcessEvent(struct Game *game, struct MapResources* data, ALLEGRO_EVENT *ev) {
	if (ev->type!=ALLEGRO_EVENT_KEY_DOWN) return;
	if ((((data->selected<4) || (data->selected==6)) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((data->selected>4) && (data->selected!=6) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((data->selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_UP)) || ((data->selected==6) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN))) {
		data->selected--;
		al_play_sample_instance(data->click);
	} else if (((data->selected<3) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((data->selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((data->selected==3) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) || ((data->selected==5) && (ev->keyboard.keycode==ALLEGRO_KEY_UP))) {
		data->selected++;
		al_play_sample_instance(data->click);
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_LEFT) || (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) || (ev->keyboard.keycode==ALLEGRO_KEY_UP) || (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) {
		al_play_sample_instance(data->click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		al_play_sample_instance(data->click);
		//game->level.input.current_level = data->selected;
		PrintConsole(game, "Selecting level %d...", data->selected);
		char gamestate[7] = {};
		sprintf(gamestate, "level%d", data->selected);
		SwitchGamestate(game, "map", gamestate);
		return;
	} else if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		SwitchGamestate(game, "map", "menu");
		return;
	}
	if (data->selected<1) data->selected=1;
	if (data->selected>data->available) data->selected=data->available;
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {

	struct MapResources *data = malloc(sizeof(struct MapResources));
	data->available = atoi(GetConfigOptionDefault(game, "MuffinAttack", "level", "1"));
	if ((data->available<1) || (data->available>6)) data->available=1;
	data->selected = data->available;
	PrintConsole(game, "Last level available: %d", data->selected);
	data->arrowpos = 0;
	(*progress)(game);

	data->map_bg = LoadScaledBitmap(game, "map/background.png", game->viewport.height*1.6, game->viewport.height);
	(*progress)(game);
	char filename[30] = { };
	sprintf(filename, "map/highlight%d.png", data->available);
	data->highlight = LoadScaledBitmap(game, filename, game->viewport.height*1.6, game->viewport.height);
	(*progress)(game);

	data->arrow = al_load_bitmap( GetDataFilePath(game, "map/arrow.png") );
	(*progress)(game);

	data->click_sample = al_load_sample( GetDataFilePath(game, "menu/click.flac") );
	(*progress)(game);
	data->sample = al_load_sample( GetDataFilePath(game, "map/map.flac") );
	(*progress)(game);

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);
	(*progress)(game);

	data->click = al_create_sample_instance(data->click_sample);
	al_attach_sample_instance_to_mixer(data->click, game->audio.fx);
	al_set_sample_instance_playmode(data->click, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	if (!data->sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	if (!data->click_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	data->map = LoadScaledBitmap(game, "table.png", game->viewport.width, game->viewport.height);
	(*progress)(game);
	al_set_target_bitmap(data->map);
	al_draw_bitmap(data->map_bg, (game->viewport.width-game->viewport.height*1.6)/2, 0 ,0);
	al_draw_bitmap(data->highlight, (game->viewport.width-game->viewport.height*1.6)/2, 0 ,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	(*progress)(game);

	return data;
}

void Gamestate_Unload(struct Game *game, struct MapResources* data) {
	al_destroy_bitmap(data->map);
	al_destroy_bitmap(data->map_bg);
	al_destroy_bitmap(data->highlight);
	al_destroy_bitmap(data->arrow);
	al_destroy_sample_instance(data->music);
	al_destroy_sample(data->sample);
	al_destroy_sample_instance(data->click);
	al_destroy_sample(data->click_sample);
}

void Gamestate_Stop(struct Game *game, struct MapResources* data) {
	FadeGamestate(game, false);
	al_stop_sample_instance(data->music);
}

void Gamestate_Pause(struct Game *game, struct MapResources* data) {}
void Gamestate_Resume(struct Game *game, struct MapResources* data) {}
void Gamestate_Reload(struct Game *game, struct MapResources* data) {}
