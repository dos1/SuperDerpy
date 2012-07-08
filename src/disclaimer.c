/*! \file disclaimer.c
 *  \brief Disclaimer about early release preview.
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
 */
#include <stdio.h>
#include "intro.h"
#include "about.h"

void Disclaimer_Draw(struct Game *game) {
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.3, ALLEGRO_ALIGN_CENTRE, "This is an early development preview of the game.");
    al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.4, ALLEGRO_ALIGN_CENTRE, "It's not supposed to be complete!");
    al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.5, ALLEGRO_ALIGN_CENTRE, "Keep in mind that everything may be changed");
    al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.6, ALLEGRO_ALIGN_CENTRE, "and many things surely will change.");
    al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, al_get_display_height(game->display)*0.9, ALLEGRO_ALIGN_CENTRE, "Press any key to continue...");
}

void Disclaimer_Load(struct Game *game) {
    ALLEGRO_EVENT ev;
    float fadeloop;
    for (fadeloop=255; fadeloop>0; fadeloop-=tps(game, 600)) {
        al_wait_for_event(game->event_queue, &ev);
        Disclaimer_Draw(game);
        al_draw_filled_rectangle(0,0,al_get_display_width(game->display),al_get_display_height(game->display),al_map_rgba(0,0,0,fadeloop));
        DrawConsole(game);
        al_flip_display();
    }
    Disclaimer_Draw(game);
}

int Disclaimer_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
    UnloadGameState(game);
    game->loadstate = GAMESTATE_INTRO;
    LoadGameState(game);
	return 0;
}

void Disclaimer_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
    PrintConsole(game, "Preloading GAMESTATE_INTRO...");
    Intro_Preload(game, progress);
}

void Disclaimer_Unload(struct Game *game) {
    ALLEGRO_EVENT ev;
    float fadeloop;
    for (fadeloop=0; fadeloop<256; fadeloop+=tps(game, 600)) {
        al_wait_for_event(game->event_queue, &ev);
        Disclaimer_Draw(game);
        al_draw_filled_rectangle(0,0,al_get_display_width(game->display),al_get_display_height(game->display),al_map_rgba(0,0,0,fadeloop));
        DrawConsole(game);
        al_flip_display();
    }
    al_clear_to_color(al_map_rgb(0,0,0));
}
