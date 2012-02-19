#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "about.h"

void About_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");
}

void About_Load(struct Game *game) {
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->about.fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->about.fade_bitmap);
}
int About_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	//if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
		return 0;
	//}
}
void About_Preload(struct Game *game) {
	game->about.image = al_load_bitmap( "data/table.png" );
	game->about.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->about.fade_bitmap);
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");	
	al_set_target_bitmap(al_get_backbuffer(game->display));
}
void About_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	game->about.fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->about.fade_bitmap);
	al_draw_scaled_bitmap(game->about.image,0,0,al_get_bitmap_width(game->about.image),al_get_bitmap_height(game->about.image),0,0,al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");	
	al_set_target_bitmap(al_get_backbuffer(game->display));
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->about.fade_bitmap, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), 0, 0, 0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->about.image);
	al_destroy_bitmap(game->about.fade_bitmap);
}
