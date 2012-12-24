/*! \file config.h
 *  \brief Configuration manager headers.
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

#include "main.h"

/*! \brief Reads config from file into memory. */
void InitConfig(struct Game *game);
/*! \brief Returns value of requested config entry. */
const char* GetConfigOption(struct Game *game, char* section, char* name);
/*! \brief Returns value of requested config entry, or def if no such entry exists. */
const char* GetConfigOptionDefault(struct Game *game, char* section, char* name, const char* def);
/*! \brief Sets new value of requested config entry, or created new if no such entry exists. */
void SetConfigOption(struct Game *game, char* section, char* name, char* value);
/*! \brief Writes config from memory to file. */
void DeinitConfig(struct Game *game);
