/*! \file about.c
 *  \brief About screen.
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
#include "about.h"

void About_Logic(struct Game *game) {
	if (al_get_sample_instance_position(game->about.music)<700000) { return; }
	if (game->about.fadeloop>=0) {
		if (game->about.fadeloop==0) PrintConsole(game, "Fade in");
		game->about.fadeloop+=5;
		if (game->about.fadeloop>=256) {
			al_destroy_bitmap(game->about.fade_bitmap);
			game->about.fadeloop=-1;
		}
		return;
	}
	game->about.x+=0.00025;
}

void About_Draw(struct Game *game) {
	/*PrintConsole(game, "%d", al_get_sample_instance_position(game->about.music));*/
	if (al_get_sample_instance_position(game->about.music)<700000) { al_clear_to_color(al_map_rgba(0,0,0,0)); return; }
	if (game->about.fadeloop>=0) {
		if (game->about.fadeloop==0) PrintConsole(game, "Fade in");
		al_draw_tinted_bitmap(game->about.fade_bitmap,al_map_rgba_f(game->about.fadeloop/255.0,game->about.fadeloop/255.0,game->about.fadeloop/255.0,1),0,0,0);
		return;
	}

	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,game->viewportWidth, game->viewportHeight,0);
	al_draw_bitmap(game->about.letter, game->viewportWidth*0.3, -game->viewportHeight*0.1, 0);
	float x = game->about.x;
	if (x<0) x=0;
	ALLEGRO_BITMAP* subbitmap;
	subbitmap = al_create_sub_bitmap(game->about.text_bitmap, 0, x*al_get_bitmap_height(game->about.text_bitmap), al_get_bitmap_width(game->about.text_bitmap), game->viewportHeight);
	al_draw_rotated_bitmap(subbitmap, al_get_bitmap_width(subbitmap)/2.0, al_get_bitmap_height(subbitmap)/2.0, game->viewportWidth*0.5+al_get_bitmap_width(subbitmap)/2.0, game->viewportHeight*0.1+al_get_bitmap_height(subbitmap)/2.0, -0.11, 0);
	al_destroy_bitmap(subbitmap);
	if ((game->about.x>1) && (game->about.x<10)) {
		game->about.x=10;
		UnloadGameState(game);
		game->loadstate = GAMESTATE_MENU;
		LoadGameState(game);
	}
}

void About_Load(struct Game *game) {
	al_play_sample_instance(game->about.music);
	game->about.fadeloop = 0;
	About_Draw(game);
}

int About_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->loadstate = GAMESTATE_MENU;
		LoadGameState(game);
	}
	return 0;
}

