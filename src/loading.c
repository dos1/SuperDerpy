/*! \file loading.c
 *  \brief Loading screen.
 */

#include "loading.h"

void Loading_Draw(struct Game *game) {
	ALLEGRO_EVENT ev;
	int fadeloop;
	for(fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}

	al_draw_bitmap(game->loading.loading_bitmap,0,0,0);

	al_stop_timer(game->timer);

	PreloadGameState(game);

	al_start_timer(game->timer);

	for(fadeloop=255; fadeloop>0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_clear_to_color(al_map_rgb(0,0,0));
	DrawConsole(game);
	al_flip_display();
	//al_rest(0.2);
	LoadGameState(game);
}

void Loading_Load(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.0234, al_get_display_height(game->display)*0.85, ALLEGRO_ALIGN_LEFT, "Loading...");
	DrawConsole(game);
	al_flip_display();

	game->loading.loading_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->loading.loading_bitmap);

	void GenerateLoadingBitmap() {
		al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
		game->loading.image = al_load_bitmap( "data/loading.png" );
		al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

		int width = al_get_display_width(game->display);
		int height = al_get_display_height(game->display);
		int x, y;
		for (y = 0; y < height; y++) {
			float pixy = ((float)y / height) * al_get_bitmap_height(game->loading.image);
			for (x = 0; x < width; x++) {
				float pixx = ((float)x / width) * al_get_bitmap_width(game->loading.image);
				ALLEGRO_COLOR a = al_get_pixel(game->loading.image, pixx-0.25, pixy-0.25);
				ALLEGRO_COLOR b = al_get_pixel(game->loading.image, pixx+0.25, pixy-0.25);
				ALLEGRO_COLOR c = al_get_pixel(game->loading.image, pixx-0.25, pixy+0.25);
				ALLEGRO_COLOR d = al_get_pixel(game->loading.image, pixx+0.25, pixy+0.25);
				ALLEGRO_COLOR result = al_map_rgba_f(
					(a.r+b.r+c.r+d.r) / 4,
					(a.g+b.b+c.g+d.g) / 4,
					(a.b+b.g+c.b+d.b) / 4,
					(a.a+b.a+c.a+d.a) / 4
				);
				al_put_pixel(x, y, result);
			}
		}
		al_save_bitmap("data/cache/loading.png", game->loading.loading_bitmap);
		PrintConsole(game, "Cache bitmap generated.");
	}
	
	game->loading.image = al_load_bitmap( "data/cache/loading.png");
	if (game->loading.image) {
		if ((al_get_bitmap_width(game->loading.image)!=al_get_display_width(game->display)) || (al_get_bitmap_height(game->loading.image)!=al_get_display_height(game->display)))
			GenerateLoadingBitmap();
		else al_draw_bitmap(game->loading.image, 0, 0, 0);
	} else GenerateLoadingBitmap();
	
	// Scale "Loading" bitmap
	al_set_target_bitmap(game->loading.loading_bitmap);
	//al_draw_scaled_bitmap(game->loading.image,0, 0, al_get_bitmap_width(game->loading.image), al_get_bitmap_height(game->loading.image), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.0234, al_get_display_height(game->display)*0.85, ALLEGRO_ALIGN_LEFT, "Loading...");
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(game->loading.image);
}

int Loading_Keydown(struct Game *game, ALLEGRO_EVENT *ev) { return 0; }
void Loading_Preload(struct Game *game) {}
void Loading_Unload(struct Game *game) { al_destroy_bitmap(game->loading.loading_bitmap); }
