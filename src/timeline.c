/*! \file timeline.c
 *  \brief Timeline Manager framework code.
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
#include <allegro5/allegro.h>
#include "timeline.h"

int lastid;

void TM_Init() {
	
}

void TM_AddAction(void *func, struct TM_Arguments* args) {
	
}

void TM_AddBackgroundAction(void *func, struct TM_Arguments* args, int delay) {
	
}

void TM_AddDelay(int delay) {
	
}

void TM_Pause(bool pause) {
	
}

void TM_Destroy() {
	
}

struct TM_Arguments* TM_AddToArgs() {
	return NULL;
}

void TM_DestroyArgs(struct TM_Arguments* args) {
	
}