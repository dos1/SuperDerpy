/*! \file intro.c
 *  \brief Intro view with plot description.
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
#include <math.h>
#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include "../utils.h"
#include "intro.h"

int Gamestate_ProgressCount = 11;

void AnimPage(struct Game *game, struct IntroResources *data, int page, ALLEGRO_COLOR tint) {
	int offset = 0;
	if (data->in_animation) offset = -1*game->viewport.width + (cos(((-1*((data->position)%game->viewport.width))/(float)game->viewport.width)*(ALLEGRO_PI))/2.0)*game->viewport.width + game->viewport.width/2.0;

	int amount1 = 2, amount2 = 2;
	float anim = data->anim;

	if (page==2) { amount1=4; amount2=3; }
	if (page==3) { amount1=3; amount2=3; }
	if (page==5) { amount1=5; amount2=3; anim*=2; }

	if (page<6) al_draw_tinted_bitmap_region(data->animsprites[page-1],tint,al_get_bitmap_width(data->animsprites[page-1])/amount1*(int)fmod(anim,amount1),al_get_bitmap_height(data->animsprites[page-1])/amount2*(((int)(anim/amount1))%amount2),al_get_bitmap_width(data->animsprites[page-1])/amount1, al_get_bitmap_height(data->animsprites[page-1])/amount2,offset+game->viewport.width*0.09, game->viewport.height*0.18,0);

	amount1 = 2;
	amount2 = 2;
	if (page==1) { amount1=4; amount2=3; }
	if (page==2) { amount1=3; amount2=3; }
	if (page==4) { amount1=5; amount2=3; anim*=2; }

	if (page<5) al_draw_tinted_bitmap_region(data->animsprites[page],tint,al_get_bitmap_width(data->animsprites[page])/amount1*(int)fmod(anim,amount1),al_get_bitmap_height(data->animsprites[page])/amount2*(((int)(anim/amount1))%amount2),al_get_bitmap_width(data->animsprites[page])/amount1, al_get_bitmap_height(data->animsprites[page])/amount2,offset+game->viewport.width*1.09, game->viewport.height*0.18,0);
}

void FillPage(struct Game *game, struct IntroResources *data, int page) {
	char filename[30] = { };
	snprintf(filename, 30, "intro/%d.flac", page);

	data->audiostream = al_load_audio_stream(GetDataFilePath(game, filename), 4, 1024);
	al_attach_audio_stream_to_mixer(data->audiostream, game->audio.voice);
	al_set_audio_stream_playing(data->audiostream, false);
	al_set_audio_stream_gain(data->audiostream, 1.75);

	al_set_target_bitmap(data->table);
	float y = 0.2;
	float oldx = -1;
	void draw_text(int page, char* text) {
		float x = 0.45;
		if (page!=oldx) { y=0.2; oldx=page; }
		DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width*x, game->viewport.height*y, ALLEGRO_ALIGN_LEFT, text);
		y+=0.07;
	}
	
	void DrawPage(int p) {
		switch (p) {
			case 1:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap(data->table_bitmap, 0, 0, 0);
				al_hold_bitmap_drawing(true);
				draw_text(1, "Ever since Twilight Sparkle and her");
				draw_text(1, "friends imprisoned Discord in stone,");
				draw_text(1, "Equestria had been peaceful for");
				draw_text(1, "a long time.");
				al_hold_bitmap_drawing(false);
				break;
			case 2:
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0, 0, 0);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0.5, 0, 0);
				al_hold_bitmap_drawing(false);
				al_hold_bitmap_drawing(true);
				draw_text(2, "Until one day a reckless pony caused");
				draw_text(2, "a tiny bit of chaos near Discord’s");
				draw_text(2, "statue.");
				al_hold_bitmap_drawing(false);
				break;
			case 3:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0, 0, 0);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0.5, 0, 0);
				al_hold_bitmap_drawing(false);
				al_hold_bitmap_drawing(true);
				draw_text(3, "This small amount of chaos was not");
				draw_text(3, "enough to free Discord, but enough");
				draw_text(3, "to turn discarded muffins into");
				draw_text(3, "vicious muffinzombies, with aim to");
				draw_text(3, "destroy all harmony in Equestria.");
				al_hold_bitmap_drawing(false);
				break;
			case 4:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0, 0, 0);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0.5, 0, 0);
				al_hold_bitmap_drawing(false);
				al_hold_bitmap_drawing(true);
				draw_text(4, "Discord, learning from his last failure,");
				draw_text(4, "turned his muffinzombies against Twilight");
				draw_text(4, "and her friends, trapping them in their");
				draw_text(4, "own homes. With the bearers of the");
				draw_text(4, "Elements out of the way, he now waits");
				draw_text(4, "until chaos takes hold of the world,");
				draw_text(4, "so he can rule Equestria once again.");
				al_hold_bitmap_drawing(false);
				break;
			case 5:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0, 0, 0);
				al_draw_bitmap_region(data->table_bitmap, al_get_bitmap_width(data->table_bitmap)/2, 0, al_get_bitmap_width(data->table_bitmap)/2, al_get_bitmap_height(data->table_bitmap), game->viewport.width*0.5, 0, 0);
				al_hold_bitmap_drawing(false);
				al_hold_bitmap_drawing(true);
				draw_text(5, "Who can defeat Discord without");
				draw_text(5, "the Elements of Harmony?");
				draw_text(5, "");
				draw_text(5, "Well... There is somepony who knows");
				draw_text(5, "all about muffins...");
				al_hold_bitmap_drawing(false);
				break;
		}
	}

	DrawPage(page);
	ALLEGRO_BITMAP* second = al_create_bitmap(game->viewport.width, game->viewport.height);
	al_set_target_bitmap(second);
	DrawPage(page+1);
	al_set_target_bitmap(data->table);
	al_draw_bitmap(second, game->viewport.width, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(second);
}

void Gamestate_Logic(struct Game *game, struct IntroResources* data) {
	data->anim += 1.0/20.0;
	if (data->in_animation) {
		int old = data->position%game->viewport.width;
		data->position -= 10;
		if (data->position%game->viewport.width>old) {
			data->in_animation = false;
			FillPage(game, data, data->page+1);
			PrintConsole(game, "Animation finished.");
			al_set_audio_stream_playing(data->audiostream, true);
		}
	}
}

void Gamestate_Draw(struct Game *game, struct IntroResources* data) {
	al_clear_to_color(al_map_rgb(0,0,0));
	if (data->in_animation) {
		al_draw_bitmap(data->table, -1*game->viewport.width + (cos(((-1*((data->position)%game->viewport.width))/(float)game->viewport.width)*(ALLEGRO_PI))/2.0)*game->viewport.width + game->viewport.width/2.0, 0, 0);
		AnimPage(game, data, data->page, al_map_rgba_f(1,1,1,1));
	}
	else {
		al_draw_bitmap(data->table, 0, 0, 0);
		AnimPage(game, data, data->page+1, al_map_rgba_f(1,1,1,1));
	}
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width/2, game->viewport.height*0.88, ALLEGRO_ALIGN_CENTRE, "Press any key to continue or escape to skip...");
	al_draw_bitmap(data->frame, 0, 0, 0);
	/*PrintConsole(game, "drawing");*/
	if (data->in_animation && data->position<=-4*game->viewport.width) {
		PrintConsole(game, "This was the last page.");
		data->in_animation = false;
		data->page--;
		StopGamestate(game, "intro");
		LoadGamestate(game, "map");
		StartGamestate(game, "map");
	}
}

