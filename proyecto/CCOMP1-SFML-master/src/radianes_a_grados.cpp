#include <chrono>

#include "Radianes_a_grados.hpp"
#include "Global.hpp"

float radianes_a_grados(float i_radianes)
{
	return 180 * i_radianes / PI;
}