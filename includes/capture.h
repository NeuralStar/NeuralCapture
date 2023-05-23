#pragma once

///  Includes  ///

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

#include "unicorn.h"

#include "Data.h"
#include "Timer.h"
#include "config.h"

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
void		writeValues(std::ofstream& out, float* buffer, uint32_t& buffer_size, Data* const data, const long long &dur, const std::vector<int> &eyes);
void		writeHorizon(std::string &buff, const std::vector<int> &eyes);
void		writeDirectives(std::string& buff, Data* const data);

bool				tracker_init(void);
std::vector<int>	tracker_getData(void);
void				tracker_stop(void);

bool		recordDevice(t_handle* handle, Data* const data);
void		filterBuffer(float* buffer, uint64_t time_diff);

bool		new_connection(const std::string& ip, const int& port);
bool		send_data(const std::string& buffer);
bool		stop_connection();