void Gamestate_Start(struct Game *game, struct IntroResources* data) {
	data->audiostream = NULL;
	data->position = 0;
	data->page = 0;
	data->in_animation = false;
	data->anim = 0;

	FillPage(game, data, 1);
	al_play_sample_instance(data->music);
	FadeGamestate(game, true);
	al_set_audio_stream_playing(data->audiostream, true);
}

void Gamestate_ProcessEvent(struct Game *game, struct IntroResources* data, ALLEGRO_EVENT *ev) {
	if (ev->type != ALLEGRO_EVENT_KEY_DOWN) return;
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		StopGamestate(game, "intro");
		LoadGamestate(game, "map");
		StartGamestate(game, "map");
		return;
	}
	if (!data->in_animation) {
		if (data->audiostream) {
			al_set_audio_stream_playing(data->audiostream, false);
			al_destroy_audio_stream(data->audiostream);
			data->audiostream=NULL;
		}
		PrintConsole(game, "Animate page (was on %d)...", ++data->page);
		data->in_animation = true;
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	struct IntroResources *data = malloc(sizeof(struct IntroResources));

	data->animsprites[0] = LoadScaledBitmap(game, "intro/1.png", (int)(game->viewport.height*1.6*0.3125)*2, game->viewport.height*0.63*2);
	(*progress)(game);
	data->animsprites[1] = LoadScaledBitmap(game, "intro/2.png", (int)(game->viewport.height*1.7*0.3125)*4, game->viewport.height*0.47*3);
	(*progress)(game);
	data->animsprites[2] = LoadScaledBitmap(game, "intro/3.png", (int)(game->viewport.height*1.6*0.3125)*3, game->viewport.height*0.63*3);
	(*progress)(game);
	data->animsprites[3] = LoadScaledBitmap(game, "intro/4.png", (int)(game->viewport.height*1.6*0.3125)*2, game->viewport.height*0.63*2);
	(*progress)(game);
	data->animsprites[4] = LoadScaledBitmap(game, "intro/5.png", (int)(game->viewport.height*1.6*0.3125)*5, game->viewport.height*0.63*3);
	(*progress)(game);

	data->table_bitmap =LoadScaledBitmap(game, "intro/paper.png", game->viewport.width, game->viewport.height);
	(*progress)(game);
	data->frame =LoadScaledBitmap(game, "intro/frame.png", game->viewport.width, game->viewport.height);
	(*progress)(game);

	data->sample = al_load_sample( GetDataFilePath(game, "intro/intro.flac") );
	(*progress)(game);

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_gain(data->music, 0.75);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);
	(*progress)(game);

	if (!data->sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	data->table = al_create_bitmap(game->viewport.width*2, game->viewport.height);
	(*progress)(game);

	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.04,0 );
	(*progress)(game);

	return data;
}

void Gamestate_Unload(struct Game *game, struct IntroResources* data) {
	al_destroy_bitmap(data->frame);
	al_destroy_bitmap(data->table);
	int i;
	for (i=0; i<5; i++) {
		al_destroy_bitmap(data->animsprites[i]);
	}
	al_destroy_font(data->font);
	al_destroy_sample_instance(data->music);
	al_destroy_sample(data->sample);
	al_destroy_bitmap(data->table_bitmap);
}

void Gamestate_Stop(struct Game *game, struct IntroResources* data) {
	FadeGamestate(game, false);
	if (data->audiostream) {
		al_set_audio_stream_playing(data->audiostream, false);
		al_destroy_audio_stream(data->audiostream);
	}
	al_stop_sample_instance(data->music);
}

void Gamestate_Pause(struct Game *game, struct IntroResources* data) {}
void Gamestate_Resume(struct Game *game, struct IntroResources* data) {}
void Gamestate_Reload(struct Game *game, struct IntroResources* data) {}
