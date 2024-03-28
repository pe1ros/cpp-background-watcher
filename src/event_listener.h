#pragma once

#include <iostream>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "file_reader.h"
#include "time_formatter.h"

wchar_t keyCodeToChar(int keyCode);

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);