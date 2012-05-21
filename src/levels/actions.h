/*! \file actions.h
 *  \brief Header file with Level actions for Timeline Manager.
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
#include "../timeline.h"

/*! \brief Display level failed screen and set fade-out to be run after few seconds.  */
bool LevelFailed(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Accelerate current speed game until threshold is reached  */
bool Accelerate(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Set Derpy to walk and move her position on screen. */
bool Walk(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Move screen until some position is reached. */
bool Move(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Fade-in HP meter. */
bool ShowMeter(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Fly Derpy, fly! */
bool Fly(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Generates obstracles. */
bool GenerateObstacles(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Stops Derpy. */
bool Stop(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Shows letter from Twilight on screen. */
bool Letter(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Fade-in screen. */
bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Fade-out screen. */
bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Show welcome screen (for instance "Level 1: Fluttershy"). */
bool Welcome(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

/*! \brief Pass level and fade-out after few seconds. */
bool PassLevel(struct Game *game, struct TM_Action *action, enum TM_ActionState state);
