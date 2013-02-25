/*! \file level.h
 *  \brief Playable Level headers.
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
#include "../main.h"


/*! \brief Structure representing one spritesheet for character animation. */
struct Spritesheet {
	char* name; /*!< Name of the spritesheet (used in file paths). */
	ALLEGRO_BITMAP* bitmap; /*!< Spritesheet bitmap. */
	int rows; /*!< Number of rows in the spritesheet. */
	int cols; /*!< Number of columns in the spritesheet. */
	int blanks; /*!< Number of blank frames at the end of the spritesheet. */
	float speed; /*!< Speed modifier of spritesheet animation. */
	float aspect; /*!< Aspect ratio of the frame. */
	float scale; /*!< Scale modifier of the frame. */
	char* successor; /*!< Name of animation successor. If it's not blank, then animation will be played only once. */
	struct Spritesheet* next; /*!< Next spritesheet in the queue. */
};

/*! \brief Structure representing one visible character. */
struct Character {
	char* name; /*!< Name of the character (used in file paths). */
	struct Spritesheet *spritesheet; /*!< Current spritesheet used by character. */
	struct Spritesheet *spritesheets; /*!< List of all spritesheets registered to character. */
	ALLEGRO_BITMAP* bitmap;
	int pos; /*!< Current spritesheet position. */
	float x; /*!< Horizontal position of character (0-1). */
	float y; /*!< Vertical position of character (0-1). */
	float angle; /*!< Characters display angle (radians). */
	void* data; /*!< Additional, custom character data (HP etc.). */
};


void SelectSpritesheet(struct Game *game, struct Character *character, char* name);
void RegisterSpritesheet(struct Game *game, struct Character *character, char* name);
void AdvanceLevel(struct Game *game);
/*! \brief Replaces first '?' char in filename with current level number. */
char* GetLevelFilename(struct Game *game, char* filename);

void DrawCharacter(struct Game *game, struct Character *character);


/*void Level_Passed(struct Game *game);
void Level_Pause(struct Game *game);
void Level_Resume(struct Game *game);
void Level_Draw(struct Game *game);
void Level_Logic(struct Game *game);
void Level_Preload(struct Game *game, void (*progress)(struct Game*, float));
void Level_Unload(struct Game *game);
void Level_Load(struct Game *game);
void Level_ProcessEvent(struct Game *game, ALLEGRO_EVENT *ev);
int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev);
void Level_UnloadBitmaps(struct Game *game);
void Level_PreloadBitmaps(struct Game *game, void (*progress)(struct Game*, float));
*/
