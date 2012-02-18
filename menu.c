#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <math.h>
#include "menu.h"

void Menu_Draw(struct Game *game) {

	//game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)); //*0.8122);
	al_set_target_bitmap(game->menu.pinkcloud_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	float x = 1.5;//*(rand() / (float)RAND_MAX);
	int minus;
	if (game->menu.cloud_position>0) minus=1; else minus=-1;
	al_draw_scaled_bitmap(game->menu.rain, 0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), fmod(minus*game->menu.cloud_position,3)*x*5+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.7, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.88+(fmod(-1.8*(game->menu.cloud_position+80), 6))/20.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.5, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.1, 0);
	al_draw_scaled_bitmap(game->menu.rain, 0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), fmod(minus*game->menu.cloud_position,3)*x*3+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/3.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.78+(fmod(-2.8*(game->menu.cloud_position+80), 4))/18.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.5, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.1, 0);
	al_draw_scaled_bitmap(game->menu.rain, 0, 0, al_get_bitmap_width(game->menu.rain), al_get_bitmap_height(game->menu.rain), fmod(minus*game->menu.cloud_position,3)*x*6+al_get_bitmap_width(game->menu.pinkcloud_bitmap)/2.1, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*(0.87+(fmod(-4.9*(game->menu.cloud_position+80), 8))/26.0), al_get_bitmap_width(game->menu.pinkcloud_bitmap)*0.4, al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.08, 0);
	al_draw_scaled_bitmap(game->menu.pinkcloud,0, 0, al_get_bitmap_width(game->menu.pinkcloud), al_get_bitmap_height(game->menu.pinkcloud), 0, 0, al_get_bitmap_width(game->menu.pinkcloud_bitmap), al_get_bitmap_height(game->menu.pinkcloud_bitmap)*0.8122,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(183,234,193));
	float tint = (sin((game->menu.cloud_position-30)/15)+1)/2;
	if (tint < 0.0001) { PrintConsole(game, "random tint"); game->menu.mountain_position = (al_get_display_width(game->display)*(rand()/(float)RAND_MAX)/2)+al_get_display_width(game->display)/2; }
	al_draw_tinted_bitmap(game->menu.mountain_bitmap,al_map_rgba_f(tint,tint,tint,tint),game->menu.mountain_position, 0,0);
	al_draw_scaled_bitmap(game->menu.cloud_bitmap,0,0,al_get_bitmap_width(game->menu.cloud_bitmap), al_get_bitmap_height(game->menu.cloud_bitmap), al_get_display_width(game->display)*(sin((game->menu.cloud_position/40)-4.5)-0.3), al_get_display_height(game->display)*0.35, al_get_bitmap_width(game->menu.cloud_bitmap)/2, al_get_bitmap_height(game->menu.cloud_bitmap)/2,0);
	al_draw_bitmap(game->menu.cloud2_bitmap,al_get_display_width(game->display)*(game->menu.cloud2_position/100.0), al_get_display_height(game->display)/1.5,0);
	al_draw_bitmap(game->menu.menu_bitmap,0, 0,0);
	al_draw_bitmap(game->menu.pinkcloud_bitmap,(al_get_display_width(game->display)*0.12) + (cos((game->menu.cloud_position/25+80)*1.74444))*40, 0,0);
	al_draw_bitmap(game->menu.cloud_bitmap,al_get_display_width(game->display)*game->menu.cloud_position/100, 30,0);

	al_draw_bitmap(game->menu.pie_bitmap, al_get_display_width(game->display)/2, al_get_display_height(game->display)*(game->menu.cloud_position)/10,0);

	al_draw_text(game->menu.font_title, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.1, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
	al_draw_text(game->menu.font_subtitle, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.275, ALLEGRO_ALIGN_CENTRE, "Muffin Attack");

	ALLEGRO_FONT *font;
	char* text;
	font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
	text = "Start game"; if (game->menu.options) text="Fullscreen: on";
	al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
	text = "Options"; if (game->menu.options) text="Music: on";
	al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
	text = "About"; if (game->menu.options) text="Sounds: on";
	al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.7, ALLEGRO_ALIGN_CENTRE, text);
	font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
	text = "Exit"; if (game->menu.options) text="Back";
	al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.8, ALLEGRO_ALIGN_CENTRE, text);

	game->menu.cloud_position-=0.1;
	game->menu.cloud2_position-=0.025;
	if (game->menu.cloud_position<-80) { game->menu.cloud_position=100; PrintConsole(game, "cloud_position"); }
	if (game->menu.cloud2_position<0) { game->menu.cloud2_position=100; PrintConsole(game, "cloud2_position"); }
}

