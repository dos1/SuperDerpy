/*! \file main.c * \brief Main file of SuperDerpy engine.
 *
 *   Contains basic functions shared by all views.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Also, ponies.
 */
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <locale.h>
#include <signal.h>
#include "gamestates/menu.h"
#include "gamestates/loading.h"
#include "gamestates/about.h"
#include "gamestates/intro.h"
#include "gamestates/map.h"
#include "gamestates/level.h"
#include "gamestates/pause.h"
#include "gamestates/disclaimer.h"
#include "config.h"

/*! \brief Macro for preloading gamestate.
 *
 *  Preloading of state happens when loading screen is displayed.
 */
#define PRELOAD_STATE(state, name) case state:\
	PrintConsole(game, "Preload %s...", #state); DrawConsole(game); al_flip_display(); name ## _Preload(game, progress); break;
/*! \brief Macro for unloading gamestate.
 *
 *  Unloading of state happens after it's fadeout.
 */
#define UNLOAD_STATE(state, name) case state:\
	PrintConsole(game, "Unload %s...", #state); name ## _Unload(game); break;
/*! \brief Macro for loading gamestate.
 *
 *  Loading of state means setting it as active and running it.
 */
#define LOAD_STATE(state, name) case state:\
	PrintConsole(game, "Load %s...", #state); name ## _Load(game); break;
/*! \brief Macro for sending keydown events to gamestate. */
#define KEYDOWN_STATE(state, name) else if (game.gamestate==state) { if (name ## _Keydown(&game, &ev)) break; }
/*! \brief Macro for drawing active gamestate. */
#define DRAW_STATE(state, name) case state:\
	name ## _Draw(game); break;
/*! \brief Macro for invoking logic function of active gamestate. */
#define LOGIC_STATE(state, name) case state:\
	name ## _Logic(game); break;
/*! \brief Macro for invoking pause function of active gamestate. */
#define PAUSE_STATE(state, name) case state:\
	PrintConsole(game, "Pause %s...", #state); name ## _Pause(game); break;
/*! \brief Macro for invoking resume function of active gamestate. */
#define RESUME_STATE(state, name) case state:\
	PrintConsole(game, "Resume %s...", #state); name ## _Resume(game); break;

double old_time = 0, fps;
int frames_done = 0;
bool memoryscale;

char* GetDataFilePath(char* filename) {

	char *result = 0;

	if (al_filename_exists(filename)) {
		return strdup(filename);
	}

	char origfn[255] = "data/";
	strcat(origfn, filename);

	if (al_filename_exists(origfn)) {
		return strdup(origfn);
	}

	void TestPath(char* subpath) {
		ALLEGRO_PATH *tail = al_create_path(filename);
		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		ALLEGRO_PATH *data = al_create_path(subpath);
		al_join_paths(path, data);
		al_join_paths(path, tail);
		//printf("Testing for %s\n", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		if (al_filename_exists(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
			result = strdup(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		}
		al_destroy_path(tail);
		al_destroy_path(data);
		al_destroy_path(path);
	}
	TestPath("../share/superderpy/data/");
	TestPath("../data/");
	TestPath("../Resources/data/");
	TestPath("data/");

	if (!result) {
		printf("FATAL: Could not find data file: %s!\n", filename);
		exit(1);
	}
	return result;
}

void PrintConsole(struct Game *game, char* format, ...) {
	va_list vl;
	va_start(vl, format);
	char text[255] = {};
	vsprintf(text, format, vl);
	va_end(vl);
	if (game->debug) { printf("%s\n", text); fflush(stdout); }
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->console), al_get_bitmap_height(game->console));
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->console, 0, al_get_bitmap_height(game->console)*0.2, al_get_bitmap_width(game->console), al_get_bitmap_height(game->console)*0.8, 0, 0, 0);
	al_draw_text(game->font_console, al_map_rgb(255,255,255), game->viewportWidth*0.005, al_get_bitmap_height(game->console)*0.81, ALLEGRO_ALIGN_LEFT, text);
	al_set_target_bitmap(game->console);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap(con, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(con);
}

void DrawConsole(struct Game *game) {
	if (game->showconsole) {
		al_draw_bitmap(game->console, 0, 0, 0);
		double game_time = al_get_time();
		if(game_time - old_time >= 1.0) {
			fps = frames_done / (game_time - old_time);
			frames_done = 0;
			old_time = game_time;
		}
		char sfps[6] = { };
		sprintf(sfps, "%.0f", fps);
		al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255), game->viewportWidth*0.99, 0, ALLEGRO_ALIGN_RIGHT, sfps);
	}
	frames_done++;
}

void PreloadGameState(struct Game *game, void (*progress)(struct Game*, float)) {
	if (game->loadstate<1) {
		PrintConsole(game, "ERROR: Attempted to preload invalid gamestate %d! Loading GAMESTATE_MENU instead...", game->loadstate);
		game->loadstate = GAMESTATE_MENU;
	}
	if ((game->loadstate==GAMESTATE_MENU) && (game->menu.loaded)) {
		PrintConsole(game, "GAMESTATE_MENU already loaded, skipping...");
		return;
	}
	switch (game->loadstate) {
		PRELOAD_STATE(GAMESTATE_MENU, Menu)
		PRELOAD_STATE(GAMESTATE_LOADING, Loading)
		PRELOAD_STATE(GAMESTATE_ABOUT, About)
		PRELOAD_STATE(GAMESTATE_INTRO, Intro)
		PRELOAD_STATE(GAMESTATE_MAP, Map)
		PRELOAD_STATE(GAMESTATE_LEVEL, Level)
		PRELOAD_STATE(GAMESTATE_DISCLAIMER, Disclaimer)
		default:
			PrintConsole(game, "ERROR: Attempted to preload unknown gamestate %d!", game->loadstate);
		break;
	}
	PrintConsole(game, "finished");
}

void UnloadGameState(struct Game *game) {
	switch (game->gamestate) {
		case GAMESTATE_MENU:
			if (game->shuttingdown) {
				PrintConsole(game, "Unload GAMESTATE_MENU..."); Menu_Unload(game);
			} else {
				PrintConsole(game, "Just stopping GAMESTATE_MENU..."); Menu_Stop(game);
			}
			break;
		UNLOAD_STATE(GAMESTATE_PAUSE, Pause)
		UNLOAD_STATE(GAMESTATE_LOADING, Loading)
		UNLOAD_STATE(GAMESTATE_ABOUT, About)
		UNLOAD_STATE(GAMESTATE_INTRO, Intro)
		UNLOAD_STATE(GAMESTATE_MAP, Map)
		UNLOAD_STATE(GAMESTATE_LEVEL, Level)
		UNLOAD_STATE(GAMESTATE_DISCLAIMER, Disclaimer)
		default:
			PrintConsole(game, "ERROR: Attempted to unload unknown gamestate %d!", game->gamestate);
			break;
	}
	PrintConsole(game, "finished");
}

void LoadGameState(struct Game *game) {
	game->gamestate = game->loadstate;
	game->loadstate = -1;
	switch (game->gamestate) {
		LOAD_STATE(GAMESTATE_MENU, Menu)
		LOAD_STATE(GAMESTATE_LOADING, Loading)
		LOAD_STATE(GAMESTATE_ABOUT, About)
		LOAD_STATE(GAMESTATE_INTRO, Intro)
		LOAD_STATE(GAMESTATE_MAP, Map)
		LOAD_STATE(GAMESTATE_LEVEL, Level)
		LOAD_STATE(GAMESTATE_DISCLAIMER, Disclaimer)
		default:
			PrintConsole(game, "ERROR: Attempted to load unknown gamestate %d!", game->loadstate);
	}
	PrintConsole(game, "finished");
}

void DrawGameState(struct Game *game) {
	switch (game->gamestate) {
		DRAW_STATE(GAMESTATE_MENU, Menu)
		DRAW_STATE(GAMESTATE_PAUSE, Pause)
		DRAW_STATE(GAMESTATE_LOADING, Loading)
		DRAW_STATE(GAMESTATE_ABOUT, About)
		DRAW_STATE(GAMESTATE_INTRO, Intro)
		DRAW_STATE(GAMESTATE_MAP, Map)
		DRAW_STATE(GAMESTATE_LEVEL, Level)
		DRAW_STATE(GAMESTATE_DISCLAIMER, Disclaimer)
		default:
			game->showconsole = true;
			al_clear_to_color(al_map_rgb(0,0,0));
			PrintConsole(game, "ERROR: Unknown gamestate %d reached! (5 sec sleep)", game->gamestate);
			DrawConsole(game);
			al_flip_display();
			al_rest(5.0);
			PrintConsole(game, "Returning to menu...");
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_MENU;
			break;
	}
}

void LogicGameState(struct Game *game) {
	switch (game->gamestate) {
		LOGIC_STATE(GAMESTATE_ABOUT, About)
		LOGIC_STATE(GAMESTATE_MENU, Menu)
		LOGIC_STATE(GAMESTATE_MAP, Map)
		LOGIC_STATE(GAMESTATE_INTRO, Intro)
		LOGIC_STATE(GAMESTATE_LEVEL, Level)
		default:
			// not every gamestate needs to have logic function
			break;
	}
}

void PauseGameState(struct Game *game) {
	switch (game->loadstate) {
		PAUSE_STATE(GAMESTATE_LEVEL, Level)
		default:
			// not every gamestate needs to have pause function
			break;
	}
}

void ResumeGameState(struct Game *game) {
	switch (game->loadstate) {
		RESUME_STATE(GAMESTATE_LEVEL, Level)
		default:
			// not every gamestate needs to have resume function
			break;
	}
}

void FadeGameState(struct Game *game, bool in) {
	ALLEGRO_BITMAP* bitmap = al_create_bitmap(game->viewportWidth, game->viewportHeight);
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	float fadeloop;
	if (in) {
		fadeloop = 255;
	} else {
		fadeloop = 0;
	}
	while ((in && fadeloop>=0) || (!in && fadeloop<255)) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(game->event_queue, &ev);
		if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == game->timer)) {
			LogicGameState(game);
			if (in) {
				fadeloop-=10;
			} else {
				fadeloop+=10;
			}
		}
		if (al_is_event_queue_empty(game->event_queue)) {
			DrawGameState(game);
			al_draw_tinted_bitmap(bitmap,al_map_rgba_f(1,1,1,fadeloop/255.0),0,0,0);
			DrawConsole(game);
			al_flip_display();
		}
	}
	al_destroy_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0,0,0));
	if (in) {
		DrawGameState(game);
	}
}

