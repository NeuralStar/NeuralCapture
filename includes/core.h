#pragma once

///  Includes  ///

#include "capture.h"
#include "training.h"

#include <thread>
#include <nlohmann/json.hpp>

///  Functions  ///

int			launchCapture(data_type const data);
int			launchTraining(data_type const data);

bool		loadConfig(const std::string& path);
int			launchSingle(int (*entrypoint)(data_type const));
int			launchThreads();