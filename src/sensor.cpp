#include <M5StickCPlus2.h>
#include "const.h"
#include "sensor.hpp"

Adafruit_BMP280 bmp;	 // 気圧センサBMP280用のインスタンスを作成
SensirionI2cSht4x sht4x; // 温湿度センサSHT40用のインスタンスを作成

Sensor::Sensor(void)
{
	Wire.begin(32, 33); // I2C通信初期化　SDA=32, SCL=33

	// 気圧センサ BMP280初期化
	while (!bmp.begin(0x76))
	{
		Serial.println("BMP280 fail");
		delay(1000);
	}

	// BMP280のサンプリングレートとフィルタ設定
	bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,	  // モード：ノーマル
					Adafruit_BMP280::SAMPLING_X2,	  // 温度サンプリングレート：2倍
					Adafruit_BMP280::SAMPLING_X16,	  // 圧力サンプリングレート：16倍
					Adafruit_BMP280::FILTER_X16,	  // フィルタ：16倍
					Adafruit_BMP280::STANDBY_MS_500); // 待ち時間：500ms
	Serial.println("BMP280 OK!");

	// 温湿度センサ SHT40初期化
	sht4x.begin(Wire, 0x44);
	uint32_t serialNumber;
	uint16_t error = sht4x.serialNumber(serialNumber); // シリアルナンバー取得実行
	if (error)
	{
		Serial.print("SHT40 Error: ");
		char errorMessage[256];
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	}
	else
	{
		Serial.println("SHT40 OK!");
		Serial.print("Serial Number: ");
		Serial.println(serialNumber);
	}
}

float Sensor::getLandryIndex(float &pressure, float &temperature, float &humidity)
{
	pressure = 0.0;
	temperature = 0.0;
	humidity = 0.0;

	// 圧力データ取得
	pressure = bmp.readPressure();

	// 温湿度データ取得
	uint16_t error = sht4x.measureHighPrecision(temperature, humidity);
	if (error)
	{
		char errorMessage[256];
		errorToString(error, errorMessage, 256);
		Serial.print("SHT40 Error: ");
		Serial.println(errorMessage);

		return 0;
	}
	else
	{
		float landryIndex = 0.81 * temperature + 0.01 * humidity * (0.99 * temperature - 14.3) + 46.3;
		Serial.printf("Press:%.0fhPa, Temp:%.1fc, Hum:%.1f%%, Landry Index:%.1f\n", pressure, temperature, humidity, landryIndex);

		return landryIndex;
	}
}