/*! \brief Scales bitmap using software linear filtering method to current target. */
void ScaleBitmap(ALLEGRO_BITMAP* source, int width, int height) {
	if ((al_get_bitmap_width(source)==width) && (al_get_bitmap_height(source)==height)) {
		al_draw_bitmap(source, 0, 0, 0);
		return;
	}
	int x, y;
	al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
	al_lock_bitmap(source, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

	/* linear filtering code written by SiegeLord */

	ALLEGRO_COLOR interpolate(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, float frac) {
		return al_map_rgba_f(c1.r + frac * (c2.r - c1.r),
												 c1.g + frac * (c2.g - c1.g),
												 c1.b + frac * (c2.b - c1.b),
												 c1.a + frac * (c2.a - c1.a));
	}

	for (y = 0; y < height; y++) {
		float pixy = ((float)y / height) * ((float)al_get_bitmap_height(source) - 1);
		float pixy_f = floor(pixy);
		for (x = 0; x < width; x++) {
			float pixx = ((float)x / width) * ((float)al_get_bitmap_width(source) - 1);
			float pixx_f = floor(pixx);

			ALLEGRO_COLOR a = al_get_pixel(source, pixx_f, pixy_f);
			ALLEGRO_COLOR b = al_get_pixel(source, pixx_f + 1, pixy_f);
			ALLEGRO_COLOR c = al_get_pixel(source, pixx_f, pixy_f + 1);
			ALLEGRO_COLOR d = al_get_pixel(source, pixx_f + 1, pixy_f + 1);

			ALLEGRO_COLOR ab = interpolate(a, b, pixx - pixx_f);
			ALLEGRO_COLOR cd = interpolate(c, d, pixx - pixx_f);
			ALLEGRO_COLOR result = interpolate(ab, cd, pixy - pixy_f);

			al_put_pixel(x, y, result);
		}
	}
	al_unlock_bitmap(al_get_target_bitmap());
	al_unlock_bitmap(source);
}

ALLEGRO_BITMAP* LoadScaledBitmap(char* filename, int width, int height) {
	ALLEGRO_BITMAP *source, *target = al_create_bitmap(width, height);
	al_set_target_bitmap(target);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	char* origfn = GetDataFilePath(filename);
	void GenerateBitmap() {
		if (memoryscale) al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

		source = al_load_bitmap( origfn );
		if (memoryscale) {
			al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
			ScaleBitmap(source, width, height);
		}
		else {
			al_draw_scaled_bitmap(source, 0, 0, al_get_bitmap_width(source), al_get_bitmap_height(source), 0, 0, width, height, 0);
		}
		/*al_save_bitmap(cachefn, target);
		PrintConsole(game, "Cache bitmap %s generated.", filename);*/
		al_destroy_bitmap(source);
	}

	/*source = al_load_bitmap( cachefn );
	if (source) {
		if ((al_get_bitmap_width(source)!=width) || (al_get_bitmap_height(source)!=height)) {
			al_destroy_bitmap(source);*/
	GenerateBitmap();
	free(origfn);
	return target;
	/*	}
		return source;
	} else GenerateBitmap();
	return target;*/
}


void SetupViewport(struct Game *game) {
	game->viewportWidth = al_get_display_width(game->display);
	game->viewportHeight = al_get_display_height(game->display);
	if (atoi(GetConfigOptionDefault("SuperDerpy", "letterbox", "1"))) {
		float const aspectRatio = (float)1920 / (float)1080; // full HD
		int clipWidth = game->viewportWidth, clipHeight = game->viewportWidth / aspectRatio;
		int clipX = 0, clipY = (game->viewportHeight - clipHeight) / 2;
		if (clipY <= 0) {
			clipHeight = game->viewportHeight;
			clipWidth = game->viewportHeight * aspectRatio;
			clipX = (game->viewportWidth - clipWidth) / 2;
			clipY = 0;
		}
		al_set_clipping_rectangle(clipX, clipY, clipWidth, clipHeight);

		/*float scaleX = (float)clipWidth  / (float)800,
					scaleY = (float)clipHeight / (float)450;*/
		ALLEGRO_TRANSFORM projection;
		al_build_transform(&projection, clipX, clipY, 1, 1, 0.0f);
		al_use_transform(&projection);
		game->viewportWidth = clipWidth;
		game->viewportHeight = clipHeight;
	}
}

int Shared_Load(struct Game *game) {
	game->font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.09,0 );
	if(!game->font) {
		fprintf(stderr, "failed to load game font!\n");
		return -1;
	}
	game->font_console = al_load_ttf_font(GetDataFilePath("fonts/DejaVuSansMono.ttf"),game->viewportHeight*0.018,0 );
	if(!game->font_console) {
		fprintf(stderr, "failed to load console font!\n");
		return -1;
	}
	game->console = al_create_bitmap(game->viewportWidth, game->viewportHeight*0.12);
	al_set_target_bitmap(game->console);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	return 0;
}

