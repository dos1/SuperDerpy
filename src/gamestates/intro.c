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
#include "intro.h"
#include "map.h"

void AnimPage(struct Game *game, int page, ALLEGRO_COLOR tint) {
	int offset = 0;
	if (game->intro.in_animation) offset = -1*game->viewportWidth + (cos(((-1*((game->intro.position)%game->viewportWidth))/(float)game->viewportWidth)*(ALLEGRO_PI))/2.0)*game->viewportWidth + game->viewportWidth/2.0;

	int amount1 = 2, amount2 = 2;
	float anim = game->intro.anim;

	if (page==2) { amount1=4; amount2=3; }
	if (page==3) { amount1=3; amount2=3; }
	if (page==5) { amount1=5; amount2=3; anim*=2; }

	if (page<6) al_draw_tinted_bitmap_region(game->intro.animsprites[page-1],tint,game->viewportHeight*1.6*0.3125*(int)fmod(anim,amount1),game->viewportHeight*0.63*(((int)(anim/amount1))%amount2),game->viewportHeight*1.6*0.3125, game->viewportHeight*0.63,offset+game->viewportWidth*0.08, game->viewportHeight*0.18,0);

	amount1 = 2;
	amount2 = 2;
	if (page==1) { amount1=4; amount2=3; }
	if (page==2) { amount1=3; amount2=3; }
	if (page==4) { amount1=5; amount2=3; anim*=2; }

	if (page<5) al_draw_tinted_bitmap_region(game->intro.animsprites[page],tint,game->viewportHeight*1.6*0.3125*(int)fmod(anim,amount1),game->viewportHeight*0.63*(((int)(anim/amount1))%amount2),game->viewportHeight*1.6*0.3125, game->viewportHeight*0.63,offset+game->viewportWidth*1.08, game->viewportHeight*0.18,0);
}

void FillPage(struct Game *game, int page) {
	char filename[30] = { };
	sprintf(filename, "intro/%d.flac", page);

	game->intro.audiostream = al_load_audio_stream(GetDataFilePath(filename), 4, 1024);
	al_attach_audio_stream_to_mixer(game->intro.audiostream, game->audio.voice);
	al_set_audio_stream_playing(game->intro.audiostream, false);
	al_set_audio_stream_gain(game->intro.audiostream, 1.75);

	al_set_target_bitmap(game->intro.table);
	float y = 0.2;
	float oldx = -1;
	void draw_text(int page, char* text) {
		float x = 0.45;
		if (page!=oldx) { y=0.2; oldx=page; }
		al_draw_text_with_shadow(game->intro.font, al_map_rgb(255,255,255), game->viewportWidth*x, game->viewportHeight*y, ALLEGRO_ALIGN_LEFT, text);
		y+=0.07;
	}
	
	void DrawPage(int p) {
		switch (p) {
			case 1:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap(game->intro.table_bitmap, 0, 0, 0);
				al_hold_bitmap_drawing(true);
				draw_text(1, "Ever since Twilight Sparkle and her");
				draw_text(1, "friends imprisoned Discord in stone,");
				draw_text(1, "Equestria had been peaceful for");
				draw_text(1, "a long time.");
				al_hold_bitmap_drawing(false);
				break;
			case 2:
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0, 0, 0);
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0.5, 0, 0);
				al_hold_bitmap_drawing(false);
				al_hold_bitmap_drawing(true);
				draw_text(2, "Until one day a reckless pony caused");
				draw_text(2, "a tiny bit of chaos near Discord’s");
				draw_text(2, "statue.");
				al_hold_bitmap_drawing(false);
				break;
			case 3:
				al_hold_bitmap_drawing(true);
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0, 0, 0);
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0.5, 0, 0);
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
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0, 0, 0);
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0.5, 0, 0);
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
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0, 0, 0);
				al_draw_bitmap_region(game->intro.table_bitmap, al_get_bitmap_width(game->intro.table_bitmap)/2, 0, al_get_bitmap_width(game->intro.table_bitmap)/2, al_get_bitmap_height(game->intro.table_bitmap), game->viewportWidth*0.5, 0, 0);
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
	ALLEGRO_BITMAP* second = al_create_bitmap(game->viewportWidth, game->viewportHeight);
	al_set_target_bitmap(second);
	DrawPage(page+1);
	al_set_target_bitmap(game->intro.table);
	al_draw_bitmap(second, game->viewportWidth, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(second);
}

void Intro_Logic(struct Game *game) {
	game->intro.anim += 1.0/20.0;
	if (game->intro.in_animation) {
		int old = game->intro.position%game->viewportWidth;
		game->intro.position -= 10;
		if (game->intro.position%game->viewportWidth>old) {
			game->intro.in_animation = false;
			FillPage(game, game->intro.page+1);
			PrintConsole(game, "Animation finished.");
			al_set_audio_stream_playing(game->intro.audiostream, true);
		}
	}
}

