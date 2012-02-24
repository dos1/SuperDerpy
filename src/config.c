#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct ConfigOption {
	char* name;
	char* value;
	char* section;
	struct ConfigOption *next;
};

struct Config {
	FILE *file;
	struct ConfigOption *list;
};

struct Config config;

void InitConfig() {
	config.file = fopen("SuperDerpy.ini","r+");
	if (! config.file) { fopen("SuperDerpy.ini","w+"); return; }
	char string[255];
	char section[255] = "[MuffinAttack]";
	struct ConfigOption *old = NULL;
	while ( fgets (string , 255 , config.file) != NULL ) {
		if (string[0]=='#') { continue; } 
		if (string[strlen(string)-1]=='\n') string[strlen(string)-1]='\0';
		if (string[0]=='[') { strcpy(section, string); continue; } 
		bool before=true;
		struct ConfigOption *new = malloc(sizeof(struct ConfigOption));
		if (old==NULL) {
			config.list = new;
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
	fclose(config.file);
	
/*	old = config.list;
	while (old!=NULL) {
		printf("Section: %s, name: %s, value: %s\n", old->section, old->name, old->value);
		old=old->next;
	}*/
}

char* ReadConfigOption(char* section, char* name) {
	struct ConfigOption *old = config.list;
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

void DeinitConfig() {
	struct ConfigOption *old = config.list;
	while (old!=NULL) {
		struct ConfigOption *prev = old;
		old=old->next;
		free(prev->name);
		free(prev->value);
		free(prev->section);
		free(prev);
	}
}