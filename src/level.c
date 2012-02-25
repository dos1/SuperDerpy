#include <stdio.h>
#include "level.h"

void Level_Draw(struct Game *game) {
	al_set_target_bitmap(game->level.derpytmp);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(game->level.derpy_walkcycle,500*(game->level.derpy_frame%6),400*(game->level.derpy_frame/6),500,400,0,0,0);
	al_set_target_bitmap(game->level.derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->level.derpytmp, 0, 0, al_get_bitmap_width(game->level.derpytmp),al_get_bitmap_height(game->level.derpytmp), 0, 0, al_get_bitmap_width(game->level.derpy),al_get_bitmap_height(game->level.derpy), 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	game->level.derpy_pos=game->level.derpy_pos+0.00092;
	if (game->level.derpy_pos>1) game->level.derpy_pos=-0.2;
	game->level.derpy_frame_tmp++;
	if (game->level.derpy_frame_tmp%3==0) {
		game->level.derpy_frame++;
		if (game->level.derpy_frame==24) game->level.derpy_frame=0;
	}
	al_draw_scaled_bitmap(game->level.image,0,0,al_get_bitmap_width(game->level.image),al_get_bitmap_height(game->level.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap(game->level.derpy, game->level.derpy_pos*al_get_display_width(game->display), al_get_display_height(game->display)-al_get_bitmap_height(game->level.derpy), 0);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
}

void Level_Load(struct Game *game) {
	al_play_sample(game->level.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.fade_bitmap);
	Level_Draw(game);
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
	}
	return 0;
}

void Level_Preload(struct Game *game) {
	game->level.derpy_frame = 0;
	game->level.derpy_frame_tmp = 0;
	game->level.derpy_pos = -0.2;
	PrintConsole(game, "Initializing level %d...", game->level.current_level);
	game->level.image = al_load_bitmap( "data/table.png" );
	game->level.sample = al_load_sample( "data/about.flac" );
	game->level.derpy_walkcycle = al_load_bitmap( "data/derpcycle.png" );
	
	if (!game->level.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	game->level.derpy = al_create_bitmap(al_get_display_width(game->display)*0.1953125, al_get_display_height(game->display)*0.25);
	game->level.derpytmp = al_create_bitmap(500, 400);
	
	game->level.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.fade_bitmap);
	al_draw_scaled_bitmap(game->level.image,0,0,al_get_bitmap_width(game->level.image),al_get_bitmap_height(game->level.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_set_target_bitmap(al_get_backbuffer(game->display));
}

void Level_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	game->level.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.fade_bitmap);
	al_draw_scaled_bitmap(game->level.image,0,0,al_get_bitmap_width(game->level.image),al_get_bitmap_height(game->level.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.fade_bitmap, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.image);
	al_destroy_bitmap(game->level.fade_bitmap);
	al_destroy_sample(game->level.sample);
}
