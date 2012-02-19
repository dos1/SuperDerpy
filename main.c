#include <stdio.h>
#include "menu.h"
#include "loading.h"
#include "about.h"
#include "intro.h"
#include "map.h"

float FPS = 60;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 500;
bool FULLSCREEN = true;
bool DEBUG_MODE = true;

void PrintConsole(struct Game *game, char* text) {
	if (DEBUG_MODE) printf("%s\n", text);
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*1.0);
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_text(game->font_console, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.005, al_get_bitmap_height(game->console)*0.8, ALLEGRO_ALIGN_LEFT, text);
	al_set_target_bitmap(game->console);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	//al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_bitmap(con, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(con);
}

void DrawConsole(struct Game *game) {
	if (game->showconsole) al_draw_bitmap(game->console, 0, 0, 0);
}

void PreloadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		PrintConsole(game, "Preload GAMESTATE_MENU...");
		Menu_Preload(game);
	}
	else if (game->loadstate==GAMESTATE_LOADING) {
		PrintConsole(game, "Preload GAMESTATE_LOADING...");
		Loading_Preload(game);
	}
	else if (game->loadstate==GAMESTATE_ABOUT) {
		PrintConsole(game, "Preload GAMESTATE_ABOUT...");
		About_Preload(game);
	}
	else if (game->loadstate==GAMESTATE_INTRO) {
		PrintConsole(game, "Preload GAMESTATE_INTRO...");
		Intro_Preload(game);
	}
	else if (game->loadstate==GAMESTATE_MAP) {
		PrintConsole(game, "Preload GAMESTATE_MAP...");
		Map_Preload(game);
	} else {
		PrintConsole(game, "ERROR: Attempted to preload unknown gamestate!");
	}
	PrintConsole(game, "finished");
}

void UnloadGameState(struct Game *game) {
	if (game->gamestate==GAMESTATE_MENU) {
		PrintConsole(game, "Unload GAMESTATE_MENU...");
		Menu_Unload(game);
	}
	else if (game->gamestate==GAMESTATE_LOADING) {
		PrintConsole(game, "Unload GAMESTATE_LOADING...");
		Loading_Unload(game);
	}
	else if (game->gamestate==GAMESTATE_ABOUT) {
		PrintConsole(game, "Unload GAMESTATE_ABOUT...");
		About_Unload(game);
	}
	else if (game->gamestate==GAMESTATE_INTRO) {
		PrintConsole(game, "Unload GAMESTATE_INTRO...");
		Intro_Unload(game);
	}
	else if (game->gamestate==GAMESTATE_MAP) {
		PrintConsole(game, "Unload GAMESTATE_MAP...");
		Map_Unload(game);
	} else {
		PrintConsole(game, "ERROR: Attempted to unload unknown gamestate!");
	}
	PrintConsole(game, "finished");
}

void LoadGameState(struct Game *game) {
	if (game->loadstate==GAMESTATE_MENU) {
		PrintConsole(game, "Load GAMESTATE_MENU...");
		Menu_Load(game);
	}
	else if (game->loadstate==GAMESTATE_LOADING) {
		PrintConsole(game, "Load GAMESTATE_LOADING...");
		Loading_Load(game);
	}
	else if (game->loadstate==GAMESTATE_ABOUT) {
		PrintConsole(game, "Load GAMESTATE_ABOUT...");
		About_Load(game);
	}
	else if (game->loadstate==GAMESTATE_INTRO) {
		PrintConsole(game, "Load GAMESTATE_INTRO...");
		Intro_Load(game);
	}
	else if (game->loadstate==GAMESTATE_MAP) {
		PrintConsole(game, "Load GAMESTATE_MAP...");
		Map_Load(game);
	} else {
		PrintConsole(game, "ERROR: Attempted to load unknown gamestate!");
	}
	PrintConsole(game, "finished");
	game->gamestate = game->loadstate;
	game->loadstate = -1;
}

int main(int argc, char **argv){
   srand(time(NULL));
   //ALLEGRO_DISPLAY_MODE disp_data;
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

   if(!al_init_acodec_addon()){
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
   }

   if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
   }

   if(!al_install_keyboard()){
      fprintf(stderr, "failed to initialize keyboard!\n");
      return -1;
   }

   if (!al_reserve_samples(10)){
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
   game.font = al_load_ttf_font("data/ShadowsIntoLight.ttf",al_get_display_height(game.display)*0.09,0 );
   game.font_console = al_load_ttf_font("data/DejaVuSansMono.ttf",al_get_display_height(game.display)*0.018,0 );
   
   game.event_queue = al_create_event_queue();
   if(!game.event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(game.display);
      return -1;
   }

   al_register_event_source(game.event_queue, al_get_display_event_source(game.display));
   al_register_event_source(game.event_queue, al_get_timer_event_source(game.timer));
   al_register_event_source(game.event_queue, al_get_keyboard_event_source());

   game.showconsole = DEBUG_MODE;
   game.console = al_create_bitmap(al_get_display_width(game.display), al_get_display_height(game.display)*0.12);
   al_set_target_bitmap(game.console);
   al_clear_to_color(al_map_rgba(0,0,0,80));
   al_set_target_bitmap(al_get_backbuffer(game.display));

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
	 if ((ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_TILDE)) {
		 game.showconsole = !game.showconsole;
	 }
	 if (game.gamestate==GAMESTATE_LOADING) {
		if (Loading_Keydown(&game, &ev)) break;
	 }
	 else if (game.gamestate==GAMESTATE_MENU) {
		if (Menu_Keydown(&game, &ev)) break;
	 }
	 else if (game.gamestate==GAMESTATE_ABOUT) {
		if (About_Keydown(&game, &ev)) break;
	 }
	 else if (game.gamestate==GAMESTATE_INTRO) {
		if (Intro_Keydown(&game, &ev)) break;
	 }
	 else if (game.gamestate==GAMESTATE_MAP) {
		if (Map_Keydown(&game, &ev)) break;
	 }
	else {
		game.showconsole = true;
		PrintConsole(&game, "ERROR: Keystroke in unknown gamestate! (5 sec sleep)");
		DrawConsole(&game);
		al_flip_display();
		al_rest(5.0);
		PrintConsole(&game, "Returning to menu...");
		game.gamestate = GAMESTATE_LOADING;
		game.loadstate = GAMESTATE_MENU;
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
	 else if (game.gamestate==GAMESTATE_ABOUT) {
		About_Draw(&game);
	}
	 else if (game.gamestate==GAMESTATE_INTRO) {
		Intro_Draw(&game);
	}
	 else if (game.gamestate==GAMESTATE_MAP) {
		Map_Draw(&game);
	}
	else {
		game.showconsole = true;
		PrintConsole(&game, "ERROR: Unknown gamestate reached! (5 sec sleep)");
		DrawConsole(&game);
		al_flip_display();
		al_rest(5.0);
		PrintConsole(&game, "Returning to menu...");
		game.gamestate = GAMESTATE_LOADING;
		game.loadstate = GAMESTATE_MENU;
	}
	DrawConsole(&game);
	al_flip_display();
      }
   }

   UnloadGameState(&game);
   al_rest(0.1);
   al_destroy_timer(game.timer);
   al_destroy_display(game.display);
   al_destroy_bitmap(game.loading.loading_bitmap);
   al_destroy_event_queue(game.event_queue);
   al_destroy_font(game.font);
   al_destroy_font(game.font_console);
   return 0;
}
