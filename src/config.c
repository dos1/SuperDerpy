/*! \file config.c
 *  \brief Configuration manager code.
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
#include "config.h"

void InitConfig(struct Game *game) {
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
	ALLEGRO_PATH *data = al_create_path("SuperDerpy.ini");
	al_join_paths(path, data);
	game->_priv.config = al_load_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	if (!game->_priv.config) game->_priv.config=al_create_config();
	al_destroy_path(path);
	al_destroy_path(data);
}

void SetConfigOption(struct Game *game, char* section, char* name, char* value) {
	al_set_config_value(game->_priv.config, section, name, value);
}

const char* GetConfigOption(struct Game *game, char* section, char* name) {
	return al_get_config_value(game->_priv.config, section, name);
}

const char* GetConfigOptionDefault(struct Game *game, char* section, char* name, const char* def) {
	const char* ret = GetConfigOption(game, section, name);
	if (!ret) return def; else return ret;
}

void DeinitConfig(struct Game *game) {
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
	ALLEGRO_PATH *data = al_create_path("SuperDerpy.ini");
	al_make_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	al_join_paths(path, data);
	al_save_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), game->_priv.config);
	al_destroy_path(path);
	al_destroy_path(data);
	al_destroy_config(game->_priv.config);
}
