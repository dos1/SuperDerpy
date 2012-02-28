/*! \file level.h
 *  \brief Playable Level headers.
 */

#include "main.h"

void Level_Draw(struct Game *game);
void Level_Preload(struct Game *game);
void Level_Unload(struct Game *game);
void Level_Load(struct Game *game);
int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
