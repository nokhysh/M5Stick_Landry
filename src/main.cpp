#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <Ambient.h>
#include "sensor.hpp"
#include "weather.hpp"
#include "line.hpp"
#include "const.h"

M5Canvas canvas(&M5.Lcd);
Ambient ambient;
WiFiClient client;

// 初期設定 -----------------------------------------
void setup()
{
	/* 本体初期設定 */
	auto cfg = M5.config();
	StickCP2.begin(cfg);

	/* シリアル通信初期化 */
	Serial.begin(9600);
	while (!Serial)
	{
		delay(100);
	}

	// ベース画面の初期設定
	M5.Lcd.fillScreen(BLACK); // 背景色
	M5.Lcd.setRotation(1);	  // 画面向き設定（USB位置基準 0：下/ 1：右/ 2：上/ 3：左）
	M5.Lcd.setTextSize(1);	  // 文字サイズ（整数倍率）

	canvas.setTextWrap(true);								   // 改行をする（画面をはみ出す時自動改行する場合はtrue。書かないとtrue）
	canvas.createSprite(M5.Lcd.width() - 10, M5.Lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）
	canvas.setCursor(0, 0);									   // 表示座標
	canvas.setFont(&fonts::Font4);							   // フォント

	// 動作周波数を下げる
	setCpuFrequencyMhz(10);

	/* センサの初期化 */
	Sensor sensor;
}

// メイン -----------------------------------------
void loop()
{
	// LCD表示設定
	canvas.fillScreen(BLACK);					 // 画面初期化
	canvas.setFont(&fonts::lgfxJapanGothicP_28); // フォント
	canvas.setCursor(0, 0);						 // メモリ描画領域の表示座標指定

	setCpuFrequencyMhz(240);

	/* WiFi接続 */
	Serial.printf("SSID: %s\n", SSID);
	canvas.printf("Connecting to %s\n", SSID);
	WiFi.begin(SSID, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	canvas.print("\nWiFi connected!\n");
	canvas.pushSprite(&M5.Lcd, 10, 15);
	delay(1000 * 5);
	canvas.fillScreen(BLACK);					 // 画面初期化
	canvas.setCursor(0, 0);						 // メモリ描画領域の表示座標指定
	
	/* センサー情報の取得 */
	Sensor* pSensor;
	float pressure = 0.0;
	float temperature = 0.0;
	float humidity = 0.0;
	float landryIndex = pSensor->getLandryIndex(pressure, temperature, humidity);
	// canvas.printf("気圧：%.0fhPa\n", pressure / 100);
	canvas.printf("温度：%.1f℃\n", temperature);
	canvas.printf("湿度：%.1f%\n", humidity);
	canvas.printf("洗濯指数：%.1f\n", landryIndex);

	/* 天気情報の取得 */
	Weather* pWeather;
	String weatherInfo = pWeather->getWeatherInfo();
	Serial.println(weatherInfo);
	canvas.printf("Weather: %s\n", weatherInfo);
	canvas.pushSprite(&M5.Lcd, 10, 10);

	/* Ambientにセンサ情報を送信 */
	ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
	ambient.set(1, String(temperature).c_str());
	ambient.set(2, String(humidity).c_str());
	ambient.set(3, String(landryIndex).c_str());
	ambient.send();

	/* LINE通知 */
	if (weatherInfo == "Rain")
	{
		Line* pLine;
		pLine->send("気温は" + String(temperature, 0) + "C、湿度は" + String(humidity, 0) + "％です。");
		pLine->send("雨が降ります！");
	}

	/* Wi-Fi切断 */
	WiFi.disconnect(true);

	// 動作周波数を下げる
	setCpuFrequencyMhz(10);

	delay(1000 * 600); // 10分待機
}