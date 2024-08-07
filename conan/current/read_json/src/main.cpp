#include "main.h"

int main() {

	// Define JSON string
	std::string jsonString = "{ \"msg\": \"JSON read successful!\" }";

	// Parse JSON string
	auto jsonData = nlohmann::json::parse(jsonString);

	// Print JSON data
	std::cout << jsonData["msg"] << std::endl;
}