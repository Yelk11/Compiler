#ifndef IO_H
#define IO_H

char* tac_read_file(const char* filename);
void tac_write_file(const char* filename, char* outbuffer);
char* sh(const char* cmd);

#endif