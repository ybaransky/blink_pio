#pragma once

typedef struct Config {
  char hostname[64];
  int port;
  int data;
} Config;

extern void config_init(void);
extern void config_load(const char *filename, Config &config);
extern void config_save(const char *filename, const Config &config);

extern void print_file(const char *filename);