void Intro_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	if (game->intro.in_animation) {
		al_draw_bitmap(game->intro.table, -1*game->viewportWidth + (cos(((-1*((game->intro.position)%game->viewportWidth))/(float)game->viewportWidth)*(ALLEGRO_PI))/2.0)*game->viewportWidth + game->viewportWidth/2.0, 0, 0);
		AnimPage(game, game->intro.page, al_map_rgba_f(1,1,1,1));
	}
	else {
		al_draw_bitmap(game->intro.table, 0, 0, 0);
		AnimPage(game, game->intro.page+1, al_map_rgba_f(1,1,1,1));
	}
	al_draw_text_with_shadow(game->intro.font, al_map_rgb(255,255,255), game->viewportWidth/2, game->viewportHeight*0.90, ALLEGRO_ALIGN_CENTRE, "Press any key to continue or escape to skip...");
	al_draw_bitmap(game->intro.frame, 0, 0, 0);
	/*PrintConsole(game, "drawing");*/
	if (game->intro.in_animation && game->intro.position<=-4*game->viewportWidth) {
		PrintConsole(game, "This was the last page.");
		game->intro.in_animation = false;
		game->intro.page--;
		UnloadGameState(game);
		game->loadstate = GAMESTATE_MAP;
		PrintConsole(game, "Chainloading GAMESTATE_MAP...");
		LoadGameState(game);
	}
}

void Intro_Load(struct Game *game) {
	al_play_sample_instance(game->intro.music);
	FadeGameState(game, true);
	al_set_audio_stream_playing(game->intro.audiostream, true);
}

int Intro_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->loadstate = GAMESTATE_MAP;
		PrintConsole(game, "Chainloading GAMESTATE_MAP...");
		LoadGameState(game);
		return 0;
	}
	if (!game->intro.in_animation) {
		if (game->intro.audiostream) {
			al_set_audio_stream_playing(game->intro.audiostream, false);
			al_destroy_audio_stream(game->intro.audiostream);
			game->intro.audiostream=NULL;
		}
		PrintConsole(game, "Animate page (was on %d)...", ++game->intro.page);
		game->intro.in_animation = true;
	}
	return 0;
}

void Intro_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(16);

	game->intro.audiostream = NULL;
	game->intro.position = 0;
	game->intro.page = 0;
	game->intro.in_animation = false;
	game->intro.anim = 0;

	game->intro.animsprites[0] = LoadScaledBitmap("intro/1.png", (int)(game->viewportHeight*1.6*0.3125)*2, game->viewportHeight*0.63*2);
	PROGRESS;
	game->intro.animsprites[1] = LoadScaledBitmap("intro/2.png", (int)(game->viewportHeight*1.6*0.3125)*4, game->viewportHeight*0.63*3);
	PROGRESS;
	game->intro.animsprites[2] = LoadScaledBitmap("intro/3.png", (int)(game->viewportHeight*1.6*0.3125)*3, game->viewportHeight*0.63*3);
	PROGRESS;
	game->intro.animsprites[3] = LoadScaledBitmap("intro/4.png", (int)(game->viewportHeight*1.6*0.3125)*2, game->viewportHeight*0.63*2);
	PROGRESS;
	game->intro.animsprites[4] = LoadScaledBitmap("intro/5.png", (int)(game->viewportHeight*1.6*0.3125)*5, game->viewportHeight*0.63*3);
	PROGRESS;

	game->intro.table_bitmap =LoadScaledBitmap("intro/paper.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	game->intro.frame =LoadScaledBitmap("intro/frame.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;

	game->intro.sample = al_load_sample( GetDataFilePath("intro/intro.flac") );
	PROGRESS;

	game->intro.music = al_create_sample_instance(game->intro.sample);
	al_attach_sample_instance_to_mixer(game->intro.music, game->audio.music);
	al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music, ALLEGRO_PLAYMODE_LOOP);

	if (!game->intro.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	game->intro.table = al_create_bitmap(game->viewportWidth*2, game->viewportHeight);

	game->intro.font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.04,0 );

	FillPage(game, 1);
	PROGRESS;
	al_set_target_bitmap(al_get_backbuffer(game->display));
	PrintConsole(game, "Chainpreloading GAMESTATE_MAP...");
	PROGRESS;
	void MapProgress(struct Game* game, float p) {
		if (progress) (*progress)(game, load_p+=1/load_a);
	}
	Map_Preload(game, &MapProgress);
}

void Intro_Unload(struct Game *game) {
	FadeGameState(game, false);
	if (game->intro.audiostream) {
		al_set_audio_stream_playing(game->intro.audiostream, false);
		al_destroy_audio_stream(game->intro.audiostream);
	}
	al_destroy_bitmap(game->intro.frame);
	al_destroy_bitmap(game->intro.table);
	int i;
	for (i=0; i<5; i++) {
		al_destroy_bitmap(game->intro.animsprites[i]);
	}
	al_destroy_font(game->intro.font);
	al_destroy_sample_instance(game->intro.music);
	al_destroy_sample(game->intro.sample);
	al_destroy_bitmap(game->intro.table_bitmap);
}
