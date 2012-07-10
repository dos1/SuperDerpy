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
#include "config.h"
#include "menu.h"

void DrawMenuState(struct Game *game) {
	ALLEGRO_FONT *font;
	char* text = malloc(255*sizeof(char));
	struct ALLEGRO_COLOR color;
	switch (game->menu.menustate) {
		case MENUSTATE_MAIN:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, "Start game");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, "Options");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, "About");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");	
			break;
		case MENUSTATE_OPTIONS:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, "Control settings");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, "Video settings");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, "Audio settings");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_AUDIO:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			if (game->music) sprintf(text, "Music volume: %d0%%", game->music);
			else sprintf(text, "Music disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			if (game->fx) sprintf(text, "Effects volume: %d0%%", game->fx);
			else sprintf(text, "Effects disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			if (game->voice) sprintf(text, "Voice volume: %d0%%", game->voice);
			else sprintf(text, "Voice disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_VIDEO:
			if (game->menu.options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
				color = al_map_rgba(0,0,0,128);
			}
			else {
				sprintf(text, "Fullscreen: no");
				color = al_map_rgba(255,255,255,255);
			}
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, color, al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, "Resolution: 800x500");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, "FPS: 60");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_PAUSE:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, "Resume game");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, "Return to map");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, "Options");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		default:
			game->menu.selected=0;
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
}

