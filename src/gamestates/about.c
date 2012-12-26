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
#include <allegro5/allegro_ttf.h>
#include "../utils.h"
#include "about.h"

int Gamestate_ProgressCount = 0;

void Gamestate_Logic(struct Game *game, struct AboutResources* data) {
	if (al_get_sample_instance_position(data->music)<700000) { return; }
	if (data->fadeloop==0) {
		PrintConsole(game, "Fade in");
		data->fadeloop=1;
		FadeGamestate(game, true);
	}
	data->x+=0.00025;
}

void Gamestate_Draw(struct Game *game, struct AboutResources* data) {
	/*PrintConsole(game, "%d", al_get_sample_instance_position(data->music));*/
	if ((al_get_sample_instance_position(data->music)<700000) || (data->fadeloop==0)) {
		al_clear_to_color(al_map_rgba(0,0,0,0));
		return;
	}

	al_draw_scaled_bitmap(data->image,0,0,al_get_bitmap_width(data->image),al_get_bitmap_height(data->image),0,0,game->viewport.width, game->viewport.height,0);
	al_draw_bitmap(data->letter, game->viewport.width-al_get_bitmap_width(data->letter), -game->viewport.height*0.1, 0);
	float x = data->x;
	if (x<0) x=0;
	ALLEGRO_BITMAP* subbitmap;
	subbitmap = al_create_sub_bitmap(data->text_bitmap, 0, x*al_get_bitmap_height(data->text_bitmap), al_get_bitmap_width(data->text_bitmap), game->viewport.height);
	al_draw_rotated_bitmap(subbitmap, al_get_bitmap_width(subbitmap)/2.0, al_get_bitmap_height(subbitmap)/2.0, game->viewport.width-al_get_bitmap_width(data->letter)+al_get_bitmap_width(subbitmap), game->viewport.height*0.1+al_get_bitmap_height(subbitmap)/2.0, -0.11, 0);
	al_destroy_bitmap(subbitmap);
	if ((data->x>1) && (data->x<10)) {
		data->x=10;
		SwitchGamestate(game, "about", "menu");
	}
}

void Gamestate_Start(struct Game *game, struct AboutResources* data) {
	al_play_sample_instance(data->music);
	data->fadeloop = 0;
}

void Gamestate_ProcessEvent(struct Game *game, struct AboutResources *data, ALLEGRO_EVENT *ev) {
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			SwitchGamestate(game, "about", "menu");
		}
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game *game)) {

	struct AboutResources *data = malloc(sizeof(struct AboutResources));

	data->image =LoadScaledBitmap(game, "table.png", game->viewport.width, game->viewport.height);
	data->letter = LoadScaledBitmap(game, "about/letter.png", game->viewport.height*1.3, game->viewport.height*1.3 );

	data->sample = al_load_sample( GetDataFilePath("about/about.flac") );

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_position(data->music, game->config.music ? 420000 : 700000);

	data->font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewport.height*0.035,0 );
	data->x = -0.1;
	if (!data->sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	data->text_bitmap = al_create_bitmap(game->viewport.height*1.6*0.4, game->viewport.height*3.225);
	al_set_target_bitmap(data->text_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_text(data->font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(data->text_bitmap), 0.015*al_get_bitmap_height(data->text_bitmap), ALLEGRO_ALIGN_CENTRE, "Super Derpy: Muffin Attack");
	al_draw_text(data->font, al_map_rgb(0,0,0), 0.5*al_get_bitmap_width(data->text_bitmap), 0.035*al_get_bitmap_height(data->text_bitmap), ALLEGRO_ALIGN_CENTRE, "Version 0.1a (Development Preview)");
	
	float y=0.07;
	void draw_text(char* text) {
		al_draw_text(data->font, al_map_rgb(0,0,0), 0, y*al_get_bitmap_height(data->text_bitmap), ALLEGRO_ALIGN_LEFT, text);
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

	al_set_target_backbuffer(game->display);
	return data;
}

void Gamestate_Unload(struct Game *game, struct AboutResources* data) {
	al_destroy_bitmap(data->image);
	al_destroy_bitmap(data->letter);
	al_destroy_bitmap(data->text_bitmap);
	al_destroy_sample_instance(data->music);
	al_destroy_sample(data->sample);
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Stop(struct Game *game, struct AboutResources* data) {
	if (data->fadeloop!=0) {
		FadeGamestate(game, false);
	}
	al_stop_sample_instance(data->music);
}

void Gamestate_Reload(struct Game *game, struct AboutResources* data) {}

void Gamestate_Resume(struct Game *game, struct AboutResources* data) {}
void Gamestate_Pause(struct Game *game, struct AboutResources* data) {}
