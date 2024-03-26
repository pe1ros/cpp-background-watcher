#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <tuple>

#include "system_profiler.h"

bool FileExists(const std::string& filename);

void WriteToOutputFile(const std::wstring& new_string_key, const std::wstring& new_string_time, const std::string& filename);

void CreateLaunchAgentFile(char* path);