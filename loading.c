#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "loading.h"

void Loading_Draw(struct Game *game) {
		ALLEGRO_EVENT ev;
		for(int fadeloop=0; fadeloop<256; fadeloop+=10){
			al_wait_for_event(game->event_queue, &ev);
			al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		
		al_draw_bitmap(game->loading.loading_bitmap,0,0,0);
		al_flip_display();
		
		PreloadGameState(game);
		for(int fadeloop=255; fadeloop>0; fadeloop-=10){
			al_wait_for_event(game->event_queue, &ev);
			al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();
		//al_rest(0.2);
		LoadGameState(game);
}

void Loading_Load(struct Game *game) {
	game->loading.image = al_load_bitmap( "loading.png" );

	// Scale "Loading" bitmap
	game->loading.loading_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->loading.loading_bitmap);
	al_draw_scaled_bitmap(game->loading.image,0, 0, al_get_bitmap_width(game->loading.image), al_get_bitmap_height(game->loading.image), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.0234, al_get_display_height(game->display)*0.85, ALLEGRO_ALIGN_LEFT, "Loading...");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(game->loading.image);
}