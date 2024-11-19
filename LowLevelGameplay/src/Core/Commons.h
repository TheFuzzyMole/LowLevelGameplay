#pragma once
#include <shared_mutex>
#define FIXED_FRAMERATE (1.f/60.f)
#define PHYSICS_SOLVER_ITERATIONS 5
#define UNIT_SCALEFACTOR 100

#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 900

extern std::condition_variable_any EndOfFrame;
extern std::condition_variable_any NextUpdate;
extern std::condition_variable_any NextFixedUpdate;
