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

bool LevelFailed(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Accelerate(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Walk(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Move(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool ShowMeter(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Fly(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool GenerateObstacles(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Stop(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Letter(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool FadeIn(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool FadeOut(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool Welcome(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

bool PassLevel(struct Game *game, struct TM_Action *action, enum TM_ActionState state);
