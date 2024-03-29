#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <cstdlib>

#include "system_profiler.h"

const std::string FILE_NAME = "system_log.txt";

bool FileExists(const std::string& filename);

void WriteToOutputFile(const std::wstring& key, const std::string& info);

void CreateLaunchAgentFile(char* path);