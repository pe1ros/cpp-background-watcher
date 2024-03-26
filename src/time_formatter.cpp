#include "time_formatter.h"

using namespace std;

string getTimeStamp(const char* format) {
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char time_string[100];
    strftime(time_string, sizeof(time_string), format, local_time);

    return time_string;
}
