#include "Config.h"

// Default values

bool			Config::training = true;
bool			Config::capture = true;

std::string		Config::file = "data.csv";
uint32_t		Config::sample = 250;
uint32_t		Config::frames = 1;
double			Config::duration = 60.0;
bool			Config::signal = true;
bool			Config::allow_nopair = false;

std::string		Config::title = "Neural Training";
uint32_t		Config::height = 900;
uint32_t		Config::width = 1500;
uint32_t		Config::timer = 2;