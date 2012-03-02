/*! \file pause.h
 *  \brief Pause state headers.
 */

#include "main.h"

void Pause_Draw(struct Game *game);
void Pause_Preload(struct Game *game);
void Pause_Unload(struct Game *game);
void Pause_Load(struct Game *game);
int Pause_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
