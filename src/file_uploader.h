#pragma once

#include <string>
#include <ctime>
#include <fstream>
#include <iostream>

#include <curl/curl.h>

size_t SendFileWithPostRequest(const std::string& url, const std::string& filePath);

void CheckAndSendFile(const std::string& url, const std::string& filePath);
