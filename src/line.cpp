#include <HTTPClient.h>
#include "line.hpp"
#include "const.h"

void Line::send(String message)
{
	HTTPClient http;

	// Create a JSON payload dynamically
	String jsonPayload = "{ \"to\": \"" + String(LINE_USER_ID) + "\", \"messages\": [ { \"type\": \"text\", \"text\": \"" + String(message) + "\" } ] }";

	// Configure the HTTP client
	http.begin(LINE_API_URL);
	http.addHeader("Content-Type", "application/json");
	http.addHeader("Authorization", LINE_ACCESS_TOKEN);

	// Send the POST request with the generated JSON payload
	int httpResponseCode = http.POST(jsonPayload);

	// Handle the response
	if (httpResponseCode > 0)
	{
		Serial.printf("HTTP Response code: %d\n", httpResponseCode);
		String response = http.getString();
		Serial.println("Response:");
		Serial.println(response);
	}
	else
	{
		Serial.printf("Error occurred: %s\n", http.errorToString(httpResponseCode).c_str());
	}

	// Close the connection
	http.end();
}