#pragma once

///  Includes  ///

#include <iostream>
#include <fstream>
#include <cstdint>

#include "unicorn.h"
#include "config.h"

///  Definitions  ///

typedef UNICORN_AMPLIFIER_CONFIGURATION		t_config;
typedef UNICORN_DEVICE_SERIAL				t_device;
typedef UNICORN_HANDLE						t_handle;

///  debug.cpp  ///

bool		checkConfig();
void		printAPIVersion();
bool		recordError(std::ofstream& out, float* buffer, const std::string& msg = std::string());
nullptr_t	clear(t_device* devices, t_handle* handle, const std::string &msg = std::string());

///  init.cpp  ///

t_device*	fetch_devices(void);
t_handle*	select_device(t_device* devices);

///  data.cpp  ///

bool		recordDevice(t_handle* handle);