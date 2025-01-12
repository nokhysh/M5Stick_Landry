#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <SensirionI2cSht4x.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_Sensor.h"


class Sensor
{
public:
	Sensor();
	~Sensor(){};
	float getLandryIndex(float& pressure, float& temperature, float& humidity);
private:
};

#endif