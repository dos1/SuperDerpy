void InitConfig();
char* GetConfigOption(char* section, char* name);
char* GetConfigOptionDefault(char* section, char* name, char* def);
void SetConfigOption(char* section, char* name, char* value);
void DeinitConfig();
