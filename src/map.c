#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "map.h"

void Map_Draw(struct Game *game) {
	al_draw_bitmap(game->map.map, 0, 0, 0);
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
	//if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
		return 0;
	//}
}

void Map_Preload(struct Game *game) {
	game->map.available = 6;
	game->map.selected = game->map.available;
	game->map.map = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	game->map.background = al_load_bitmap( "data/table.png" );
	game->map.map_bg = al_load_bitmap( "data/map/background.png" );
	char filename[30] = { };
	sprintf(filename, "data/map/highlight%d.png", game->map.available);
	game->map.highlight = al_load_bitmap( filename );
	game->map.sample = al_load_sample( "data/map.flac" );
	
	if (!game->map.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
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
	al_destroy_sample(game->map.sample);
}
