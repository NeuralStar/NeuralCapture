#pragma once

///  Includes  ///

#include <iostream>
#include <fstream>
#include <cstdint>

#include "unicorn.h"

#include "Data.h"
#include "Timer.h"
#include "Config.h"

///  Definitions  ///

typedef UNICORN_AMPLIFIER_CONFIGURATION		t_config;
typedef UNICORN_DEVICE_SERIAL				t_device;
typedef UNICORN_HANDLE						t_handle;

///  Functions  ///

bool		checkConfig();
void		printAPIVersion();
bool		recordError(std::ofstream& out, float* buffer, const std::string& msg = std::string());
nullptr_t	clear(t_device* devices, t_handle* handle, const std::string& msg = std::string());

t_device*	fetch_devices(void);
t_handle*	select_device(t_device* devices);

bool		defineCollums(t_handle* handle, std::ofstream& out);
void		writeValues(std::ofstream& out, float* buffer, uint32_t& buffer_size);
void		writeDirectives(std::ofstream& out, Data* const data);

bool		recordDevice(t_handle* handle, Data* const data);
void		filterBuffer(float* buffer, uint64_t time_diff);