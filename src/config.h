#pragma once
#define HOSTNAME_SIZE 64
#define PASSWORD_SIZE 64
#define FILENAME_SIZE 64

typedef struct WifiParmeters {
  char      hostname[HOSTNAME_SIZE];
  char      password[PASSWORD_SIZE];
  int16_t   channel;
  void      print(void);
  void      init(void);
} WifiParameters;

class Config {
  public:
    char            filename[FILENAME_SIZE];
    WifiParameters  wifiParameters;
    int             data;

    Config() {};
    
    void  init(const char* filename);
    void  load(void);
    void  save(void);
    void  print(void);
    void  printFile(void);
};

extern Config gConfig;