void Menu_Draw(struct Game *game) {
	if (!game->menu.loaded) {
		game->gamestate=GAMESTATE_LOADING;
		game->loadstate=GAMESTATE_MENU;
		return;
	}

	al_set_target_bitmap(game->menu.pinkcloud_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float x = 1.5;
	int minus;
	if (game->menu.cloud_position>0) minus=1; else minus=-1;
	al_draw_bitmap(game->menu.rain_bitmap, fmod(minus*game->menu.cloud_position,3)*x*5+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.7, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.88+(fmod(-1.8*(game->menu.cloud_position+80), 6))/20.0), 0);
	al_draw_bitmap(game->menu.rain_bitmap, fmod(minus*game->menu.cloud_position,3)*x*3+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/3.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.78+(fmod(-2.8*(game->menu.cloud_position+80), 4))/18.0), 0);
	al_draw_scaled_bitmap(game->menu.rain_bitmap, 0, 0, al_get_bitmap_width(game->menu.rain_bitmap), al_get_bitmap_height(game->menu.rain_bitmap), fmod(minus*game->menu.cloud_position,3)*x*6+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.87+(fmod(-4.9*(game->menu.cloud_position+80), 8))/26.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.4, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.08, 0);
	al_draw_bitmap(game->menu.pinkcloud, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(183,234,193));
	float tint = (sin((game->menu.cloud_position-80)/15)+1)/2;
	if (tint < 0.000004) { PrintConsole(game, "random tint %f", tint); game->menu.mountain_position = (al_get_display_width(game->display)*(rand()/(float)RAND_MAX)/2)+al_get_display_width(game->display)/2; }
	al_draw_tinted_bitmap(game->menu.mountain,al_map_rgba_f(tint,tint,tint,tint),game->menu.mountain_position, 0,0);
	al_draw_scaled_bitmap(game->menu.cloud,0,0,al_get_bitmap_width(game->menu.cloud), al_get_bitmap_height(game->menu.cloud), al_get_display_width(game->display)*(sin((game->menu.cloud_position/40)-4.5)-0.3), al_get_display_height(game->display)*0.35, al_get_bitmap_width(game->menu.cloud)/2, al_get_bitmap_height(game->menu.cloud)/2,0);
	al_draw_bitmap(game->menu.cloud2,al_get_display_width(game->display)*(game->menu.cloud2_position/100.0), al_get_display_height(game->display)/1.5,0);
	al_draw_bitmap(game->menu.image,0, al_get_display_height(game->display)*0.55,0);

	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.12) + (cos((game->menu.cloud_position/25+80)*1.74444))*40, 0,0);
	al_draw_bitmap(game->menu.cloud,al_get_display_width(game->display)*game->menu.cloud_position/100, al_get_display_height(game->display)*0.1,0);

	al_draw_bitmap(game->menu.pie_bitmap, al_get_display_width(game->display)/2, al_get_display_height(game->display)*(game->menu.cloud_position)/10,0);

	/* GLASS EFFECT */
	al_set_target_bitmap(game->menu.blurbg);

	al_clear_to_color(al_map_rgb(183,234,193));
	al_draw_scaled_bitmap(game->menu.cloud,0,0,al_get_bitmap_width(game->menu.cloud), al_get_bitmap_height(game->menu.cloud), al_get_display_width(game->display)*(sin((game->menu.cloud_position/40)-4.5)-0.3) - (al_get_display_width(game->display)/2)+(al_get_bitmap_width(game->menu.logo)/2), al_get_display_height(game->display)*0.35-(al_get_display_height(game->display)*0.1), al_get_bitmap_width(game->menu.cloud)/2, al_get_bitmap_height(game->menu.cloud)/2,0);
	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.12) + (cos((game->menu.cloud_position/25+80)*1.74444))*40 - (al_get_display_width(game->display)/2)+(al_get_bitmap_width(game->menu.logo)/2), -(al_get_display_height(game->display)*0.1),0);
	al_draw_bitmap(game->menu.cloud,al_get_display_width(game->display)*game->menu.cloud_position/100 - (al_get_display_width(game->display)/2)+(al_get_bitmap_width(game->menu.logo)/2), al_get_display_height(game->display)*0.1-(al_get_display_height(game->display)*0.1),0);
	al_draw_bitmap(game->menu.pie_bitmap, al_get_display_width(game->display)/2 - (al_get_display_width(game->display)/2)+(al_get_bitmap_width(game->menu.logo)/2), al_get_display_height(game->display)*(game->menu.cloud_position)/10 -(al_get_display_height(game->display)*0.1),0);

	/*al_draw_bitmap_region(al_get_backbuffer(game->display), (al_get_display_width(game->display)/2)-(al_get_bitmap_width(game->menu.logo)/2), (al_get_display_height(game->display)*0.1), al_get_bitmap_width(game->menu.logo), al_get_bitmap_height(game->menu.logo), 0, 0, 0);*/

	al_set_target_bitmap(game->menu.blurbg2);
	al_clear_to_color(al_map_rgba(0,0,0,0));

	float alpha = (1.0/8.0);
	ALLEGRO_COLOR color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	int bx = 0, by = 0;
	for (by = -2; by <= 2; by++) {
		for (bx = -2; bx <= 2; bx++) {
			if (sqrt(bx*bx+by*by) <= 2)
				al_draw_tinted_bitmap(game->menu.blurbg, color, bx*2, by*2, 0);
		}
	}
	al_draw_bitmap(game->menu.glass, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_ALPHA);
	al_draw_bitmap(game->menu.logo, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_draw_bitmap(game->menu.blurbg2, (al_get_display_width(game->display)/2)-(al_get_bitmap_width(game->menu.logo)/2), (al_get_display_height(game->display)*0.1), 0);

	al_draw_bitmap(game->menu.logoblur, (al_get_display_width(game->display)/2)-(al_get_bitmap_width(game->menu.logo)/2)-2, (al_get_display_height(game->display)*0.1)-2, 0);
	al_draw_tinted_bitmap(game->menu.logo, al_map_rgba_f(0.1, 0.1, 0.1, 0.1), (al_get_display_width(game->display)/2)-(al_get_bitmap_width(game->menu.logo)/2), (al_get_display_height(game->display)*0.1), 0);
	/* END OF GLASS EFFECT */

	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Muffin Attack");

	DrawMenuState(game);

	game->menu.cloud_position-=tps(game, 0.1*60);
	game->menu.cloud2_position-=tps(game, 0.025*60);
	if (game->menu.cloud_position<-80) { game->menu.cloud_position=100; PrintConsole(game, "cloud_position"); }
	if (game->menu.cloud2_position<0) { game->menu.cloud2_position=100; PrintConsole(game, "cloud2_position"); }
}

