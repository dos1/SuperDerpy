#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Configuration manager code.

   Please keep in mind that config file has to be well-formed in order to function properly.

   Lines starting with '[' contain section name. No additional whitespace
   between braces and name is allowed.

   Section CANNOT be declared multiple times.

   Lines starting with '#' are ignored.
   All other lines have to look like this one:
     key=value
   All whitespace at beginning, end or around '=' char will belong to key or value.
   If multiple '=' are present, the first one is used to split line into key and value.

   Keys CANNOT be repeated in the same section.

   Rewriting config file removes all comments from it.

   If config file is not following this guides, it may be handled incorrectly.
*/

struct ConfigOption {
	char* name;
	char* value;
	char* section;
	struct ConfigOption *next;
};

struct ConfigOption *config;

void AppendToConfig(char* section, char* name, char* value) {
	struct ConfigOption *new = malloc(sizeof(struct ConfigOption));
	new->next = NULL;
	strcpy(new->section, section);
	strcpy(new->name, name);
	strcpy(new->value, value);
	if (config==NULL) config = new;
	struct ConfigOption *old = config;
	while (old->next != NULL) {
		if (!strcmp(old->section, section)) break;
		old=old->next;
	}
	if (old->next) {
		new->next = old->next;
		old->next = new;
	} else {
		old->next = new;
	}
}

void InitConfig() {
	FILE *file = fopen("SuperDerpy.ini","r+");
	if (! file) { return; }
	char string[255];
	char section[255] = "[MuffinAttack]";
	struct ConfigOption *old = NULL;
	while ( fgets (string , 255 , file) != NULL ) {
		if (string[0]=='#') { continue; } 
		if (string[strlen(string)-1]=='\n') string[strlen(string)-1]='\0';
		if (string[0]=='[') { strcpy(section, string); continue; } 
		bool before=true;
		struct ConfigOption *new = malloc(sizeof(struct ConfigOption));
		if (old==NULL) {
			config = new;
			old = new;
		} else { old->next = new; old = new; }
		new->section = malloc(sizeof(char)*255);
		strcpy(new->section, section);
		new->name = malloc(sizeof(char)*255);
		new->name[0] = '\0';
		new->value = malloc(sizeof(char)*255);
		new->value[0] = '\0';
		for (int i=0; i<strlen(string); i++) {
			if (string[i]=='=') { before=false; continue; }
			char temp[2];
			temp[0]=string[i];
			temp[1]='\0';
			if (before) strcat(new->name, temp);
			else strcat(new->value, temp);
			//printf("%s", temp);
		}
	}
	fclose(file);
}

void SetConfigOption(char* section, char* name, char* value) {
	struct ConfigOption *old = config;
	while (old!=NULL) {
		if (!strcmp(section, old->section) && !strcmp(name, old->name)) {
			strcpy(old->value, value);
			return;
		}
		old=old->next;
	}
	AppendToConfig(section, name, value);
}

char* GetConfigOption(char* section, char* name) {
	struct ConfigOption *old = config;
	char *ret = malloc(sizeof(char)*255);
	while (old!=NULL) {
		if (!strcmp(section, old->section) && !strcmp(name, old->name)) {
			strcpy(ret, old->value);
			return ret;
		}
		old=old->next;
	}
	free(ret);
	return NULL;
}

char* GetConfigOptionDefault(char* section, char* name, char* def) {
	char* ret = GetConfigOption(section, name);
	if (!ret) return def; else return ret;
}

void DeinitConfig() {
	FILE *file = fopen("SuperDerpy.ini","w");
	char section[255] = {};
	struct ConfigOption *old = config;
	while (old!=NULL) {
		if (strcmp(section, old->section)) {
			strcpy(section, old->section);
			fprintf(file, "%s\n", section);
		}
		fprintf(file, "%s=%s\n", old->name, old->value);
		struct ConfigOption *prev = old;
		old=old->next;
		free(prev->name);
		free(prev->value);
		free(prev->section);
		free(prev);
	}
	fclose(file);
}
