#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include "intro.h"

void Intro_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	//printf("%f\n", ((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display)));
	if (game->intro.in_animation)
		al_draw_bitmap(game->intro.table, -1*(game->intro.page)*al_get_display_width(game->display) + (cos(((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display))*(3.1415))/2.0)*al_get_display_width(game->display) + al_get_display_width(game->display)/2.0, al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
	else
		al_draw_bitmap(game->intro.table, -1*(game->intro.page)*al_get_display_width(game->display), al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");
	//PrintConsole(game, "drawing");
	if (game->intro.in_animation) {
		//PrintConsole(game, "animating");
		game->intro.position -= 5;
		if (game->intro.position%al_get_display_width(game->display)==0) {
			game->intro.in_animation = false;
			PrintConsole(game, "Animation finished.");
		}
		else if (game->intro.position<=-3*al_get_display_width(game->display)) {
			PrintConsole(game, "This was the last page.");
			UnloadGameState(game);
			game->gamestate = GAMESTATE_LOADING;
			game->loadstate = GAMESTATE_MENU;
		}
	}
}

void Intro_Load(struct Game *game) {
	ALLEGRO_EVENT ev;
	for(int fadeloop=0; fadeloop<256; fadeloop+=10){
		al_wait_for_event(game->event_queue, &ev);
		al_draw_tinted_bitmap(game->intro.table_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
		DrawConsole(game);
		al_flip_display();
	}
}
int Intro_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		UnloadGameState(game);
		game->gamestate = GAMESTATE_LOADING;
		game->loadstate = GAMESTATE_MENU;
		return 0;
	}
	if (!game->intro.in_animation) {
		PrintConsole(game, "Animate page...");
		game->intro.in_animation = true;
		game->intro.page++;
	}
	return 0;
}
void Intro_Preload(struct Game *game) {
	game->intro.position = 0;
	game->intro.page = 0;
	game->intro.in_animation = false;
	game->intro.table_bitmap = al_load_bitmap( "table.png" );
	game->intro.table = al_create_bitmap(al_get_display_width(game->display)*4, al_get_display_height(game->display)*2);
	al_set_target_bitmap(game->intro.table);
	al_draw_bitmap(game->intro.table_bitmap, 0, 0, 0);
	//game->intro.table_bitmap = al_load_bitmap( "loading.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display), 0, ALLEGRO_FLIP_HORIZONTAL);
	//game->intro.table_bitmap = al_load_bitmap( "menu.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display)*2, 0, 0);
	//game->intro.table_bitmap = al_load_bitmap( "table.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display)*3, 0, ALLEGRO_FLIP_HORIZONTAL);

	//game->intro.table_bitmap = al_load_bitmap( "loading.png" );
	al_draw_bitmap(game->intro.table_bitmap, 0, al_get_display_height(game->display), ALLEGRO_FLIP_VERTICAL);
	//game->intro.table_bitmap = al_load_bitmap( "loading.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display), al_get_display_height(game->display), ALLEGRO_FLIP_VERTICAL | ALLEGRO_FLIP_HORIZONTAL);
	//game->intro.table_bitmap = al_load_bitmap( "table.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display)*2, al_get_display_height(game->display), ALLEGRO_FLIP_VERTICAL);
	//game->intro.table_bitmap = al_load_bitmap( "menu.png" );
	al_draw_bitmap(game->intro.table_bitmap, al_get_display_width(game->display)*3, al_get_display_height(game->display), ALLEGRO_FLIP_VERTICAL | ALLEGRO_FLIP_HORIZONTAL);

	al_set_target_bitmap(al_get_backbuffer(game->display));
}
void Intro_Unload(struct Game *game) {
	ALLEGRO_EVENT ev;
	for(int fadeloop=255; fadeloop>=0; fadeloop-=10){
		al_wait_for_event(game->event_queue, &ev);
		if (game->intro.in_animation)
			al_draw_tinted_bitmap(game->intro.table, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), -1*(game->intro.page)*al_get_display_width(game->display) + (cos(((-1*((game->intro.position)%al_get_display_width(game->display)))/(float)al_get_display_width(game->display))*(3.1415))/2.0)*al_get_display_width(game->display)  + al_get_display_width(game->display)/2.0, al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);
		else
			al_draw_tinted_bitmap(game->intro.table, al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1), -1*(game->intro.page)*al_get_display_width(game->display), al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)), 0);

		//al_draw_tinted_bitmap(game->intro.table_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),-1*(game->intro.page)*al_get_display_width(game->display), al_get_display_height(game->display)*((game->intro.position/3.0)/(float)al_get_display_width(game->display)),0);
		DrawConsole(game);
		al_flip_display();
	}
	al_destroy_bitmap(game->intro.table_bitmap);
	al_destroy_bitmap(game->intro.table);
}
