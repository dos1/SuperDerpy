/*! \file level.c
 *  \brief Playable Level code.
 */

#include <stdio.h>
#include <math.h>
#include "pause.h"
#include "level.h"

void Level_Draw(struct Game *game) {
	if (!al_get_sample_instance_playing(game->level.music) && (game->loadstate==GAMESTATE_LEVEL)) { 
		al_set_sample_instance_playing(game->level.music, true);
		al_set_sample_instance_position(game->level.music, game->level.music_pos);
	}
	al_set_target_bitmap(game->level.derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(game->level.derpy_walkcycle,al_get_bitmap_width(game->level.derpy)*(game->level.derpy_frame%6),al_get_bitmap_height(game->level.derpy)*(game->level.derpy_frame/6),al_get_bitmap_width(game->level.derpy), al_get_bitmap_height(game->level.derpy),0,0,0);
	//al_set_target_bitmap(game->level.derpy);
	//al_clear_to_color(al_map_rgba(0,0,0,0));
	//al_draw_scaled_bitmap(game->level.derpytmp, 0, 0, al_get_bitmap_width(game->level.derpytmp),al_get_bitmap_height(game->level.derpytmp), 0, 0, al_get_bitmap_width(game->level.derpy),al_get_bitmap_height(game->level.derpy), 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	game->level.derpy_pos=game->level.derpy_pos+tps(game, 60*0.00092);
	if (game->level.derpy_pos>1) { UnloadGameState(game);
		game->loadstate = GAMESTATE_MENU;
		LoadGameState(game); return; }
	int i;
	for (i = 0; i < tps(game, 60); i++ ) {
	game->level.derpy_frame_tmp++;
	if (game->level.derpy_frame_tmp%3==0) {
		if (game->level.derpy_frame_tmp%5==0) game->level.derpy_frame++;
		game->level.derpy_frame++;
		if (game->level.derpy_frame>=24) game->level.derpy_frame=0;
	}
	}
	al_draw_scaled_bitmap(game->level.image,0,0,al_get_bitmap_width(game->level.image),al_get_bitmap_height(game->level.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap(game->level.derpy, game->level.derpy_pos*al_get_display_width(game->display), al_get_display_height(game->display)-al_get_bitmap_height(game->level.derpy), ALLEGRO_FLIP_HORIZONTAL);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
}

void Level_Load(struct Game *game) {
	if (game->music) al_play_sample_instance(game->level.music);
	//al_play_sample(game->level.sample, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	int fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.fade_bitmap);
	Level_Draw(game);
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		game->level.music_pos = al_get_sample_instance_position(game->level.music);
		al_set_sample_instance_playing(game->level.music, false);
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
		Pause_Load(game);
	}
	return 0;
}

void Level_Preload(struct Game *game) {
	game->level.derpy_frame = 0;
	game->level.derpy_frame_tmp = 0;
	game->level.derpy_pos = -0.2;
	PrintConsole(game, "Initializing level %d...", game->level.current_level);
	//game->level.image = al_load_bitmap( "data/table.png" );
	game->level.image =LoadFromCache(game, "table.png", al_get_display_width(game->display), al_get_display_height(game->display));
	game->level.sample = al_load_sample( "data/moonwalk.flac" );
	//game->level.derpy_walkcycle = al_load_bitmap( "data/derpcycle.png" );
	game->level.derpy_walkcycle =LoadFromCache(game, "derpcycle.png", al_get_display_width(game->display)*0.1953125*6, al_get_display_height(game->display)*0.25*4);
	
	if (!game->level.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	
	game->level.music = al_create_sample_instance(game->level.sample);
	al_attach_sample_instance_to_mixer(game->level.music, al_get_default_mixer());
	al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

	game->level.derpy = al_create_bitmap(al_get_display_width(game->display)*0.1953125, al_get_display_height(game->display)*0.25);
	//game->level.derpytmp = al_create_bitmap(500, 400);
	
	game->level.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.fade_bitmap);
	al_draw_bitmap(game->level.image,0,0,0);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	Pause_Preload(game);
}

void Level_Unload(struct Game *game) {
	Pause_Unload_Real(game);
	ALLEGRO_EVENT ev;
	game->level.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->level.fade_bitmap);
	al_draw_scaled_bitmap(game->level.image,0,0,al_get_bitmap_width(game->level.image),al_get_bitmap_height(game->level.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_bitmap(game->level.derpy, game->level.derpy_pos*al_get_display_width(game->display), al_get_display_height(game->display)-al_get_bitmap_height(game->level.derpy), ALLEGRO_FLIP_HORIZONTAL);
	al_draw_textf(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2.2, ALLEGRO_ALIGN_CENTRE, "Level %d: Not implemented yet!", game->level.current_level);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/1.8, ALLEGRO_ALIGN_CENTRE, "Have some moonwalk instead.");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	int fadeloop;
	for(fadeloop=255; fadeloop>=0; fadeloop-=tps(game, 600)){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->level.fade_bitmap, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->level.image);
	al_destroy_bitmap(game->level.derpy);
	al_destroy_bitmap(game->level.derpy_walkcycle);
	al_destroy_bitmap(game->level.fade_bitmap);
	al_destroy_sample(game->level.sample);
}
