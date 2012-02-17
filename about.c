#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "about.h"

void About_Draw(struct Game *game) {
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(game->font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)/2, ALLEGRO_ALIGN_CENTRE, "Not implemented yet!");
	DrawConsole(game);
	al_flip_display();
	al_rest(5.0);
	UnloadGameState(game);
	game->gamestate = GAMESTATE_LOADING;
	game->loadstate = GAMESTATE_MENU;
}

void About_Load(struct Game *game) {}
int About_Keydown(struct Game *game, ALLEGRO_EVENT *ev) { return 0; }
void About_Preload(struct Game *game) {}
void About_Unload(struct Game *game) {}
