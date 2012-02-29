/*! \file menu.h
 *  \brief Main Menu view headers.
 */

#include "main.h"

void Menu_Draw(struct Game *game);
void Menu_Preload(struct Game *game);
void Menu_Stop(struct Game *game);
void Menu_Unload(struct Game *game);
void Menu_Load(struct Game *game);
int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
