#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weather.hpp"
#include "const.h"

String Weather::getWeatherInfo(void)
{
	HTTPClient http;
	http.begin(WEATHER_ENDPOINT + WEATHER_API_KEY);
	int httpCode = http.GET();
	String weather = "Error";
	if (httpCode > 0)
	{
		String payload = http.getString(); // JSON形式の結果を取得
		Serial.println(httpCode);
		Serial.println(payload);

		// 必要なバッファサイズを決定
		StaticJsonDocument<1024> doc;

		// JSONを解析
		DeserializationError error = deserializeJson(doc, payload);

		if (error)
		{
			Serial.print("deserializeJson() failed: ");
			Serial.println(error.c_str());
			return weather; // エラーの場合はデフォルト値を返す
		}

		// JSONデータから値を取得
		const char *mainWeather = doc["weather"][0]["main"];
		if (mainWeather != nullptr)
		{
			weather = String(mainWeather);
		}
	}
	else
	{
		Serial.print("HTTP GETエラー: ");
		Serial.println(http.errorToString(httpCode));
	}

	http.end(); // HTTP接続を終了
	return weather;
}