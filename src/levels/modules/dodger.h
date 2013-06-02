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

struct Dodger {

		struct Character *character; /* TODO: move to level */

	struct {
		struct {
			ALLEGRO_BITMAP *pie1; /*!< Pie bitmap. */
			ALLEGRO_BITMAP *pie2; /*!< Pie bitmap (crossed). */
			ALLEGRO_BITMAP *muffin; /*!< Good muffin bitmap. */
			ALLEGRO_BITMAP *badmuffin; /*!< Bad muffin bitmap. */
			ALLEGRO_BITMAP *cherry; /*!< Cherry bitmap. */
			ALLEGRO_BITMAP *pig; /*!< Pig spritesheet bitmap. */
			ALLEGRO_BITMAP *screwball; /*!< Screwball spritesheet bitmap. */
		} obst_bmps; /*!< Obstacle bitmaps. */
		struct Obstacle *obstacles; /*!< List of obstacles being currently rendered. */
	} dodger;


	struct {
int current_level; /*!< Level number. */
} input; /*!< Gamestate input data. */
int current_level; /*!< Level number. */
float speed; /*!< Speed of the player. */
float speed_modifier; /*!< Modifier of the speed of the player. */
float bg_pos; /*!< Position of the background layer of the scene. */
float st_pos; /*!< Position of the stage layer of the scene. */
float fg_pos; /*!< Position of the foreground layer of the scene. */
float cl_pos; /*!< Position of the clouds layer of the scene. */
float derpy_x; /*!< Horizontal position of Derpy (0-1). */
float derpy_y; /*!< Vertical position of Derpy (0-1). */
float derpy_angle; /*!< Angle of Derpy sprite on screen (radians). */
float hp; /*!< Player health points (0-1). */
bool handle_input; /*!< When false, player looses control over Derpy. */
bool failed; /*!< Indicates if player failed level. */
bool unloading; /*!< Indicated if level is already being unloaded. */
float meter_alpha; /*!< Alpha level of HP meter. */
int sheet_rows; /*!< Number of rows in current spritesheet. */
int sheet_cols; /*!< Number of cols in current spritesheet. */
int sheet_pos; /*!< Frame position in current spritesheet. */
int sheet_blanks; /*!< Number of blank frames at the end of current spritesheet. */
char* sheet_successor; /*!< Successor of current animation. If blank, then it's looped. */
float sheet_tmp; /*!< Temporary counter used to slow down spritesheet animation. */
float sheet_speed; /*!< Current speed of Derpy animation. */
float sheet_speed_modifier; /*!< Modifier of speed, specified by current spritesheet. */
float sheet_scale; /*!< Scale modifier of current spritesheet. */
ALLEGRO_FONT *letter_font; /*!< Font used in letter from Twilight on first level. */
ALLEGRO_SAMPLE *sample; /*!< Sample with background music. */
ALLEGRO_SAMPLE_INSTANCE *music; /*!< Sample instance with background music. */
unsigned int music_pos; /*!< Position of sample instance. Used when pausing game. */
ALLEGRO_BITMAP *background; /*!< Bitmap of the background layer of the scene. */
ALLEGRO_BITMAP *stage; /*!< Bitmap of the stage layer of the scene. */
ALLEGRO_BITMAP *foreground; /*!< Bitmap of the foreground layer of the scene. */
ALLEGRO_BITMAP *clouds; /*!< Bitmap of the clouds layer of the scene. */
ALLEGRO_BITMAP *welcome; /*!< Bitmap of the welcome text (for instance "Level 1: Fluttershy"). */
ALLEGRO_BITMAP **derpy_sheet; /*!< Pointer to active Derpy sprite sheet. */
ALLEGRO_BITMAP *derpy; /*!< Derpy sprite. */
ALLEGRO_BITMAP *meter_bmp; /*!< Bitmap of the HP meter. */
ALLEGRO_BITMAP *meter_image; /*!< Derpy image used in the HP meter. */
ALLEGRO_BITMAP *letter; /*!< Bitmap with letter from Twilight. */
bool debug_show_sprite_frames; /*!< When true, displays colorful borders around spritesheets and their active areas. */
struct Spritesheet* derpy_sheets; /*!< List of spritesheets of Derpy character. */
//struct Spritesheet* pony_sheets; /*!< List of spritesheets of character rescued by Derpy. */
struct {
ALLEGRO_BITMAP *owl; /*!< Owlicious bitmap. */
} level1; /*!< Resources used by level 1. */
};

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

void Dodger_Draw(struct Game *game, struct Dodger *data);
void Dodger_Logic(struct Game *game, struct Dodger *data);
void Dodger_Start(struct Game *game, struct Dodger *data);
void Dodger_Stop(struct Game *game, struct Dodger *data);
void Dodger_Unload(struct Game *game, struct Dodger *data);
struct Dodger* Dodger_Load(struct Game *game, int current_level);
void Dodger_UnloadBitmaps(struct Game *game, struct Dodger *data);
void Dodger_LoadBitmaps(struct Game *game, struct Dodger *data, void (*progress)(struct Game*, float));
void Dodger_ProcessEvent(struct Game *game, struct Dodger *data, ALLEGRO_EVENT *ev);
void Dodger_Resume(struct Game *game, struct Dodger *data);
void Dodger_Pause(struct Game *game, struct Dodger *data);
