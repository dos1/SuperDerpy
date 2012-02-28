/*! \file map.h
 *  \brief Map view headers.
 */

#include "main.h"

void Map_Draw(struct Game *game);
void Map_Preload(struct Game *game);
void Map_Unload(struct Game *game);
void Map_Load(struct Game *game);
int Map_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
