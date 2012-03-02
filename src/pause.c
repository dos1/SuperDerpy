/*! \file pause.c
 *  \brief Pause state.
 */

#include <stdio.h>
#include "pause.h"

int Pause_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) || (ev->keyboard.keycode==ALLEGRO_KEY_P)) {
		al_destroy_bitmap(game->pause.bitmap);
		game->gamestate = game->loadstate;
	}
	return 0;
}

void Pause_Load(struct Game* game) {
	game->gamestate=game->loadstate;
	DrawGameState(game);
	game->gamestate=GAMESTATE_PAUSE;
	ALLEGRO_BITMAP *fade = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(fade);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_draw_tinted_bitmap(fade,al_map_rgba_f(1,1,1,0.75),0,0,0);
	al_draw_text_with_shadow(game->font, al_map_rgb(255,255,255),  al_get_display_width(game->display)*0.5, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE,"Game paused!");
	game->pause.bitmap = al_create_bitmap(al_get_display_width(game->display), al_get_display_height(game->display));
	al_set_target_bitmap(game->pause.bitmap);
	al_draw_bitmap(al_get_backbuffer(game->display), 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
}

void Pause_Draw(struct Game* game) {
	al_draw_bitmap(game->pause.bitmap, 0, 0, 0);
	DrawConsole(game);
}

void Pause_Unload(struct Game* game) {
	al_destroy_bitmap(game->pause.bitmap);
	game->gamestate=game->loadstate;
	UnloadGameState(game);
}