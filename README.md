# M5Stick_Landry

本アプリケーションは，M5StickCPlus2に温湿度情報及び天気情報を表示するアプリケーションです。
また、雨の日の場合は、LINEに通知されます。

## OS

* MAC

## ハードウェア

* M5StickCPlus2
* ENV.IV SENSOR

## ソフトウェア

* Open Weather：天気情報取得APIを使用
* LINE API：LINEに温湿度情報、天気情報を通知
* Ambient：ブラウザで温湿度情報を表示
* PlatFormIO：シリアル通信
** 各種Libraryはplatformio.iniに記載

## ファイル構成

``` txt
.
├── README.md
├── include
│   └── README
├── lib
│   └── README
├── partitions.csv
├── platformio.ini
└── src
    ├── const.h ※
    ├── line.cpp
    ├── line.hpp
    ├── main.cpp
    ├── sensor.cpp
    ├── sensor.hpp
    ├── weather.cpp
    └── weather.hpp
```
※ src/const.hは定数ファイルです。
WifiのID・パスワード、Open WeatherのAPI Token、LINE APIのID・API Token、AmbientのCH名・API Tokenが記載されているため、各自で設定してください。

## インストール手順

1. Open WeatherでAPI Tokenを発行する

2. LINE APIでAPI Tokenを発行する

3. AmbientでAPI Tokenを発行する

4. VScodeにPlatFormIOをインストールする

5. リポジトリの取得

    ``` py
    git init
    git remote add origin https://github.com/nokhysh/M5Stick_Landry.git
    git pull origin main
    ```

## 使用法

1. M5StickCPlus2とSerial接続 

2. PlatFormIOでBuild

3. Upload

## 作成者

* 林 尚毅