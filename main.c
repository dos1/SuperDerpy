#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>

float FPS = 40;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 500;
bool FULLSCREEN = true;

enum gamestate_enum {
	GAMESTATE_LOADING,
	GAMESTATE_MENU
};

struct Menu {
	ALLEGRO_BITMAP *menu_bitmap, *menu_fade_bitmap, *image;
	ALLEGRO_BITMAP *cloud_bitmap, *cloud;
	ALLEGRO_BITMAP *pinkcloud_bitmap, *pinkcloud;
	float cloud_position;
	ALLEGRO_SAMPLE *sample;
};

struct Loading {
	ALLEGRO_BITMAP *loading_bitmap, *image;
};

struct Game {
	ALLEGRO_DISPLAY *display;
	ALLEGRO_FONT *font;
	enum gamestate_enum gamestate;
	enum gamestate_enum loadstate;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	struct Menu menu;
	struct Loading loading;
};

void PreloadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		game->menu.cloud_position = 100;
		game->menu.image = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));  //FIXME: WHYYYYYYY?
		game->menu.image = al_load_bitmap( "menu.png" );
		
		game->menu.sample = al_load_sample( "menu.wav" );
		game->menu.cloud = al_load_bitmap( "cloud.png" );
		game->menu.pinkcloud = al_load_bitmap( "pinkcloud.png" );
		
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
		
		game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)*0.8122);
		al_set_target_bitmap(game->menu.pinkcloud_bitmap);
		al_draw_scaled_bitmap(game->menu.pinkcloud,0, 0, al_get_bitmap_width(game->menu.pinkcloud), al_get_bitmap_height(game->menu.pinkcloud), 0, 0, al_get_bitmap_width(game->menu.pinkcloud_bitmap), al_get_bitmap_height(game->menu.pinkcloud_bitmap),0); 
		al_destroy_bitmap(game->menu.pinkcloud);

		al_set_target_bitmap(game->menu.menu_fade_bitmap);
		al_draw_bitmap(game->menu.pinkcloud_bitmap,al_get_display_width(game->display)*0.132, 0,0);
		al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");

		al_set_target_bitmap(al_get_backbuffer(game->display));

	}
}

void UnloadGameState(struct Game *game) {
	if (game->gamestate==GAMESTATE_MENU) {
		al_destroy_sample(game->menu.sample);
		for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
			al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		al_destroy_bitmap(game->menu.menu_fade_bitmap);
	}
}

void LoadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		al_play_sample(game->menu.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		for(int fadeloop=0; fadeloop<256; fadeloop+=10){
			al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
	}
	else if (game->loadstate==GAMESTATE_LOADING) {
		game->loading.image = al_load_bitmap( "loading.png" );

		// Scale "Loading" bitmap
		game->loading.loading_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
		al_set_target_bitmap(game->loading.loading_bitmap);
		al_draw_scaled_bitmap(game->loading.image,0, 0, al_get_bitmap_width(game->loading.image), al_get_bitmap_height(game->loading.image), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display),0);
		al_draw_text(game->font, al_map_rgb(255,255,255), 30, al_get_display_height(game->display)-120, ALLEGRO_ALIGN_LEFT, "Loading...");
		al_set_target_bitmap(al_get_backbuffer(game->display));
		al_destroy_bitmap(game->loading.image);
	}
	game->gamestate = game->loadstate;
	game->loadstate = -1;
}

int main(int argc, char **argv){
   ALLEGRO_DISPLAY_MODE disp_data;
   bool redraw = true;

   struct Game game;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
   
   game.timer = al_create_timer(1.0 / FPS);
   if(!game.timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }

   if(!al_init_image_addon()) {
      fprintf(stderr, "failed to initialize image addon!\n");
      //al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
      //                           NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
   }

   if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
   }

   if(!al_install_keyboard()){
      fprintf(stderr, "failed to initialize keyboard!\n");
      return -1;
   }
   
   if(!al_init_acodec_addon()){
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
   }

   if (!al_reserve_samples(1)){
      fprintf(stderr, "failed to reserve samples!\n");
      return -1;
   }
   
   al_init_font_addon();

   if(!al_init_ttf_addon()){
      fprintf(stderr, "failed to initialize fonts!\n");
      return -1;
   }

   if (FULLSCREEN) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   game.display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
   if(!game.display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
   al_set_window_title(game.display, "Super Derpy: Muffin Attack");
   al_hide_mouse_cursor(game.display);
   game.font = al_load_ttf_font("ShadowsIntoLight.ttf",72,0 );
   
   game.event_queue = al_create_event_queue();
   if(!game.event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(game.display);
      return -1;
   }

   al_register_event_source(game.event_queue, al_get_display_event_source(game.display));
   al_register_event_source(game.event_queue, al_get_timer_event_source(game.timer));
   al_register_event_source(game.event_queue, al_get_keyboard_event_source());

   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

   al_start_timer(game.timer);
   
   game.loadstate = GAMESTATE_LOADING;
   PreloadGameState(&game);
   LoadGameState(&game);
   game.loadstate = GAMESTATE_MENU;
   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(game.event_queue, &ev);

      if(ev.type == ALLEGRO_EVENT_TIMER) {
         redraw = true;
      }
      else if((ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) || (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
         break;
      }
      
      if(redraw && al_is_event_queue_empty(game.event_queue)) {
         redraw = false;
	 if (game.gamestate==GAMESTATE_LOADING) {
		for(int fadeloop=0; fadeloop<256; fadeloop+=10){
			al_draw_tinted_bitmap(game.loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		
		al_draw_bitmap(game.loading.loading_bitmap,0,0,0);
		al_flip_display();
		
		PreloadGameState(&game);
		for(int fadeloop=255; fadeloop>0; fadeloop-=10){
			al_draw_tinted_bitmap(game.loading.loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();
		//al_rest(0.2);
		LoadGameState(&game);
	 }
	 else if (game.gamestate==GAMESTATE_MENU) {
		al_clear_to_color(al_map_rgb(183,234,193));
		al_draw_scaled_bitmap(game.menu.cloud_bitmap,0,0,al_get_bitmap_width(game.menu.cloud_bitmap), al_get_bitmap_height(game.menu.cloud_bitmap), al_get_display_width(game.display)*(game.menu.cloud_position-20)/(-75), al_get_display_height(game.display)*0.15, al_get_bitmap_width(game.menu.cloud_bitmap)/2, al_get_bitmap_height(game.menu.cloud_bitmap)/2,0);
		al_draw_bitmap(game.menu.menu_bitmap,0, 0,0);
		al_draw_bitmap(game.menu.pinkcloud_bitmap,(al_get_display_width(game.display)*0.132) + (cos((game.menu.cloud_position/50+80)*1.74444))*20, 0,0);
		al_draw_bitmap(game.menu.cloud_bitmap,al_get_display_width(game.display)*game.menu.cloud_position/100, 30,0);
		al_draw_text(game.font, al_map_rgb(255,255,255), al_get_display_width(game.display)*0.5, al_get_display_height(game.display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
		game.menu.cloud_position-=0.2;
		if (game.menu.cloud_position<-80) game.menu.cloud_position=100;
		al_flip_display();
	}
      }
   }

   UnloadGameState(&game);
   al_destroy_timer(game.timer);
   al_destroy_display(game.display);
   al_destroy_bitmap(game.loading.loading_bitmap);
   al_destroy_event_queue(game.event_queue);
   return 0;
}
