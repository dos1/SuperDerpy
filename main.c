#include <stdio.h>
#include "menu.h"
#include "loading.h"

float FPS = 60;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 500;
bool FULLSCREEN = true;

void PreloadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		Menu_Preload(game);
	}
}

void UnloadGameState(struct Game *game) {
	if (game->gamestate==GAMESTATE_MENU) {
		Menu_Unload(game);
	}
}

void LoadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		Menu_Load(game);
	}
	else if (game->loadstate==GAMESTATE_LOADING) {
		Loading_Load(game);
	}
	game->gamestate = game->loadstate;
	game->loadstate = -1;
}

int main(int argc, char **argv){
   srand(time(NULL));
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

   if (!al_reserve_samples(2)){
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
   game.font = al_load_ttf_font("ShadowsIntoLight.ttf",al_get_display_height(game.display)*0.09,0 );
   
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
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
      else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
	 if (game.gamestate==GAMESTATE_LOADING) {
		if (Loading_Keydown(&game, &ev)) break;
	 }
	 else if (game.gamestate==GAMESTATE_MENU) {
		if (Menu_Keydown(&game, &ev)) break;
	 }
      }
      
      if(redraw && al_is_event_queue_empty(game.event_queue)) {
         redraw = false;
	 if (game.gamestate==GAMESTATE_LOADING) {
		Loading_Draw(&game);
	 }
	 else if (game.gamestate==GAMESTATE_MENU) {
		Menu_Draw(&game);
	}
      }
   }

   UnloadGameState(&game);
   al_destroy_timer(game.timer);
   al_destroy_display(game.display);
   al_destroy_bitmap(game.loading.loading_bitmap);
   al_destroy_event_queue(game.event_queue);
   al_destroy_font(game.font);
   return 0;
}
