#pragma once

#include <string>
#include <tuple>
#include <iostream>

#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

char* GetUserName();

// TODO: check this method later, its invoke crash
std::string GetDeviceInfo();

std::tuple<std::string, std::string> GetDeviceIP();

char* GetHostName();

void ReloadLaunchConfiguration();