void Menu_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(16);

	game->menu.options.fullscreen = game->fullscreen;
	game->menu.options.fps = game->fps;
	game->menu.options.width = game->width;
	game->menu.options.height = game->height;
	game->menu.loaded = true;
	game->menu.image = LoadScaledBitmap( "menu/menu.png", al_get_display_width(game->display), al_get_display_height(game->display)*0.45);
	PROGRESS;
	game->menu.mountain = LoadScaledBitmap( "menu/mountain.png", al_get_display_width(game->display)*0.055, al_get_display_height(game->display)/9 );
	PROGRESS;
	game->menu.cloud = LoadScaledBitmap( "menu/cloud.png", al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.25 );
	PROGRESS;
	game->menu.cloud2 = LoadScaledBitmap( "menu/cloud2.png", al_get_display_width(game->display)*0.2, al_get_display_height(game->display)*0.1 );
	PROGRESS;
	game->menu.logo = LoadScaledBitmap( "menu/logo.png", al_get_display_width(game->display)*0.3, al_get_display_height(game->display)*0.35 );
	game->menu.blurbg = al_create_bitmap(al_get_display_width(game->display)*0.3, al_get_display_height(game->display)*0.35);
	game->menu.blurbg2 = al_create_bitmap(al_get_display_width(game->display)*0.3, al_get_display_height(game->display)*0.35);
	PROGRESS;
	game->menu.logoblur = al_create_bitmap(al_get_display_width(game->display)*0.3+4, al_get_display_height(game->display)*0.35+4);
	al_set_target_bitmap(game->menu.logoblur);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float alpha = (1.0/40.0);
	ALLEGRO_COLOR color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	int by, bx;
	for (by = -2; by <= 2; by++) {
		for (bx = -2; bx <= 2; bx++) {
			if (sqrt(bx*bx+by*by) <= 2)
				al_draw_tinted_bitmap(game->menu.logo, color, bx, by, 0);
		}
	}
	al_set_target_bitmap(al_get_backbuffer(game->display));
	PROGRESS;
	game->menu.glass = LoadScaledBitmap( "menu/glass.png", al_get_display_width(game->display)*0.3, al_get_display_height(game->display)*0.35 );
	PROGRESS;
	game->menu.pinkcloud = LoadScaledBitmap( "menu/pinkcloud.png", al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)*0.8122);
	PROGRESS;
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	game->menu.rain = al_load_bitmap( "data/menu/rain.png" );
	PROGRESS;
	game->menu.pie = al_load_bitmap( "data/levels/pie1.png" );
	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);
	PROGRESS;

	game->menu.sample = al_load_sample( "data/menu/menu.flac" );
	PROGRESS;
	game->menu.rain_sample = al_load_sample( "data/menu/rain.flac" );
	PROGRESS;
	game->menu.click_sample = al_load_sample( "data/menu/click.flac" );
	PROGRESS;
	game->menu.mountain_position = al_get_display_width(game->display)*0.7;

	game->menu.music = al_create_sample_instance(game->menu.sample);
	al_attach_sample_instance_to_mixer(game->menu.music, game->audio.music);
	al_set_sample_instance_playmode(game->menu.music, ALLEGRO_PLAYMODE_LOOP);

	game->menu.rain_sound = al_create_sample_instance(game->menu.rain_sample);
	al_attach_sample_instance_to_mixer(game->menu.rain_sound, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.rain_sound, ALLEGRO_PLAYMODE_LOOP);

	game->menu.click = al_create_sample_instance(game->menu.click_sample);
	al_attach_sample_instance_to_mixer(game->menu.click, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.click, ALLEGRO_PLAYMODE_ONCE);

	game->menu.font_title = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.16,0 );
	game->menu.font_subtitle = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.08,0 );
	game->menu.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.05,0 );
	game->menu.font_selected = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.065,0 );
	PROGRESS;

	if (!game->menu.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.rain_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.click_sample){
		fprintf(stderr, "Audio clip sample#3 not loaded!\n" );
		exit(-1);
	}

	game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display));

	game->menu.pie_bitmap = al_create_bitmap(al_get_display_width(game->display)/2, al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.pie_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.5, 0, al_get_display_width(game->display)*0.11875, al_get_display_height(game->display)*0.0825, 0);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.1, al_get_bitmap_height(game->menu.pie_bitmap)*0.3, al_get_display_width(game->display)*0.09, al_get_display_height(game->display)*0.06, ALLEGRO_FLIP_HORIZONTAL);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.3, al_get_bitmap_height(game->menu.pie_bitmap)*0.6, al_get_display_width(game->display)*0.13, al_get_display_height(game->display)*0.1, 0);
	al_destroy_bitmap(game->menu.pie);
	PROGRESS;

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	game->menu.rain_bitmap = al_create_bitmap(al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.5, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.1);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al_set_target_bitmap(game->menu.rain_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.rain,0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), 0, 0, al_get_bitmap_width(game->menu.rain_bitmap), al_get_bitmap_height(game->menu.rain_bitmap),0);
	al_destroy_bitmap(game->menu.rain);
	PROGRESS;
}

