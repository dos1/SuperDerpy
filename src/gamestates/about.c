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
	if (game->about.fadeloop==0) {
		PrintConsole(game, "Fade in");
		game->about.fadeloop=1;
		FadeGameState(game, true);
	}
	game->about.x+=0.00025;
}

void About_Draw(struct Game *game) {
	/*PrintConsole(game, "%d", al_get_sample_instance_position(game->about.music));*/
	if ((al_get_sample_instance_position(game->about.music)<700000) || (game->about.fadeloop==0)) {
		al_clear_to_color(al_map_rgba(0,0,0,0));
		return;
	}

	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,game->viewportWidth, game->viewportHeight,0);
	al_draw_bitmap(game->about.letter, game->viewportWidth-al_get_bitmap_width(game->about.letter), -game->viewportHeight*0.1, 0);
	float x = game->about.x;
	if (x<0) x=0;
	ALLEGRO_BITMAP* subbitmap;
	subbitmap = al_create_sub_bitmap(game->about.text_bitmap, 0, x*al_get_bitmap_height(game->about.text_bitmap), al_get_bitmap_width(game->about.text_bitmap), game->viewportHeight);
	al_draw_rotated_bitmap(subbitmap, al_get_bitmap_width(subbitmap)/2.0, al_get_bitmap_height(subbitmap)/2.0, game->viewportWidth-al_get_bitmap_width(game->about.letter)+al_get_bitmap_width(subbitmap), game->viewportHeight*0.1+al_get_bitmap_height(subbitmap)/2.0, -0.11, 0);
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

static void draw_text(struct Game *game, char* text, float* y) {
	al_draw_text(game->about.font, al_map_rgb(0,0,0), 0, (*y)*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_LEFT, text);
	*y += 0.0131;
}

void About_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(5);

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
	game->about.text_bitmap = al_create_bitmap(game->viewportHeight*1.6*0.4, game->viewportHeight*3.225);
	al_set_target_bitmap(game->about.text_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_text(game->about.font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.015*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Super Derpy: Muffin Attack");
	al_draw_text(game->about.font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.035*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Version 0.1a (Development Preview)");

	float y=0.07;

	draw_text(game, "Written by:", &y);
	draw_text(game, " - Sebastian Krzyszkowiak", &y);
	draw_text(game, "   http://dosowisko.net/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Animations:", &y);
	draw_text(game, " - Yudhaikeledai", &y);
	draw_text(game, "   http://yudhaikeledai.deviantart.com/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Voices:", &y);
	/*draw_text(game, " - BaldDumboRat", &y);
	draw_text(game, "     Derpy Hooves", &y);*/
	draw_text(game, " - MEMJ0123", &y);
	draw_text(game, "   http://www.youtube.com/user/MEMJ0123", &y);
	draw_text(game, "     Princess Celestia", &y);
	draw_text(game, " - Meredith Sims (Emichwan88)", &y);
	draw_text(game, "   http://www.youtube.com/Emichwan88", &y);
	draw_text(game, "     Fluttershy", &y);
	draw_text(game, "     Twilight Sparkle", &y);
	/*draw_text(game, " - Hnilmik", &y);
	draw_text(game, "     Rarity", &y);
	draw_text(game, " - Karen VO", &y);
	draw_text(game, "     Applejack", &y);*/
	/*	draw_text(game, " - Erica Mendez", &y);
	draw_text(game, "     Rainbow Dash", &y);
	draw_text(game, " - Emily Fajardo", &y);
	draw_text(game, "     Pinkie Pie", &y);*/
	draw_text(game, "", &y);
	draw_text(game, "Original graphics:", &y);
	draw_text(game, " - Ania Karlik", &y);
	draw_text(game, "   (Eris)", &y);
	draw_text(game, " - Agata Kurczewska", &y);
	draw_text(game, "   http://kiciazkrainyczarow.deviantart.com/", &y);
	draw_text(game, " - Wiktor Grześkowiak", &y);
	draw_text(game, "   http://gl0w.pl/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Music:", &y);
	draw_text(game, " - Claire Anne Carr", &y);
	draw_text(game, "   http://claireannecarr.bandcamp.com/", &y);
	draw_text(game, " - Dr Dissonance", &y);
	draw_text(game, "   http://dr-dissonance.tumblr.com/", &y);
	draw_text(game, " - Dan O'Connor", &y);
	draw_text(game, "   http://www.danosongs.com/", &y);
	draw_text(game, " - Kevin MacLeod", &y);
	draw_text(game, "   http://incompetech.com/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Sounds:", &y);
	draw_text(game, " - Sound Jay", &y);
	draw_text(game, "   http://www.soundjay.com/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Other graphics:", &y);
	draw_text(game, " - CptOfTheFriendship", &y);
	draw_text(game, "   http://cptofthefriendship.deviantart.com/", &y);
	draw_text(game, " - hombre0", &y);
	draw_text(game, "   http://hombre0.deviantart.com/", &y);
	draw_text(game, " - Siobhan Brewer", &y);
	draw_text(game, "   http://boxdrink.deviantart.com/", &y);
	draw_text(game, "", &y);
	draw_text(game, "Super Derpy: Muffin Attack comes", &y);
	draw_text(game, "with ABSOLUTELY NO WARRANTY.", &y);
	draw_text(game, "You may redistribute copies of this", &y);
	draw_text(game, "game under the terms of", &y);
	draw_text(game, "GNU General Public License.", &y);
	draw_text(game, "For more information, see the file", &y);
	draw_text(game, "named COPYING.", &y);
	draw_text(game, "", &y);
	draw_text(game, "Super Derpy: Muffin Attack is", &y);
	draw_text(game, "a fan-made game with no profit", &y);
	draw_text(game, "intended. Its author is unpaid", &y);
	draw_text(game, "volunteer and doesn't receive", &y);
	draw_text(game, "any economical profit from it.", &y);
	draw_text(game, "", &y);
	draw_text(game, "My Little Pony: Friendship is Magic", &y);
	draw_text(game, "is copyrighted trademark owned", &y);
	draw_text(game, "by Hasbro.", &y);
	draw_text(game, "", &y);
	draw_text(game, "Author of Super Derpy is not", &y);
	draw_text(game, "affiliated to Hasbro, The Hub", &y);
	draw_text(game, "or its associates.", &y);
	draw_text(game, "", &y);
	draw_text(game, "http://www.superderpy.com/", &y);
	PROGRESS;
}

void About_Unload(struct Game *game) {
	if (game->about.fadeloop!=0) {
		FadeGameState(game, false);
	}
	al_destroy_bitmap(game->about.image);
	al_destroy_bitmap(game->about.letter);
	al_destroy_bitmap(game->about.text_bitmap);
	al_destroy_sample_instance(game->about.music);
	al_destroy_sample(game->about.sample);
	al_destroy_font(game->about.font);
}
