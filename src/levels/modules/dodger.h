/*! \file dodger.h
 *  \brief Dodger Level module headers.
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
#include "../../main.h"


/*! \brief Structure representing obstacles and power-ups flying through the level. */
struct Obstacle {
	ALLEGRO_BITMAP **bitmap; /*!< Pointer to bitmap used by obstacle. */
	float x; /*!< Horizontal position on the screen, in range 0-100. */
	float y; /*!< Vertical position on the screen, in range 0-100. */
	float speed; /*!< Horizontal speed of obstracle. */
	float angle; /*!< Angle of bitmap rotation in radians. */
	int points; /*!< Number of points given when hit by player. Positive gives HP to power, negative takes it. */
	bool hit; /*!< Indicates if this obstacle was already hit by the player or not. */

	int cols; /*!< Number of columns in spritesheet. */
	int rows; /*!< Number of rows in spritesheet. */
	int pos; /*!< Current position in spritesheet. */
	int blanks; /*!< Number of blank frames at the end of the spritesheet. */
	float tmp_pos; /*!< Temporary counter used to slow down spritesheet animation. */
	float anim_speed; /*!< Speed of spritesheet animation. */

	void (*callback)(struct Game*, struct Obstacle*); /*!< Pointer to function called to update obstacle position, animate it, etc. */
	void *data; /*!< Pointer passed to callback function. */
	struct Obstacle *prev; /*!< Previous obstacle on the list. */
	struct Obstacle *next; /*!< Next obstacle on the list. */
};

struct Dodger {


			struct Obstacle *obstacles; /*!< List of obstacles being currently rendered. */
			struct Obstacle *possible_obstacles; /* TODO: make list of used obstacles dynamic */
			float speed; /*!< Speed of the player. */
			float speed_modifier; /*!< Modifier of the speed of the player. */
			float hp; /*!< Player health points (0-1). */
			bool debug_show_sprite_frames; /*!< When true, displays colorful borders around spritesheets and their active areas. */
			struct Character *character;
			bool failed;
			bool handle_input; /*!< When false, player looses control over Derpy. */

			struct {
				ALLEGRO_BITMAP *pie1; /*!< Pie bitmap. */
				ALLEGRO_BITMAP *pie2; /*!< Pie bitmap (crossed). */
				ALLEGRO_BITMAP *muffin; /*!< Good muffin bitmap. */
				ALLEGRO_BITMAP *badmuffin; /*!< Bad muffin bitmap. */
				ALLEGRO_BITMAP *cherry; /*!< Cherry bitmap. */
				ALLEGRO_BITMAP *pig; /*!< Pig spritesheet bitmap. */
				ALLEGRO_BITMAP *screwball; /*!< Screwball spritesheet bitmap. */
			} obst_bmps; /*!< Obstacle bitmaps. */

			struct Timeline *timeline;

};

void Dodger_PreloadBitmaps(struct Game *game, struct Dodger* data);
void Dodger_Keydown(struct Game *game, struct Dodger* data, ALLEGRO_EVENT *ev);

void Dodger_Draw(struct Game *game, struct Dodger *data);
void Dodger_Logic(struct Game *game, struct Dodger *data);
void Dodger_Unload(struct Game *game, struct Dodger *data);
struct Dodger* Dodger_Load(struct Game *game, struct Character *character);
void Dodger_UnloadBitmaps(struct Game *game, struct Dodger *data);
void Dodger_LoadBitmaps(struct Game *game, struct Dodger *data, void (*progress)(struct Game*, float));
void Dodger_ProcessEvent(struct Game *game, struct Dodger *data, ALLEGRO_EVENT *ev);
void Dodger_Resume(struct Game *game, struct Dodger *data);
void Dodger_Pause(struct Game *game, struct Dodger *data);