void Menu_Stop(struct Game* game) {
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		Menu_Draw(game);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_stop_sample_instance(game->menu.music);
	al_stop_sample_instance(game->menu.rain_sound);
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
}

void ChangeMenuState(struct Game *game, enum menustate_enum state) {
	game->menu.menustate=state;
	game->menu.selected=0;
	PrintConsole(game, "menu state changed %d", state);
}

void Menu_Unload(struct Game *game) {
	if (!game->menu.loaded) return;
	if (game->gamestate==GAMESTATE_MENU) Menu_Stop(game);
	al_destroy_bitmap(game->menu.pinkcloud);
	al_destroy_bitmap(game->menu.image);
	al_destroy_bitmap(game->menu.cloud);
	al_destroy_bitmap(game->menu.cloud2);
	al_destroy_bitmap(game->menu.pinkcloud_bitmap);
	al_destroy_bitmap(game->menu.rain_bitmap);
	al_destroy_bitmap(game->menu.mountain);
	al_destroy_bitmap(game->menu.pie_bitmap);
	al_destroy_bitmap(game->menu.logo);
	al_destroy_bitmap(game->menu.logoblur);
	al_destroy_bitmap(game->menu.glass);
	al_destroy_bitmap(game->menu.blurbg);
	al_destroy_bitmap(game->menu.blurbg2);
	al_destroy_font(game->menu.font_title);
	al_destroy_font(game->menu.font_subtitle);
	al_destroy_font(game->menu.font);
	al_destroy_font(game->menu.font_selected);
	al_destroy_sample_instance(game->menu.music);
	al_destroy_sample_instance(game->menu.rain_sound);
	al_destroy_sample_instance(game->menu.click);
	al_destroy_sample(game->menu.sample);
	al_destroy_sample(game->menu.rain_sample);
	al_destroy_sample(game->menu.click_sample);
	game->menu.loaded = false;
}