void Shared_Unload(struct Game *game) {
	al_destroy_font(game->font);
	al_destroy_font(game->font_console);
	al_destroy_bitmap(game->console);
}

void derp(int sig) {
	write(STDERR_FILENO, "Segmentation fault\n", 19);
	write(STDERR_FILENO, "I just don't know what went wrong!\n", 35);
	abort();
}

int main(int argc, char **argv){
	signal(SIGSEGV, derp);

	srand(time(NULL));

	al_set_org_name("Super Derpy");
	al_set_app_name("Muffin Attack");

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	InitConfig();

	struct Game game;

	game.fullscreen = atoi(GetConfigOptionDefault("SuperDerpy", "fullscreen", "1"));
	game.music = atoi(GetConfigOptionDefault("SuperDerpy", "music", "7"));
	game.voice = atoi(GetConfigOptionDefault("SuperDerpy", "voice", "10"));
	game.fx = atoi(GetConfigOptionDefault("SuperDerpy", "fx", "10"));
	game.debug = atoi(GetConfigOptionDefault("SuperDerpy", "debug", "0"));
	game.width = atoi(GetConfigOptionDefault("SuperDerpy", "width", "800"));
	if (game.width<320) game.width=320;
	game.height = atoi(GetConfigOptionDefault("SuperDerpy", "height", "450"));
	if (game.height<200) game.height=180;
	memoryscale = !atoi(GetConfigOptionDefault("SuperDerpy", "GPU_scaling", "1"));

	if(!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		/*al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
															 NULL, ALLEGRO_MESSAGEBOX_ERROR);*/
		return -1;
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

	if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
	}
	
	/*	if (!al_reserve_samples(10)){
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}
 */
	al_init_font_addon();

	if(!al_init_ttf_addon()){
		fprintf(stderr, "failed to initialize fonts!\n");
		return -1;
	}

	if (game.fullscreen) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	else al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_new_display_option(ALLEGRO_VSYNC, 2-atoi(GetConfigOptionDefault("SuperDerpy", "vsync", "1")), ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_OPENGL, atoi(GetConfigOptionDefault("SuperDerpy", "opengl", "1")), ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	game.display = al_create_display(game.width, game.height);
	if(!game.display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	ALLEGRO_BITMAP *icon = al_load_bitmap(GetDataFilePath("icons/superderpy.png"));
	al_set_window_title(game.display, "Super Derpy: Muffin Attack");
	al_set_display_icon(game.display, icon);
	al_destroy_bitmap(icon);

	if (game.fullscreen) al_hide_mouse_cursor(game.display);
	al_inhibit_screensaver(true);

	SetupViewport(&game);

	al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

	int ret = Shared_Load(&game);
	if (ret!=0) return ret;

	PrintConsole(&game, "Viewport %dx%d", game.viewportWidth, game.viewportHeight);

	game.event_queue = al_create_event_queue();
	if(!game.event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(game.display);
		return -1;
	}

	game.audio.v = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	game.audio.mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	game.audio.fx = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	game.audio.music = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	game.audio.voice = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	al_attach_mixer_to_voice(game.audio.mixer, game.audio.v);
	al_attach_mixer_to_mixer(game.audio.fx, game.audio.mixer);
	al_attach_mixer_to_mixer(game.audio.music, game.audio.mixer);
	al_attach_mixer_to_mixer(game.audio.voice, game.audio.mixer);
	al_set_mixer_gain(game.audio.fx, game.fx/10.0);
	al_set_mixer_gain(game.audio.music, game.music/10.0);
	al_set_mixer_gain(game.audio.voice, game.voice/10.0);

	al_register_event_source(game.event_queue, al_get_display_event_source(game.display));
	al_register_event_source(game.event_queue, al_get_keyboard_event_source());

	game.showconsole = game.debug;

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
	game.timer = al_create_timer(ALLEGRO_BPS_TO_SECS(60)); // logic timer
	if(!game.timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	al_register_event_source(game.event_queue, al_get_timer_event_source(game.timer));
	al_wait_for_vsync();
	al_start_timer(game.timer);

	setlocale(LC_NUMERIC, "C"); /* FIXME? */

	game.shuttingdown = false;
	game.menu.loaded = false;
	game.restart = false;
	game.loadstate = GAMESTATE_LOADING;
	PreloadGameState(&game, NULL);
	LoadGameState(&game);
	game.loadstate = GAMESTATE_MENU;

	int c;
	while ((c = getopt (argc, argv, "l:s:")) != -1)
		switch (c) {
			case 'l':
				game.level.input.current_level = optarg[0]-'0';
				game.loadstate = GAMESTATE_LEVEL;
				break;
			case 's':
				game.loadstate = optarg[0]-'0';
				break;
		}

	while(1) {
		ALLEGRO_EVENT ev;
		if (al_is_event_queue_empty(game.event_queue)) {
			DrawGameState(&game);
			DrawConsole(&game);
			al_flip_display();
		} else {
			al_wait_for_event(game.event_queue, &ev);
			if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == game.timer)) {
				LogicGameState(&game);
			}
			else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				break;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				/*PrintConsole(&game, "KEYCODE: %s", al_keycode_to_name(ev.keyboard.keycode));*/
			#ifdef ALLEGRO_MACOSX
				if ((ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == 104)) {
			#else
				if ((ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_TILDE)) {
			#endif
					game.showconsole = !game.showconsole;
				}
				else if ((game.debug) && (ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_F1)) {
					int i;
					for (i=0; i<512; i++) {
						LogicGameState(&game);
					}
					game.showconsole = true;
					PrintConsole(&game, "DEBUG: 512 frames skipped...");
				}	else if ((game.debug) && (ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_F10)) {
					double speed = ALLEGRO_BPS_TO_SECS(al_get_timer_speed(game.timer)); // inverting
					speed -= 10;
					if (speed<10) speed = 10;
					al_set_timer_speed(game.timer, ALLEGRO_BPS_TO_SECS(speed));
					game.showconsole = true;
					PrintConsole(&game, "DEBUG: Gameplay speed: %.2fx", speed/60.0);
				}	else if ((game.debug) && (ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_F11)) {
					double speed = ALLEGRO_BPS_TO_SECS(al_get_timer_speed(game.timer)); // inverting
					speed += 10;
					if (speed>600) speed = 600;
					al_set_timer_speed(game.timer, ALLEGRO_BPS_TO_SECS(speed));
					game.showconsole = true;
					PrintConsole(&game, "DEBUG: Gameplay speed: %.2fx", speed/60.0);
				} else if ((game.debug) && (ev.type == ALLEGRO_EVENT_KEY_DOWN) && (ev.keyboard.keycode == ALLEGRO_KEY_F12)) {
					ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
					char filename[255] = { };
					sprintf(filename, "SuperDerpy_%ld_%ld.png", time(NULL), clock());
					al_set_path_filename(path, filename);
					al_save_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), al_get_backbuffer(game.display));
					PrintConsole(&game, "Screenshot stored in %s", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
					al_destroy_path(path);
				}
				KEYDOWN_STATE(GAMESTATE_PAUSE, Pause)
				KEYDOWN_STATE(GAMESTATE_MENU, Menu)
				KEYDOWN_STATE(GAMESTATE_LOADING, Loading)
				KEYDOWN_STATE(GAMESTATE_ABOUT, About)
				KEYDOWN_STATE(GAMESTATE_INTRO, Intro)
				KEYDOWN_STATE(GAMESTATE_MAP, Map)
				KEYDOWN_STATE(GAMESTATE_LEVEL, Level)
				KEYDOWN_STATE(GAMESTATE_DISCLAIMER, Disclaimer)
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
			} else if (game.gamestate == GAMESTATE_LEVEL) {
				Level_ProcessEvent(&game, &ev);
			}
		}
	}
	game.shuttingdown = true;
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
	Shared_Unload(&game);
	al_destroy_display(game.display);
	al_destroy_event_queue(game.event_queue);
	al_destroy_mixer(game.audio.fx);
	al_destroy_mixer(game.audio.music);
	al_destroy_mixer(game.audio.mixer);
	al_destroy_voice(game.audio.v);
	al_uninstall_audio();
	DeinitConfig();
	if (game.restart) {
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();
#ifdef ALLEGRO_MACOSX
		return _al_mangled_main(argc, argv);
#else
		return main(argc, argv);
#endif
	}
	return 0;
}
