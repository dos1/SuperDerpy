#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include <stdio.h>
#include "intro.h"
#include "map.h"

void Intro_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	//printf("%f\n", ((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display)));
	if (game->intro.in_animation)
		al_draw_bitmap(game->intro.table, -1*(game->intro.page)*al_get_display_width(game->display) + (cos(((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display))*(ALLEGRO_PI))/2.0)*al_get_display_width(game->display) + al_get_display_width(game->display)/2.0, 0, 0);//al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
	else
		al_draw_bitmap(game->intro.table, -1*(game->intro.page)*al_get_display_width(game->display), 0, 0); //al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
	//al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");
	al_draw_text(game->intro.font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.90, ALLEGRO_ALIGN_CENTRE, "Press any key to continue or escape to skip...");
	//PrintConsole(game, "drawing");
	if (game->intro.in_animation) {
		//PrintConsole(game, "animating");
		game->intro.position -= 10;
		if (game->intro.position%al_get_display_width(game->display)==0) {
			game->intro.in_animation = false;
			PrintConsole(game, "Animation finished.");
		}
		else if (game->intro.position<=-4*al_get_display_width(game->display)) {
			PrintConsole(game, "This was the last page.");
			UnloadGameState(game);
			game->loadstate = GAMESTATE_MAP;
			PrintConsole(game, "Chainloading GAMESTATE_MAP...");
			LoadGameState(game);
		}
	}
}

void Intro_Load(struct Game *game) {
	al_play_sample(game->intro.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->intro.table,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	Intro_Draw(game);
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
		PrintConsole(game, "Animate page (was on %d)...", ++game->intro.page);
		game->intro.in_animation = true;
	}
	return 0;
}
void Intro_Preload(struct Game *game) {
	game->intro.position = 0;
	game->intro.page = 0;
	game->intro.in_animation = false;
	game->intro.table_bitmap = al_load_bitmap( "data/discord.png" );
	game->intro.sample = al_load_sample( "data/intro.flac" );
	
	if (!game->intro.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	game->intro.table = al_create_bitmap(al_get_display_width(game->display)*5, al_get_display_height(game->display));
	game->intro.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.045,0 );
	al_set_target_bitmap(game->intro.table);
	al_draw_tinted_scaled_bitmap(game->intro.table_bitmap, al_map_rgba_f(0.5, 0.5, 0.5, 1), 0, 0, al_get_bitmap_width(game->intro.table_bitmap), al_get_bitmap_height(game->intro.table_bitmap), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display), 0);
	//game->intro.table_bitmap = al_load_bitmap( "table.png" );
	al_draw_tinted_scaled_bitmap(game->intro.table_bitmap, al_map_rgba_f(0.5, 0.5, 0.5, 1), 0, 0, al_get_bitmap_width(game->intro.table_bitmap), al_get_bitmap_height(game->intro.table_bitmap), al_get_display_width(game->display), 0, al_get_display_width(game->display), al_get_display_height(game->display), ALLEGRO_FLIP_HORIZONTAL);
	//game->intro.table_bitmap = al_load_bitmap( "menu.png" );
	al_draw_tinted_scaled_bitmap(game->intro.table_bitmap, al_map_rgba_f(0.5, 0.5, 0.5, 1), 0, 0, al_get_bitmap_width(game->intro.table_bitmap), al_get_bitmap_height(game->intro.table_bitmap), al_get_display_width(game->display)*2, 0, al_get_display_width(game->display), al_get_display_height(game->display), ALLEGRO_FLIP_VERTICAL);
	//game->intro.table_bitmap = al_load_bitmap( "table.png" );
	al_draw_tinted_scaled_bitmap(game->intro.table_bitmap, al_map_rgba_f(0.5, 0.5, 0.5, 1), 0, 0, al_get_bitmap_width(game->intro.table_bitmap), al_get_bitmap_height(game->intro.table_bitmap), al_get_display_width(game->display)*3, 0, al_get_display_width(game->display), al_get_display_height(game->display), ALLEGRO_FLIP_HORIZONTAL^ALLEGRO_FLIP_VERTICAL);
	al_draw_tinted_scaled_bitmap(game->intro.table_bitmap, al_map_rgba_f(0.5, 0.5, 0.5, 1), 0, 0, al_get_bitmap_width(game->intro.table_bitmap), al_get_bitmap_height(game->intro.table_bitmap), al_get_display_width(game->display)*4, 0, al_get_display_width(game->display), al_get_display_height(game->display), 0);

	float y;
	float oldx = -1;
	void draw_text(int page, char* text) {
		float x = page - 1 + 0.45;
		if (x!=oldx) { y=0.2; oldx=x; }
		al_draw_text(game->intro.font, al_map_rgb(255,255,255), al_get_display_width(game->display)*x, al_get_display_height(game->display)*y, ALLEGRO_ALIGN_LEFT, text);
		y+=0.07;
	}

	draw_text(1, "Since Twilight Sparkle and her five best");
	draw_text(1, "friends defeated Discord and imprisoned");
	draw_text(1, "him in a stone using the Elements of");
	draw_text(1, "Harmony, Equestia has been a peaceful");
	draw_text(1, "place for a quite long time.");

	draw_text(2, "Suddenly, one day some reckless pony");
	draw_text(2, "accidentally introduced a tiny little");
	draw_text(2, "bit of chaos near Discords figure.");

	draw_text(3, "A tiny little bit of chaos turned not to");
	draw_text(3, "be enough for Discord to escape from rock,");
	draw_text(3, "but enough to turn nearly dropped muffins");
	draw_text(3, "into muffinzombies, with aim to destroy all");
	draw_text(3, "the harmony in Equestria.");

	draw_text(4, "Discord, who has learned his lession after");
	draw_text(4, "last failure, turned his muffinzombie-gang");
	draw_text(4, "against the same mares who previously");
	draw_text(4, "defeated him, prisoning them in their own");
	draw_text(4, "houses. Thanks to that, he's now able to");
	draw_text(4, "wait until the world is again chaotic enough");
	draw_text(4, "for him to escape and rule Equestria");
	draw_text(4, "once again.");

	draw_text(5, "Who is able to defeat Discord when the");
	draw_text(5, "Elements of Harmony are unavailable?");
	draw_text(5, "");
	draw_text(5, "Well... There is somepony who knows");
	draw_text(5, "everything about muffins...");

	al_set_target_bitmap(al_get_backbuffer(game->display));
	PrintConsole(game, "Chainpreloading GAMESTATE_MAP...");
	Map_Preload(game);
}
void Intro_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		if (game->intro.in_animation)
			al_draw_tinted_bitmap(game->intro.table, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), -1*(game->intro.page)*al_get_display_width(game->display) + (cos(((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display))*(3.1415))/2.0)*al_get_display_width(game->display)  + al_get_display_width(game->display)/2.0, 0, 0); //al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
		else
			al_draw_tinted_bitmap(game->intro.table, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), -1*(game->intro.page)*al_get_display_width(game->display), 0, 0); //al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);

		//al_draw_tinted_bitmap(game->intro.table_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),-1*(game->intro.page)*al_get_display_width(game->display), al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)),0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->intro.table_bitmap);
	al_destroy_bitmap(game->intro.table);
	al_destroy_font(game->intro.font);
	al_destroy_sample(game->intro.sample);

}
