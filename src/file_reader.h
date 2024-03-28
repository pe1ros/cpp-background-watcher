#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <cstdlib>

#include "system_profiler.h"

bool FileExists(const std::string& filename);

void WriteToOutputFile(const std::wstring& key, const std::string& info, const std::string& filename);

void CreateLaunchAgentFile(char* path);