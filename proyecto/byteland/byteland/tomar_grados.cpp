#include <chrono>

#include "Tomar_Grados.hpp"
#include "Global.hpp"
#include <math.h>

float tomar_grados(float i_degrees)
{
	return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}