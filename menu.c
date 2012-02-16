#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <math.h>
#include "menu.h"

void Menu_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(183,234,193));
	float tint = (sin((game->menu.cloud_position-30)/15)+1)/2;
	if (tint < 0.0001) game->menu.mountain_position = al_get_display_width(game->display)*(rand()/(float)RAND_MAX);
	al_draw_tinted_bitmap(game->menu.mountain_bitmap,al_map_rgba_f(tint,tint,tint,tint),game->menu.mountain_position, 0,0);
	al_draw_scaled_bitmap(game->menu.cloud_bitmap,0,0,al_get_bitmap_width(game->menu.cloud_bitmap), al_get_bitmap_height(game->menu.cloud_bitmap), al_get_display_width(game->display)*(sin((game->menu.cloud_position/40)-4.5)-0.3), al_get_display_height(game->display)*0.35, al_get_bitmap_width(game->menu.cloud_bitmap)/2, al_get_bitmap_height(game->menu.cloud_bitmap)/2,0);
	al_draw_bitmap(game->menu.cloud2_bitmap,al_get_display_width(game->display)*(game->menu.cloud2_position/100.0), al_get_display_height(game->display)/1.5,0);
	al_draw_bitmap(game->menu.menu_bitmap,0, 0,0);
	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.132) + (cos((game->menu.cloud_position/50+80)*1.74444))*20, 0,0);
	al_draw_bitmap(game->menu.cloud_bitmap,al_get_display_width(game->display)*game->menu.cloud_position/100, 30,0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	game->menu.cloud_position-=0.1;
	game->menu.cloud2_position-=0.025;
	if (game->menu.cloud_position<-80) game->menu.cloud_position=100;
	if (game->menu.cloud2_position<0) game->menu.cloud2_position=100;
	al_flip_display();
}

void Menu_Preload(struct Game *game) {
	game->menu.cloud_position = 100;
	game->menu.cloud2_position = 100;
	game->menu.image = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));  //FIXME: WHYYYYYYY?
	game->menu.image = al_load_bitmap( "menu.png" );
	game->menu.mountain = al_load_bitmap( "mountain.png" );
	game->menu.sample = al_load_sample( "menu.wav" );
	game->menu.cloud = al_load_bitmap( "cloud.png" );
	game->menu.cloud2 = al_load_bitmap( "cloud2.png" );
	game->menu.pinkcloud = al_load_bitmap( "pinkcloud.png" );
	game->menu.mountain_position = al_get_display_width(game->display)*0.7;
	
	if (!game->menu.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	// Scale menu bitmap
	game->menu.menu_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_bitmap);
	al_draw_scaled_bitmap(game->menu.image,0, 0, al_get_bitmap_width(game->menu.image), al_get_bitmap_height(game->menu.image), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_destroy_bitmap(game->menu.image);

	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_clear_to_color(al_map_rgb(183,234,193));
	al_draw_bitmap(game->menu.menu_bitmap,0, 0, 0);

	// Cloud menu bitmap
	game->menu.cloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.25);
	al_set_target_bitmap(game->menu.cloud_bitmap);
	al_draw_scaled_bitmap(game->menu.cloud,0, 0, al_get_bitmap_width(game->menu.cloud), al_get_bitmap_height(game->menu.cloud), 0, 0, al_get_bitmap_width(game->menu.cloud_bitmap), al_get_bitmap_height(game->menu.cloud_bitmap),0);
	al_destroy_bitmap(game->menu.cloud);

	game->menu.cloud2_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.2, al_get_display_height(game->display)*0.1);
	al_set_target_bitmap(game->menu.cloud2_bitmap);
	al_draw_scaled_bitmap(game->menu.cloud2,0, 0, al_get_bitmap_width(game->menu.cloud2), al_get_bitmap_height(game->menu.cloud2), 0, 0, al_get_bitmap_width(game->menu.cloud2_bitmap), al_get_bitmap_height(game->menu.cloud2_bitmap),0);
	al_destroy_bitmap(game->menu.cloud2);
	
	game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)*0.8122);
	al_set_target_bitmap(game->menu.pinkcloud_bitmap);
	al_draw_scaled_bitmap(game->menu.pinkcloud,0, 0, al_get_bitmap_width(game->menu.pinkcloud), al_get_bitmap_height(game->menu.pinkcloud), 0, 0, al_get_bitmap_width(game->menu.pinkcloud_bitmap), al_get_bitmap_height(game->menu.pinkcloud_bitmap),0); 
	al_destroy_bitmap(game->menu.pinkcloud);

	game->menu.mountain_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.055, al_get_display_height(game->display)/9);
	al_set_target_bitmap(game->menu.mountain_bitmap);
	al_draw_scaled_bitmap(game->menu.mountain,0, 0, al_get_bitmap_width(game->menu.mountain), al_get_bitmap_height(game->menu.mountain), 0, 0, al_get_bitmap_width(game->menu.mountain_bitmap), al_get_bitmap_height(game->menu.mountain_bitmap),0);
	al_destroy_bitmap(game->menu.mountain);

	al_set_target_bitmap(game->menu.menu_fade_bitmap);
	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.132) + (cos((82)*1.74444))*20, 0,0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");

	al_set_target_bitmap(al_get_backbuffer(game->display));	
}

void Menu_Unload(struct Game *game) {
	al_destroy_sample(game->menu.sample);
	ALLEGRO_EVENT ev;
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		al_flip_display();
	}
	al_destroy_bitmap(game->menu.menu_fade_bitmap);	
}

void Menu_Load(struct Game *game) {
	al_play_sample(game->menu.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		al_flip_display();
	}	
}