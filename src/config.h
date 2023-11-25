#pragma once
#define HOSTNAME_SIZE 64
#define PASSWORD_SIZE 64
#define FILENAME_SIZE 64

typedef struct WifiParmeters {
  char      _hostname[HOSTNAME_SIZE];
  char      _password[PASSWORD_SIZE];
  int       _channel;

  void      print(void);
  void      init(void);
} WifiParameters;

class Config {
  public:
    WifiParameters  _wifiParameters;
    char            _filename[FILENAME_SIZE];
    int             _data;

    Config() {};
    
    void  init(const char* filename);
    void  load(void);
    void  save(void);
    void  print(void);
    void  printFile(void);
};

extern Config gConfig;