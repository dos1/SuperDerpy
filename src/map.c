#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <math.h>
#include "map.h"

void Map_Draw(struct Game *game) {
	al_draw_bitmap(game->map.map, 0, 0, 0);
	float x,y;
	switch (game->map.selected) {
		case 1:
			x=0.18;
			y=0.225;
			break;
		case 2:
			x=0.5;
			y=0.3125;
			break;
		case 3:
			x=0.738;
			y=0.425;
			break;
		case 4:
			x=0.61;
			y=0.7625;
			break;
		case 5:
			x=0.17;
			y=0.725;
			break;
		case 6:
			x=0.41;
			y=0.518;
			break;
	}
	al_draw_scaled_bitmap(game->map.arrow, 0, 0, al_get_bitmap_width(game->map.arrow), al_get_bitmap_height(game->map.arrow), al_get_display_width(game->display)*x, al_get_display_height(game->display)*y + ((sin(game->map.arrowpos)+0.5)/20.0)*al_get_display_height(game->display), al_get_display_width(game->display)*0.1, al_get_display_height(game->display)*0.16, 0);
	game->map.arrowpos += 0.1;
}

void Map_Load(struct Game *game) {
	al_play_sample(game->map.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->map.map,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	Map_Draw(game);
}
int Map_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((((game->map.selected<4) || (game->map.selected==6)) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((game->map.selected>4) && (game->map.selected!=6) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((game->map.selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_UP)) || ((game->map.selected==6) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN))) {
		game->map.selected--;
		al_play_sample(game->map.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	} else if (((game->map.selected<3) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) || ((game->map.selected==4) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) || ((game->map.selected==3) && (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) || ((game->map.selected==5) && (ev->keyboard.keycode==ALLEGRO_KEY_UP))) {
		game->map.selected++;
		al_play_sample(game->map.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_LEFT) || (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) || (ev->keyboard.keycode==ALLEGRO_KEY_UP) || (ev->keyboard.keycode==ALLEGRO_KEY_DOWN)) {
		al_play_sample(game->map.click_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		PrintConsole(game, "Selecting level %d...", game->map.selected);
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_LEVEL;
		return 0;
	} else if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
		return 0;
	} else { return 0; }
	if (game->map.selected<1) game->map.selected=1;
	if (game->map.selected>game->map.available) game->map.selected=game->map.available;
	return 0;
}

void Map_Preload(struct Game *game) {
	game->map.available = 6;
	game->map.selected = game->map.available;
	PrintConsole(game, "Last level available: %d", game->map.selected);
	game->map.arrowpos = 0;
	game->map.map = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	game->map.background = al_load_bitmap( "data/table.png" );
	game->map.map_bg = al_load_bitmap( "data/map/background.png" );
	game->map.arrow = al_load_bitmap( "data/arrow.png" );
	game->map.click_sample = al_load_sample( "data/click.flac" );
	char filename[30] = { };
	sprintf(filename, "data/map/highlight%d.png", game->map.available);
	game->map.highlight = al_load_bitmap( filename );
	game->map.sample = al_load_sample( "data/map.flac" );
	
	if (!game->map.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	if (!game->map.click_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	al_set_target_bitmap(game->map.map);
	al_draw_scaled_bitmap(game->map.background,0,0,al_get_bitmap_width(game->map.background),al_get_bitmap_height(game->map.background),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_scaled_bitmap(game->map.map_bg,0,0,al_get_bitmap_width(game->map.map_bg),al_get_bitmap_height(game->map.map_bg),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_scaled_bitmap(game->map.highlight,0,0,al_get_bitmap_width(game->map.highlight),al_get_bitmap_height(game->map.highlight),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_set_target_bitmap(al_get_backbuffer(game->display));	
}
void Map_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->map.map, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->map.map);
	al_destroy_bitmap(game->map.background);
	al_destroy_bitmap(game->map.map_bg);
	al_destroy_bitmap(game->map.highlight);
	al_destroy_bitmap(game->map.arrow);
	al_destroy_sample(game->map.sample);
	al_destroy_sample(game->map.click_sample);
}