void Menu_Load(struct Game *game) {
	if (!game->menu.loaded) return;

	game->menu.cloud_position = 100;
	game->menu.cloud2_position = 100;
	ChangeMenuState(game,MENUSTATE_MAIN);

	al_play_sample_instance(game->menu.music);
	al_play_sample_instance(game->menu.rain_sound);
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	ALLEGRO_EVENT ev;
	float fadeloop;
	for(fadeloop=255; fadeloop>=0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		Menu_Draw(game);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
	Menu_Draw(game);
}

int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
		if ((game->menu.menustate==MENUSTATE_VIDEO) && (game->menu.selected==1) && (game->menu.options.fullscreen)) game->menu.selected--;
		al_play_sample_instance(game->menu.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
		if ((game->menu.menustate==MENUSTATE_VIDEO) && (game->menu.selected==1) && (game->menu.options.fullscreen)) game->menu.selected++;
		al_play_sample_instance(game->menu.click);
	}

	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		char *text;
		al_play_sample_instance(game->menu.click);
		switch (game->menu.menustate) {
			case MENUSTATE_MAIN:
				switch (game->menu.selected) {
					case 0:
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_DISCLAIMER;
						break;
					case 1:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					case 2:
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_ABOUT;
						break;
					case 3:
						return 1;
						break;
				}
				break;
			case MENUSTATE_AUDIO:
				text = malloc(255*sizeof(char));
				switch (game->menu.selected) {
					case 0:
						game->music--;
						if (game->music<0) game->music=10;
						sprintf(text, "%d", game->music);
						SetConfigOption("SuperDerpy", "music", text);
						al_set_mixer_gain(game->audio.music, game->music/10.0);
						break;
					case 1:
						game->fx--;
						if (game->fx<0) game->fx=10;
						sprintf(text, "%d", game->fx);
						SetConfigOption("SuperDerpy", "fx", text);
						al_set_mixer_gain(game->audio.fx, game->fx/10.0);
						break;
					case 2:
						game->voice--;
						if (game->voice<0) game->voice=10;
						sprintf(text, "%d", game->voice);
						SetConfigOption("SuperDerpy", "voice", text);
						al_set_mixer_gain(game->audio.voice, game->voice/10.0);
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
				}
				free(text);
				break;
			case MENUSTATE_OPTIONS:
				switch (game->menu.selected) {
					case 0:
						ChangeMenuState(game,MENUSTATE_CONTROLS);
						break;
					case 1:
						ChangeMenuState(game,MENUSTATE_VIDEO);
						break;
					case 2:
						ChangeMenuState(game,MENUSTATE_AUDIO);
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_MAIN);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_PAUSE:
				switch (game->menu.selected){
					case 0:
						PrintConsole(game,"Game resumed.");
						al_destroy_bitmap(game->pause.bitmap);
						game->pause.bitmap = NULL;
						game->gamestate = game->loadstate;
						break;
					case 1:
						game->gamestate=game->loadstate;
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_MAP;
						break;
					case 2:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					case 3:
						return 1;
					default:
						break;
				}
				break;
			case MENUSTATE_CONTROLS:
				switch (game->menu.selected) {
					case 3:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_VIDEO:
				switch (game->menu.selected) {
					case 0:
						game->menu.options.fullscreen = !game->menu.options.fullscreen;
						if (game->menu.options.fullscreen)
							SetConfigOption("SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption("SuperDerpy", "fullscreen", "0");
						break;
					case 3:
						if ((game->menu.options.fullscreen==game->fullscreen) && (game->menu.options.fps==game->fps) && (game->menu.options.width==game->width) && (game->menu.options.height==game->height)) {
							ChangeMenuState(game,MENUSTATE_OPTIONS);
						} else {
							PrintConsole(game, "video settings changed, restarting...");
							game->restart = true;
							return 1;
						}
						break;
					default:
						break;
				}
				break;
			default:
				return 1;
				break;
		}
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		switch (game->menu.menustate) {
			case MENUSTATE_OPTIONS:
				ChangeMenuState(game,MENUSTATE_MAIN);
				break;
			case MENUSTATE_VIDEO:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_AUDIO:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_CONTROLS:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_PAUSE:
				PrintConsole(game,"Game resumed.");
				al_destroy_bitmap(game->pause.bitmap);
				game->pause.bitmap = NULL;
				game->gamestate = game->loadstate;
				break;
			default:
				return 1;
				break;
		}
	}

	if (game->menu.selected==-1) game->menu.selected=3;
	if (game->menu.selected==4) game->menu.selected=0;
	return 0;
}