void Menu_Preload(struct Game *game) {
	game->menu.cloud_position = 100;
	game->menu.cloud2_position = 100;
	game->menu.options = false;
	//game->menu.image = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	//al_destroy_bitmap(game->menu.image); // ugh...
	game->menu.image = al_load_bitmap( "menu.png" );
	game->menu.mountain = al_load_bitmap( "mountain.png" );
	game->menu.sample = al_load_sample( "menu.wav" );
	game->menu.rain_sample = al_load_sample( "rain.wav" );
	game->menu.cloud = al_load_bitmap( "cloud.png" );
	game->menu.cloud2 = al_load_bitmap( "cloud2.png" );
	game->menu.pinkcloud = al_load_bitmap( "pinkcloud.png" );
	game->menu.pie = al_load_bitmap( "pie.png" );
	game->menu.rain = al_load_bitmap( "rain.png" );
	game->menu.mountain_position = al_get_display_width(game->display)*0.7;

	game->menu.font_title = al_load_ttf_font("ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.16,0 );
	game->menu.font_subtitle = al_load_ttf_font("ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.08,0 );
	game->menu.font = al_load_ttf_font("ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.05,0 );
	game->menu.font_selected = al_load_ttf_font("ShadowsIntoLight.ttf",al_get_display_height(game->display)*0.065,0 );
	
	if (!game->menu.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.rain_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}

	// Scale menu bitmap
	game->menu.menu_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->menu.menu_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.image,0, 0, al_get_bitmap_width(game->menu.image), al_get_bitmap_height(game->menu.image), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display),0);
	al_destroy_bitmap(game->menu.image);

	// Cloud menu bitmap
	game->menu.cloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.25);
	al_set_target_bitmap(game->menu.cloud_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.cloud,0, 0, al_get_bitmap_width(game->menu.cloud), al_get_bitmap_height(game->menu.cloud), 0, 0, al_get_bitmap_width(game->menu.cloud_bitmap), al_get_bitmap_height(game->menu.cloud_bitmap),0);
	al_destroy_bitmap(game->menu.cloud);

	game->menu.cloud2_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.2, al_get_display_height(game->display)*0.1);
	al_set_target_bitmap(game->menu.cloud2_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.cloud2,0, 0, al_get_bitmap_width(game->menu.cloud2), al_get_bitmap_height(game->menu.cloud2), 0, 0, al_get_bitmap_width(game->menu.cloud2_bitmap), al_get_bitmap_height(game->menu.cloud2_bitmap),0);
	al_destroy_bitmap(game->menu.cloud2);
	
	game->menu.pinkcloud_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.33125, al_get_display_height(game->display)); //*0.8122);

	game->menu.pie_bitmap = al_create_bitmap(al_get_display_width(game->display)/2, al_get_display_height(game->display)); //*0.8122);
	al_set_target_bitmap(game->menu.pie_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.5, 0, al_get_display_width(game->display)*0.11875, al_get_display_height(game->display)*0.0825, 0);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.1, al_get_bitmap_height(game->menu.pie_bitmap)*0.3, al_get_display_width(game->display)*0.09, al_get_display_height(game->display)*0.06, ALLEGRO_FLIP_HORIZONTAL);
	al_draw_scaled_bitmap(game->menu.pie, 0, 0, al_get_bitmap_width(game->menu.pie), al_get_bitmap_height(game->menu.pie), al_get_bitmap_width(game->menu.pie_bitmap)*0.3, al_get_bitmap_height(game->menu.pie_bitmap)*0.6, al_get_display_width(game->display)*0.13, al_get_display_height(game->display)*0.1, 0);
	al_destroy_bitmap(game->menu.pie);

	game->menu.mountain_bitmap = al_create_bitmap(al_get_display_width(game->display)*0.055, al_get_display_height(game->display)/9);
	al_set_target_bitmap(game->menu.mountain_bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_scaled_bitmap(game->menu.mountain,0, 0, al_get_bitmap_width(game->menu.mountain), al_get_bitmap_height(game->menu.mountain), 0, 0, al_get_bitmap_width(game->menu.mountain_bitmap), al_get_bitmap_height(game->menu.mountain_bitmap),0);
	al_destroy_bitmap(game->menu.mountain);

	game->menu.selected = 0;
}

void Menu_Unload(struct Game *game) {

	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));

	ALLEGRO_EVENT ev;
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		Menu_Draw(game);
		al_set_target_bitmap(game->menu.menu_fade_bitmap);
		al_draw_bitmap(al_get_backbuffer(game->display), 0, 0, 0);
		al_set_target_bitmap(al_get_backbuffer(game->display));
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
	al_destroy_bitmap(game->menu.pinkcloud);
	al_destroy_bitmap(game->menu.menu_bitmap);
	al_destroy_bitmap(game->menu.cloud_bitmap);
	al_destroy_bitmap(game->menu.cloud2_bitmap);
	al_destroy_bitmap(game->menu.pinkcloud_bitmap);
	al_destroy_bitmap(game->menu.rain);
	al_destroy_bitmap(game->menu.mountain_bitmap);
	al_destroy_bitmap(game->menu.pie_bitmap);
	al_destroy_font(game->menu.font_title);
	al_destroy_font(game->menu.font_subtitle);
	al_destroy_font(game->menu.font);
	al_destroy_font(game->menu.font_selected);
	al_destroy_sample(game->menu.sample);
	al_destroy_sample(game->menu.rain_sample);
}

void Menu_Load(struct Game *game) {
	al_play_sample(game->menu.sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	al_play_sample(game->menu.rain_sample, 0.7, -0.3, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	game->menu.menu_fade_bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));

	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		Menu_Draw(game);
		al_set_target_bitmap(game->menu.menu_fade_bitmap);
		al_draw_bitmap(al_get_backbuffer(game->display), 0, 0, 0);
		al_set_target_bitmap(al_get_backbuffer(game->display));

		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->menu.menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->menu.menu_fade_bitmap);
}

int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
	} else if ((!game->menu.options) && (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)) || (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE))) {
		return 1;
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==0)) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_INTRO;
	} else if ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==2)) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_ABOUT;
	} else if (((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (!game->menu.options) && (game->menu.selected==1)) || ((game->menu.options) && ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) || ((ev->keyboard.keycode==ALLEGRO_KEY_ENTER) && (game->menu.selected==3)))) {
		game->menu.options=!game->menu.options;
		game->menu.selected=0;
		PrintConsole(game, "options state changed");
	}
	if (game->menu.selected==-1) game->menu.selected=3;
	if (game->menu.selected==4) game->menu.selected=0;
	return 0;
}