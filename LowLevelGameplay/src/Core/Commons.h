#pragma once
#include <shared_mutex>
#define FIXED_FRAMERATE (1.f/60.f)
#define PHYSICS_SOLVER_ITERATIONS 5
#define UNIT_SCALEFACTOR 100

extern std::condition_variable_any EndOfFrame;
extern std::condition_variable_any NextUpdate;
extern std::condition_variable_any NextFixedUpdate;
