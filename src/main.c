#include <stdio.h>
#include "menu.h"
#include "loading.h"
#include "about.h"
#include "intro.h"
#include "map.h"
#include "level.h"
#include "config.h"

#define PRELOAD_STATE(state, name) case state:\
	PrintConsole(game, "Preload %s...", #state); DrawConsole(game); al_flip_display(); name ## _Preload(game); break;
#define UNLOAD_STATE(state, name) case state:\
	PrintConsole(game, "Unload %s...", #state); name ## _Unload(game); break;
#define LOAD_STATE(state, name) case state:\
	PrintConsole(game, "Load %s...", #state); name ## _Load(game); break;
#define KEYDOWN_STATE(state, name) else if (game.gamestate==state) { if (name ## _Keydown(&game, &ev)) break; }
#define DRAW_STATE(state, name) case state:\
	name ## _Draw(&game); break;

float FPS = 60;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 500;
bool FULLSCREEN = true;
bool DEBUG_MODE = true;

void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text) {
	al_draw_text(font, al_map_rgba(0,0,0,128), x+1, y+1, flags, text);
	al_draw_text(font, color, x, y, flags, text);
}

void PrintConsole(struct Game *game, char* format, ...) {
	va_list vl;
	va_start(vl, format);
	char text[255] = {};
	vsprintf(text, format, vl);
	va_end(vl);
	if (DEBUG_MODE) printf("%s\n", text);
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->console), al_get_bitmap_height(game->console));
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_text(game->font_console, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.005, al_get_bitmap_height(game->console)*0.81, ALLEGRO_ALIGN_LEFT, text);
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
	switch (game->loadstate) {
		PRELOAD_STATE(GAMESTATE_MENU, Menu)
		PRELOAD_STATE(GAMESTATE_LOADING, Loading)
		PRELOAD_STATE(GAMESTATE_ABOUT, About)
		PRELOAD_STATE(GAMESTATE_INTRO, Intro)
		PRELOAD_STATE(GAMESTATE_MAP, Map)
		PRELOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to preload unknown gamestate %d!", game->loadstate);
			break;
	}
	PrintConsole(game, "finished");
}

void UnloadGameState(struct Game *game) {
	switch (game->gamestate) {
		UNLOAD_STATE(GAMESTATE_MENU, Menu)
		UNLOAD_STATE(GAMESTATE_LOADING, Loading)
		UNLOAD_STATE(GAMESTATE_ABOUT, About)
		UNLOAD_STATE(GAMESTATE_INTRO, Intro)
		UNLOAD_STATE(GAMESTATE_MAP, Map)
		UNLOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to unload unknown gamestate %d!", game->gamestate);
			break;
	}
	PrintConsole(game, "finished");
}

void LoadGameState(struct Game *game) {
	switch (game->loadstate) {
		LOAD_STATE(GAMESTATE_MENU, Menu)
		LOAD_STATE(GAMESTATE_LOADING, Loading)
		LOAD_STATE(GAMESTATE_ABOUT, About)
		LOAD_STATE(GAMESTATE_INTRO, Intro)
		LOAD_STATE(GAMESTATE_MAP, Map)
		LOAD_STATE(GAMESTATE_LEVEL, Level)
		default:
			PrintConsole(game, "ERROR: Attempted to load unknown gamestate %d!", game->loadstate);
	}
	PrintConsole(game, "finished");
	game->gamestate = game->loadstate;
	game->loadstate = -1;
}

int main(int argc, char **argv){
	srand(time(NULL));
   
	InitConfig();
   
	bool redraw = true;

	struct Game game;

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR^ALLEGRO_MAG_LINEAR);
   
	game.timer = al_create_timer(ALLEGRO_BPS_TO_SECS(FPS));
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
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	game.display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if(!game.display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	al_set_window_title(game.display, "Super Derpy: Muffin Attack");
	if (FULLSCREEN) al_hide_mouse_cursor(game.display);
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
	while(1) {
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
			KEYDOWN_STATE(GAMESTATE_MENU, Menu)
			KEYDOWN_STATE(GAMESTATE_LOADING, Loading)
			KEYDOWN_STATE(GAMESTATE_ABOUT, About)
			KEYDOWN_STATE(GAMESTATE_INTRO, Intro)
			KEYDOWN_STATE(GAMESTATE_MAP, Map)
			KEYDOWN_STATE(GAMESTATE_LEVEL, Level)
			else {
				game.showconsole = true;
				PrintConsole(&game, "ERROR: Keystroke in unknown (%d) gamestate! (5 sec sleep)", game.gamestate);
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
			switch (game.gamestate) {
				DRAW_STATE(GAMESTATE_MENU, Menu)
				DRAW_STATE(GAMESTATE_LOADING, Loading)
				DRAW_STATE(GAMESTATE_ABOUT, About)
				DRAW_STATE(GAMESTATE_INTRO, Intro)
				DRAW_STATE(GAMESTATE_MAP, Map)
				DRAW_STATE(GAMESTATE_LEVEL, Level)
				default:
					game.showconsole = true;
					PrintConsole(&game, "ERROR: Unknown gamestate %d reached! (5 sec sleep)", game.gamestate);
					DrawConsole(&game);
					al_flip_display();
					al_rest(5.0);
					PrintConsole(&game, "Returning to menu...");
					game.gamestate = GAMESTATE_LOADING;
					game.loadstate = GAMESTATE_MENU;
					break;
			}
			DrawConsole(&game);
			al_flip_display();
		}
	}
	UnloadGameState(&game);
	if (game.gamestate != GAMESTATE_LOADING) {
		game.gamestate = GAMESTATE_LOADING;
		UnloadGameState(&game);
	}
	al_clear_to_color(al_map_rgb(0,0,0));
	PrintConsole(&game, "Shutting down...");
	DrawConsole(&game);
	al_flip_display();
	al_rest(0.1);
	al_destroy_timer(game.timer);
	al_destroy_display(game.display);
	al_destroy_bitmap(game.loading.loading_bitmap);
	al_destroy_event_queue(game.event_queue);
	al_destroy_font(game.font);
	al_destroy_font(game.font_console);
	al_uninstall_audio();
	DeinitConfig();
	return 0;
}
