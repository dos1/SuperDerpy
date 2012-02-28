/*! \file config.h
 *  \brief Configuration manager headers.
 */

/*! \brief Reads config from file into memory. */
void InitConfig();
/*! \brief Returns value of requested config entry. */
char* GetConfigOption(char* section, char* name);
/*! \brief Returns value of requested config entry, or def if no such entry exists. */
char* GetConfigOptionDefault(char* section, char* name, char* def);
/*! \brief Sets new value of requested config entry, or created new if no such entry exists. */
void SetConfigOption(char* section, char* name, char* value);
/*! \brief Writes config from memory to file. */
void DeinitConfig();
