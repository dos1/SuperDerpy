/*! \file menu.c
 *  \brief Main Menu view.
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
#include <allegro5/allegro_ttf.h>
#include "../config.h"
#include "../utils.h"
#include "menu.h"

int Gamestate_ProgressCount = 24;

void DrawMenuState(struct Game *game, struct MenuResources *data) {
	ALLEGRO_FONT *font;
	char* text = malloc(255*sizeof(char));
	struct ALLEGRO_COLOR color;
	switch (data->menustate) {
		case MENUSTATE_MAIN:
			font = data->font; if (data->selected==0) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Start game");
			font = data->font; if (data->selected==1) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Options");
			font = data->font; if (data->selected==2) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "About");
			font = data->font; if (data->selected==3) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		case MENUSTATE_OPTIONS:
			font = data->font; if (data->selected==0) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Control settings");
			font = data->font; if (data->selected==1) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Video settings");
			font = data->font; if (data->selected==2) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "Audio settings");
			font = data->font; if (data->selected==3) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_AUDIO:
			font = data->font; if (data->selected==0) font = data->font_selected;
			if (game->config.music) sprintf(text, "Music volume: %d0%%", game->config.music);
			else sprintf(text, "Music disabled");
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = data->font; if (data->selected==1) font = data->font_selected;
			if (game->config.fx) sprintf(text, "Effects volume: %d0%%", game->config.fx);
			else sprintf(text, "Effects disabled");
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			font = data->font; if (data->selected==2) font = data->font_selected;
			if (game->config.voice) sprintf(text, "Voice volume: %d0%%", game->config.voice);
			else sprintf(text, "Voice disabled");
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, text);
			font = data->font; if (data->selected==3) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_VIDEO:
			if (data->options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
				color = al_map_rgba(0,0,0,128);
			}
			else {
				sprintf(text, "Fullscreen: no");
				color = al_map_rgba(255,255,255,255);
			}
			font = data->font; if (data->selected==0) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = data->font; if (data->selected==1) font = data->font_selected;
			DrawTextWithShadow(font, color, game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Resolution: 800x500");
			font = data->font; if (data->selected==2) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "FPS: 60");
			font = data->font; if (data->selected==3) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_PAUSE:
			font = data->font; if (data->selected==0) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Resume game");
			font = data->font; if (data->selected==1) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Return to map");
			font = data->font; if (data->selected==2) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "Options");
			font = data->font; if (data->selected==3) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		default:
			data->selected=0;
			font = data->font; if (data->selected==0) font = data->font_selected;
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
}

void Gamestate_Draw(struct Game *game, struct MenuResources* data) {

	al_set_target_bitmap(data->pinkcloud_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float x = 1.5;
	int minus;
	if (data->cloud_position>0) minus=1; else minus=-1;
	al_draw_bitmap(data->rain_bitmap, fmod(minus*data->cloud_position,3)*x*5+al_get_bitmap_width(data->pinkcloud_bitmap)/2.7, al_get_bitmap_height(data->pinkcloud_bitmap)*(0.88+(fmod(-1.8*(data->cloud_position+80), 6))/20.0), 0);
	al_draw_bitmap(data->rain_bitmap, fmod(minus*data->cloud_position,3)*x*3+al_get_bitmap_width(data->pinkcloud_bitmap)/3.1, al_get_bitmap_height(data->pinkcloud_bitmap)*(0.78+(fmod(-2.8*(data->cloud_position+80), 4))/18.0), 0);
	al_draw_scaled_bitmap(data->rain_bitmap, 0, 0, al_get_bitmap_width(data->rain_bitmap), al_get_bitmap_height(data->rain_bitmap), fmod(minus*data->cloud_position,3)*x*6+al_get_bitmap_width(data->pinkcloud_bitmap)/2.1, al_get_bitmap_height(data->pinkcloud_bitmap)*(0.87+(fmod(-4.9*(data->cloud_position+80), 8))/26.0), al_get_bitmap_width(data->pinkcloud_bitmap)*0.4, al_get_bitmap_height(data->pinkcloud_bitmap)*0.08, 0);
	al_draw_bitmap(data->pinkcloud, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(183,234,193));
	float tint = (sin((data->cloud_position-80)/15)+1)/2;
	if (tint < 0.000004) { PrintConsole(game, "random tint %f", tint); data->mountain_position = (game->viewport.width*(rand()/(float)RAND_MAX)/2)+game->viewport.width/2; }
	al_draw_tinted_bitmap(data->mountain,al_map_rgba_f(tint,tint,tint,tint),data->mountain_position, 0,0);
	al_draw_scaled_bitmap(data->cloud,0,0,al_get_bitmap_width(data->cloud), al_get_bitmap_height(data->cloud), game->viewport.width*(sin((data->cloud_position/40)-4.5)-0.3), game->viewport.height*0.35, al_get_bitmap_width(data->cloud)/2, al_get_bitmap_height(data->cloud)/2,0);
	al_draw_bitmap(data->cloud2,game->viewport.width*(data->cloud2_position/100.0), game->viewport.height-(game->viewport.width*(1240.0/3910.0))*0.7,0);
	al_draw_bitmap(data->image,0, game->viewport.height-(game->viewport.width*(1240.0/3910.0)),0);

	al_draw_bitmap(data->pinkcloud_bitmap,(game->viewport.width*0.12) + (cos((data->cloud_position/25+80)*1.74444))*40, 0,0);
	al_draw_bitmap(data->cloud,game->viewport.width*data->cloud_position/100, game->viewport.height*0.1,0);

	al_draw_bitmap(data->pie_bitmap, game->viewport.width/2, game->viewport.height*(data->cloud_position)/10,0);

	/* GLASS EFFECT */
	al_set_target_bitmap(data->blurbg);

	al_clear_to_color(al_map_rgb(183,234,193));
	al_draw_scaled_bitmap(data->cloud,0,0,al_get_bitmap_width(data->cloud), al_get_bitmap_height(data->cloud), game->viewport.width*(sin((data->cloud_position/40)-4.5)-0.3) - (game->viewport.width/2)+(al_get_bitmap_width(data->logo)/2), game->viewport.height*0.35-(game->viewport.height*0.1), al_get_bitmap_width(data->cloud)/2, al_get_bitmap_height(data->cloud)/2,0);
	al_draw_bitmap(data->pinkcloud_bitmap,(game->viewport.width*0.12) + (cos((data->cloud_position/25+80)*1.74444))*40 - (game->viewport.width/2)+(al_get_bitmap_width(data->logo)/2), -(game->viewport.height*0.1),0);
	al_draw_bitmap(data->cloud,game->viewport.width*data->cloud_position/100 - (game->viewport.width/2)+(al_get_bitmap_width(data->logo)/2), game->viewport.height*0.1-(game->viewport.height*0.1),0);
	al_draw_bitmap(data->pie_bitmap, game->viewport.width/2 - (game->viewport.width/2)+(al_get_bitmap_width(data->logo)/2), game->viewport.height*(data->cloud_position)/10 -(game->viewport.height*0.1),0);

	/*al_draw_bitmap_region(al_get_backbuffer(game->display), (game->viewport.width/2)-(al_get_bitmap_width(data->logo)/2), (game->viewport.height*0.1), al_get_bitmap_width(data->logo), al_get_bitmap_height(data->logo), 0, 0, 0);*/

	al_set_target_bitmap(data->blurbg2);
	al_clear_to_color(al_map_rgba(0,0,0,0));

	float alpha = (1.0/8.0);
	ALLEGRO_COLOR color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	int bx = 0, by = 0;
	for (by = -2; by <= 2; by++) {
		for (bx = -2; bx <= 2; bx++) {
			if (sqrt(bx*bx+by*by) <= 2)
				al_draw_tinted_bitmap(data->blurbg, color, bx*2, by*2, 0);
		}
	}
	al_draw_bitmap(data->glass, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_ALPHA);
	al_draw_bitmap(data->logo, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_draw_bitmap(data->blurbg2, (game->viewport.width/2)-(al_get_bitmap_width(data->logo)/2), (game->viewport.height*0.1), 0);

	al_draw_bitmap(data->logoblur, (game->viewport.width/2)-(al_get_bitmap_width(data->logo)/2)-2, (game->viewport.height*0.1)-2, 0);
	al_draw_tinted_bitmap(data->logo, al_map_rgba_f(0.1, 0.1, 0.1, 0.1), (game->viewport.width/2)-(al_get_bitmap_width(data->logo)/2), (game->viewport.height*0.1), 0);
	/* END OF GLASS EFFECT */

	DrawTextWithShadow(data->font_title, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	DrawTextWithShadow(data->font_subtitle, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.275, ALLEGRO_ALIGN_CENTRE, "Muffin Attack");

	DrawMenuState(game, data);
}

void Gamestate_Logic(struct Game *game, struct MenuResources* data) {
	data->cloud_position-=0.1;
	data->cloud2_position-=0.025;
	if (data->cloud_position<-80) { data->cloud_position=100; PrintConsole(game, "cloud_position"); }
	if (data->cloud2_position<0) { data->cloud2_position=100; PrintConsole(game, "cloud2_position"); }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {

	struct MenuResources *data = malloc(sizeof(struct MenuResources));

	data->options.fullscreen = game->config.fullscreen;
	data->options.fps = game->config.fps;
	data->options.width = game->config.width;
	data->options.height = game->config.height;
	(*progress)(game);
	data->image = LoadScaledBitmap(game,  "menu/menu.png", game->viewport.width, game->viewport.width*(1240.0/3910.0));
	(*progress)(game);
	data->mountain = LoadScaledBitmap(game,  "menu/mountain.png", game->viewport.height*1.6*0.055, game->viewport.height/9 );
	(*progress)(game);
	data->cloud = LoadScaledBitmap(game,  "menu/cloud.png", game->viewport.height*1.6*0.5, game->viewport.height*0.25 );
	(*progress)(game);
	data->cloud2 = LoadScaledBitmap(game,  "menu/cloud2.png", game->viewport.height*1.6*0.2, game->viewport.height*0.1 );
	(*progress)(game);
	data->logo = LoadScaledBitmap(game,  "menu/logo.png", game->viewport.height*1.6*0.3, game->viewport.height*0.35 );
	data->blurbg = al_create_bitmap(game->viewport.height*1.6*0.3, game->viewport.height*0.35);
	data->blurbg2 = al_create_bitmap(game->viewport.height*1.6*0.3, game->viewport.height*0.35);
	data->logoblur = al_create_bitmap(game->viewport.height*1.6*0.3+4, game->viewport.height*0.35+4);
	(*progress)(game);
	al_set_target_bitmap(data->logoblur);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float alpha = (1.0/40.0);
	ALLEGRO_COLOR color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	int by, bx;
	for (by = -2; by <= 2; by++) {
		for (bx = -2; bx <= 2; bx++) {
			if (sqrt(bx*bx+by*by) <= 2)
				al_draw_tinted_bitmap(data->logo, color, bx, by, 0);
		}
	}
	(*progress)(game);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	data->glass = LoadScaledBitmap(game,  "menu/glass.png", game->viewport.height*1.6*0.3, game->viewport.height*0.35 );
	(*progress)(game);
	//data->pinkcloud = LoadScaledBitmap(game,  "menu/pinkcloud.png", game->viewport.width*0.33125, game->viewport.height*0.8122);
	data->pinkcloud = LoadScaledBitmap(game,  "menu/pinkcloud.png", game->viewport.height*0.8122*(1171.0/2218.0), game->viewport.height*0.8122);
	(*progress)(game);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	data->rain = al_load_bitmap( GetDataFilePath(game, "menu/rain.png") );
	(*progress)(game);
	data->pie = al_load_bitmap( GetDataFilePath(game, "menu/pie.png") );
	(*progress)(game);
	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

	data->sample = al_load_sample( GetDataFilePath(game, "menu/menu.flac") );
	(*progress)(game);
	data->rain_sample = al_load_sample( GetDataFilePath(game, "menu/rain.flac") );
	(*progress)(game);
	data->click_sample = al_load_sample( GetDataFilePath(game, "menu/click.flac") );
	(*progress)(game);
	data->mountain_position = game->viewport.width*0.7;
	(*progress)(game);

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);
	(*progress)(game);

	data->rain_sound = al_create_sample_instance(data->rain_sample);
	al_attach_sample_instance_to_mixer(data->rain_sound, game->audio.fx);
	al_set_sample_instance_playmode(data->rain_sound, ALLEGRO_PLAYMODE_LOOP);
	(*progress)(game);

	data->click = al_create_sample_instance(data->click_sample);
	al_attach_sample_instance_to_mixer(data->click, game->audio.fx);
	al_set_sample_instance_playmode(data->click, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	data->font_title = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.16,0 );
	(*progress)(game);
	data->font_subtitle = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.08,0 );
	(*progress)(game);
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.05,0 );
	(*progress)(game);
	data->font_selected = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.065,0 );
	(*progress)(game);

	if (!data->sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	if (!data->rain_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	if (!data->click_sample){
		fprintf(stderr, "Audio clip sample#3 not loaded!\n" );
		exit(-1);
	}

	data->pinkcloud_bitmap = al_create_bitmap(game->viewport.height*0.8122*(1171.0/2218.0), game->viewport.height);

	data->pie_bitmap = al_create_bitmap(game->viewport.height*0.8, game->viewport.height);
	al_set_target_bitmap(data->pie_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(data->pie, 0, 0, al_get_bitmap_width(data->pie), al_get_bitmap_height(data->pie), al_get_bitmap_width(data->pie_bitmap)*0.5, 0, game->viewport.height*1.6*0.11875, game->viewport.height*0.0825, 0);
	al_draw_scaled_bitmap(data->pie, 0, 0, al_get_bitmap_width(data->pie), al_get_bitmap_height(data->pie), al_get_bitmap_width(data->pie_bitmap)*0.1, al_get_bitmap_height(data->pie_bitmap)*0.3, game->viewport.height*1.6*0.09, game->viewport.height*0.06, ALLEGRO_FLIP_HORIZONTAL);
	al_draw_scaled_bitmap(data->pie, 0, 0, al_get_bitmap_width(data->pie), al_get_bitmap_height(data->pie), al_get_bitmap_width(data->pie_bitmap)*0.3, al_get_bitmap_height(data->pie_bitmap)*0.6, game->viewport.height*1.6*0.13, game->viewport.height*0.1, 0);
	al_destroy_bitmap(data->pie);
	(*progress)(game);

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	data->rain_bitmap = al_create_bitmap(al_get_bitmap_width(data->pinkcloud_bitmap)*0.5, al_get_bitmap_height(data->pinkcloud_bitmap)*0.1);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al_set_target_bitmap(data->rain_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(data->rain,0, 0, al_get_bitmap_width(data->rain), al_get_bitmap_height(data->rain), 0, 0, al_get_bitmap_width(data->rain_bitmap), al_get_bitmap_height(data->rain_bitmap),0);
	al_destroy_bitmap(data->rain);
	(*progress)(game);

	al_set_target_backbuffer(game->display);
	return data;
}

void Gamestate_Stop(struct Game *game, struct MenuResources* data) {
	FadeGamestate(game, false);
	al_stop_sample_instance(data->music);
	al_stop_sample_instance(data->rain_sound);
}

void ChangeMenuState(struct Game *game, struct MenuResources* data, enum menustate_enum state) {
	data->menustate=state;
	data->selected=0;
	PrintConsole(game, "menu state changed %d", state);
}

void Gamestate_Unload(struct Game *game, struct MenuResources* data) {
	al_destroy_bitmap(data->pinkcloud);
	al_destroy_bitmap(data->image);
	al_destroy_bitmap(data->cloud);
	al_destroy_bitmap(data->cloud2);
	al_destroy_bitmap(data->pinkcloud_bitmap);
	al_destroy_bitmap(data->rain_bitmap);
	al_destroy_bitmap(data->mountain);
	al_destroy_bitmap(data->pie_bitmap);
	al_destroy_bitmap(data->logo);
	al_destroy_bitmap(data->logoblur);
	al_destroy_bitmap(data->glass);
	al_destroy_bitmap(data->blurbg);
	al_destroy_bitmap(data->blurbg2);
	al_destroy_font(data->font_title);
	al_destroy_font(data->font_subtitle);
	al_destroy_font(data->font);
	al_destroy_font(data->font_selected);
	al_destroy_sample_instance(data->music);
	al_destroy_sample_instance(data->rain_sound);
	al_destroy_sample_instance(data->click);
	al_destroy_sample(data->sample);
	al_destroy_sample(data->rain_sample);
	al_destroy_sample(data->click_sample);
}

void Gamestate_Start(struct Game *game, struct MenuResources* data) {

	data->cloud_position = 100;
	data->cloud2_position = 100;
	ChangeMenuState(game,data,MENUSTATE_MAIN);

	al_play_sample_instance(data->music);
	al_play_sample_instance(data->rain_sound);
	FadeGamestate(game, true);
}

void Gamestate_ProcessEvent(struct Game *game, struct MenuResources* data, ALLEGRO_EVENT *ev) {

	if (ev->type != ALLEGRO_EVENT_KEY_DOWN) return;

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		data->selected--;
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected--;
		al_play_sample_instance(data->click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		data->selected++;
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected++;
		al_play_sample_instance(data->click);
	}

	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		char *text;
		al_play_sample_instance(data->click);
		switch (data->menustate) {
			case MENUSTATE_MAIN:
				switch (data->selected) {
					case 0:
						LoadGamestate(game, "disclaimer");
						LoadGamestate(game, "intro");
						LoadGamestate(game, "map");
						StopGamestate(game, "menu");
						StartGamestate(game, "disclaimer");
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					case 2:
						StopGamestate(game, "menu");
						LoadGamestate(game, "about");
						StartGamestate(game, "about");
						break;
					case 3:
						UnloadGamestate(game, "menu");
						break;
				}
				break;
			case MENUSTATE_AUDIO:
				text = malloc(255*sizeof(char));
				switch (data->selected) {
					case 0:
						game->config.music--;
						if (game->config.music<0) game->config.music=10;
						sprintf(text, "%d", game->config.music);
						SetConfigOption(game, "SuperDerpy", "music", text);
						al_set_mixer_gain(game->audio.music, game->config.music/10.0);
						break;
					case 1:
						game->config.fx--;
						if (game->config.fx<0) game->config.fx=10;
						sprintf(text, "%d", game->config.fx);
						SetConfigOption(game, "SuperDerpy", "fx", text);
						al_set_mixer_gain(game->audio.fx, game->config.fx/10.0);
						break;
					case 2:
						game->config.voice--;
						if (game->config.voice<0) game->config.voice=10;
						sprintf(text, "%d", game->config.voice);
						SetConfigOption(game, "SuperDerpy", "voice", text);
						al_set_mixer_gain(game->audio.voice, game->config.voice/10.0);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
				}
				free(text);
				break;
			case MENUSTATE_OPTIONS:
				switch (data->selected) {
					case 0:
						ChangeMenuState(game,data,MENUSTATE_CONTROLS);
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_VIDEO);
						break;
					case 2:
						ChangeMenuState(game,data,MENUSTATE_AUDIO);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_MAIN);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_PAUSE: //TODO: REMOVE ME
				switch (data->selected){
					case 0:
						PrintConsole(game,"Game resumed.");
						//al_destroy_bitmap(game->pause.bitmap);
						//game->pause.bitmap = NULL;

						//ResumeGamestate(game);
						//game->gamestate = game->loadstate;
						break;
					case 1:
						// TODO:
						SwitchGamestate(game, "pause", "map");
						/*game->gamestate=game->loadstate;
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_MAP;*/
						break;
					case 2:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					case 3:
						UnloadGamestate(game, "menu");
						return;
					default:
						break;
				}
				break;
			case MENUSTATE_CONTROLS:
				switch (data->selected) {
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_VIDEO:
				switch (data->selected) {
					case 0:
						data->options.fullscreen = !data->options.fullscreen;
						if (data->options.fullscreen)
							SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
						break;
					case 3:
						if ((data->options.fullscreen==game->config.fullscreen) && (data->options.fps==game->config.fps) && (data->options.width==game->config.width) && (data->options.height==game->config.height)) {
							ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						} else {
							PrintConsole(game, "video settings changed, restarting...");
							game->restart = true;
							UnloadGamestate(game, "menu");
							return;
						}
						break;
					default:
						break;
				}
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
				break;
		}
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		switch (data->menustate) {
			case MENUSTATE_OPTIONS:
				ChangeMenuState(game,data,MENUSTATE_MAIN);
				break;
			case MENUSTATE_VIDEO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_AUDIO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_CONTROLS:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_PAUSE:
				PrintConsole(game,"Game resumed.");
				//al_destroy_bitmap(game->pause.bitmap);
				//game->pause.bitmap = NULL;
				//ResumeGameState(game);
				//game->gamestate = game->loadstate;
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
		}
	}

	if (data->selected==-1) data->selected=3;
	if (data->selected==4) data->selected=0;
	return;
}


void Gamestate_Pause(struct Game *game, struct MenuResources* data) {}
void Gamestate_Resume(struct Game *game, struct MenuResources* data) {}
void Gamestate_Reload(struct Game *game, struct MenuResources* data) {}
