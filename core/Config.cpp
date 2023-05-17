#include "config.h"

// Default values

bool			Config::training = true;
bool			Config::capture = true;

uint8_t			Config::method = 0;
std::string		Config::file = "data.csv";
std::string		Config::ip = "127.0.0.1";
uint32_t		Config::port = 8080;

uint32_t		Config::sample = 250;
uint32_t		Config::frames = 1;
double			Config::duration = 60.0;
bool			Config::signal = true;
bool			Config::allow_nopair = false;
uint32_t		Config::max_storage = 100;
double			Config::filter_sample = 300.0;
uint32_t		Config::channels = 8;
double			Config::interpolation_factor = 2;

std::string		Config::title = "Neural Training";
uint32_t		Config::height = 900;
uint32_t		Config::width = 1500;
uint32_t		Config::timer = 2;