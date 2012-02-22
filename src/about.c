#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "about.h"

void About_Draw(struct Game *game) {
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_scaled_bitmap(game->about.letter,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	float x = game->about.x;
	if (x<0) x=0;
	al_draw_bitmap_region(game->about.text_bitmap, 0, x*al_get_bitmap_height(game->about.text_bitmap), al_get_bitmap_width(game->about.text_bitmap), al_get_display_height(game->display)*0.8, al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, 0);
	game->about.x+=0.00025;
	if (game->about.x>1) { 
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
	}
}

void About_Load(struct Game *game) {
	al_play_sample(game->about.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->about.fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->about.fade_bitmap);
	About_Draw(game);
}
int About_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
		return 0;
	}
}
void About_Preload(struct Game *game) {
	game->about.image = al_load_bitmap( "data/table.png" );
	game->about.letter = al_load_bitmap( "data/letter.png" );
	game->about.sample = al_load_sample( "data/about.flac" );
	game->about.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.04,0 );
	game->about.x = -0.1;
	if (!game->about.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	game->about.text_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.4, al_get_display_height(game->display)*2.3);
	al_set_target_bitmap(game->about.text_bitmap);
	//al_clear_to_color(al_map_rgba(255,0,0,0.5));
	al_draw_text(game->about.font, al_map_rgb(255,255,255), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.02*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Super Derpy: Muffin Attack");
	al_draw_text(game->about.font, al_map_rgb(255,255,255), 0.5*al_get_bitmap_width(game->about.text_bitmap), 0.05*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_CENTRE, "Version 0.1");
	
	float y=0.07;
	void draw_text(char* text) {
		al_draw_text(game->about.font, al_map_rgb(255,255,255), 0, y*al_get_bitmap_height(game->about.text_bitmap), ALLEGRO_ALIGN_LEFT, text);
		y+=0.022;
	}
	
	draw_text("Written by:");
	draw_text(" - Sebastian Krzyszkowiak");
	draw_text("   http://dosowisko.net/");
	draw_text("");
	draw_text("Graphics:");
	draw_text(" - hombre0 (menu background)");
	draw_text("   http://hombre0.deviantart.com/");
	draw_text(" - Siobhan Brewer (cloud shapes)");
	draw_text("   http://boxdrink.deviantart.com/");
	draw_text("");
	draw_text("Music:");
	draw_text(" - Dr Dissonance (\"Don Discord\")");
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
	draw_text("Super Derpy: Muffin Attack comes");
	draw_text("with ABSOLUTELY NO WARRANTY.");
	draw_text("You may redistribute copies of this");
	draw_text("game under the terms of");
	draw_text("GNU General Public License.");
	draw_text("For more information, see the file");
	draw_text("named COPYING.");
	draw_text("");
	draw_text("Super Derpy: Muffin Attack is");
	draw_text("a fan-game, with no-profit intended.");
	draw_text("Its author is unpaid volunteer and");
	draw_text("doesn't receive any economical profit");
	draw_text("from it.");
	draw_text("");
	draw_text("My Little Pony: Friendship is Magic™");
	draw_text("is copyrighted trademark owned");
	draw_text("by Hasbro.");
	draw_text("");
	draw_text("Author of Super Derpy is not affiliated");
	draw_text("to Hasbro, The Hub or its associates.");

	game->about.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));

	al_set_target_bitmap(game->about.fade_bitmap);
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_scaled_bitmap(game->about.letter,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap_region(game->about.text_bitmap, 0, 0, al_get_bitmap_width(game->about.text_bitmap), al_get_display_height(game->display)*0.8, al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, 0);

	al_set_target_bitmap(al_get_backbuffer(game->display));
}
void About_Unload(struct Game *game) {
	if (game->about.x<0) game->about.x=0;
	ALLEGRO_EVENT ev;
	game->about.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->about.fade_bitmap);
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_scaled_bitmap(game->about.letter,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap_region(game->about.text_bitmap, 0, game->about.x*al_get_bitmap_height(game->about.text_bitmap), al_get_bitmap_width(game->about.text_bitmap), al_get_display_height(game->display)*0.8, al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->about.fade_bitmap, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->about.image);
	al_destroy_bitmap(game->about.letter);
	al_destroy_bitmap(game->about.fade_bitmap);
	al_destroy_bitmap(game->about.text_bitmap);
	al_destroy_sample(game->about.sample);
	al_destroy_font(game->about.font);
}
