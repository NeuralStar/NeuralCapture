#pragma once

///  Includes  ///

#include "capture.h"
#include "training.h"

#include <thread>

///  Functions  ///

int			launchCapture(Data* const data);
int			launchTraining(Data* const data);

int			launchSingle(int (*entrypoint)(Data* const));
int			launchThreads();