void About_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(6);

	game->about.image =LoadScaledBitmap("table.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	game->about.letter = LoadScaledBitmap("about/letter.png", game->viewportHeight*1.3, game->viewportHeight*1.3 );
	PROGRESS;

	game->about.sample = al_load_sample( GetDataFilePath("about/about.flac") );
	PROGRESS;

	game->about.music = al_create_sample_instance(game->about.sample);
	al_attach_sample_instance_to_mixer(game->about.music, game->audio.music);
	al_set_sample_instance_playmode(game->about.music, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_position(game->about.music, game->music ? 420000 : 700000);

	game->about.font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.035,0 );
	PROGRESS;
	game->about.x = -0.1;
	if (!game->about.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	game->about.text_bitmap = al_create_bitmap(game->viewportWidth*0.4, game->viewportHeight*3.225);
	al_set_target_bitmap(game->about.text_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_text(game->about.font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.015*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Super Derpy: Muffin Attack");
	al_draw_text(game->about.font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.035*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Version 0.1a (Development Preview)");
	
	float y=0.07;
	void draw_text(char* text) {
		al_draw_text(game->about.font, al_map_rgb(0,0,0), 0, y*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_LEFT, text);
		y+=0.0131;
	}

	draw_text("Written by:");
	draw_text(" - Sebastian Krzyszkowiak");
	draw_text("   http://dosowisko.net/");
	draw_text("");
	draw_text("Animations:");
	draw_text(" - Yudhaikeledai");
	draw_text("   http://yudhaikeledai.deviantart.com/");
	draw_text("");
	draw_text("Voices:");
	/*draw_text(" - BaldDumboRat");
	draw_text("     Derpy Hooves");*/
	draw_text(" - MEMJ0123");
	draw_text("   http://www.youtube.com/user/MEMJ0123");
	draw_text("     Princess Celestia");
	draw_text(" - Meredith Sims (Emichwan88)");
	draw_text("   http://www.youtube.com/Emichwan88");
	draw_text("     Fluttershy");
	draw_text("     Twilight Sparkle");
	/*draw_text(" - Hnilmik");
	draw_text("     Rarity");
	draw_text(" - Karen VO");
	draw_text("     Applejack");*/
	/*	draw_text(" - Erica Mendez");
	draw_text("     Rainbow Dash");
	draw_text(" - Emily Fajardo");
	draw_text("     Pinkie Pie");*/
	draw_text("");
	draw_text("Original graphics:");
	draw_text(" - Ania Karlik");
	draw_text("   (Eris)");
	draw_text(" - Agata Kurczewska");
	draw_text("   http://kiciazkrainyczarow.deviantart.com/");
	draw_text(" - Wiktor Grześkowiak");
	draw_text("   http://gl0w.pl/");
	draw_text("");
	draw_text("Music:");
	draw_text(" - Claire Anne Carr");
	draw_text("   http://claireannecarr.bandcamp.com/");
	draw_text(" - Dr Dissonance");
	draw_text("   http://dr-dissonance.tumblr.com/");
	draw_text(" - Dan O'Connor");
	draw_text("   http://www.danosongs.com/");
	draw_text(" - Kevin MacLeod");
	draw_text("   http://incompetech.com/");
	draw_text("");
	draw_text("Sounds:");
	draw_text(" - Sound Jay");
	draw_text("   http://www.soundjay.com/");
	draw_text("");
	draw_text("Other graphics:");
	draw_text(" - CptOfTheFriendship");
	draw_text("   http://cptofthefriendship.deviantart.com/");
	draw_text(" - hombre0");
	draw_text("   http://hombre0.deviantart.com/");
	draw_text(" - Siobhan Brewer");
	draw_text("   http://boxdrink.deviantart.com/");
	draw_text("");
	draw_text("Super Derpy: Muffin Attack comes");
	draw_text("with ABSOLUTELY NO WARRANTY.");
	draw_text("You may redistribute copies of this");
	draw_text("game under the terms of");
	draw_text("GNU General Public License.");
	draw_text("For more information, see the file");
	draw_text("named COPYING.");
	draw_text("");
	draw_text("Super Derpy: Muffin Attack is");
	draw_text("a fan-made game with no profit");
	draw_text("intended. Its author is unpaid");
	draw_text("volunteer and doesn't receive");
	draw_text("any economical profit from it.");
	draw_text("");
	draw_text("My Little Pony: Friendship is Magic");
	draw_text("is copyrighted trademark owned");
	draw_text("by Hasbro.");
	draw_text("");
	draw_text("Author of Super Derpy is not");
	draw_text("affiliated to Hasbro, The Hub");
	draw_text("or its associates.");
	draw_text("");
	draw_text("http://www.superderpy.com/");
	PROGRESS;

	game->about.fade_bitmap = al_create_bitmap(game->viewportWidth, game->viewportHeight);

	al_set_target_bitmap(game->about.fade_bitmap);
	al_draw_bitmap(game->about.image, 0, 0, 0);
	al_draw_bitmap(game->about.letter, game->viewportWidth*0.3, -game->viewportHeight*0.1, 0);
	ALLEGRO_BITMAP* subbitmap;
	subbitmap = al_create_sub_bitmap(game->about.text_bitmap, 0, 0, al_get_bitmap_width(game->about.text_bitmap), game->viewportHeight);
	al_draw_rotated_bitmap(subbitmap, al_get_bitmap_width(subbitmap)/2.0, al_get_bitmap_height(subbitmap)/2.0, game->viewportWidth*0.5+al_get_bitmap_width(subbitmap)/2.0, game->viewportHeight*0.1+al_get_bitmap_height(subbitmap)/2.0, -0.11, 0);
	al_destroy_bitmap(subbitmap);

	al_set_target_bitmap(al_get_backbuffer(game->display));
	PROGRESS;
}

void About_Unload(struct Game *game) {
	if (game->about.fadeloop!=0) {
		FadeGameState(game, false);
	}
	al_destroy_bitmap(game->about.image);
	al_destroy_bitmap(game->about.letter);
	if (game->about.fadeloop>=0) al_destroy_bitmap(game->about.fade_bitmap);
	al_destroy_bitmap(game->about.text_bitmap);
	al_destroy_sample_instance(game->about.music);
	al_destroy_sample(game->about.sample);
	al_destroy_font(game->about.font);
}
