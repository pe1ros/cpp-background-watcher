#include "system_profiler.h"

using namespace std;

char* GetUserName() {
    char* username = getlogin();
    return  username;
}

string GetDeviceInfo() {
    io_service_t service = IOServiceGetMatchingService(
        kIOMasterPortDefault,
        IOServiceMatching("IOPlatformExpertDevice")
    );

    if (service) {
        CFStringRef model = (CFStringRef)IORegistryEntryCreateCFProperty(
            service,
            CFSTR("model"),
            kCFAllocatorDefault,
            0
        );

        if (model) {
            return CFStringGetCStringPtr(model, kCFStringEncodingUTF8);
            CFRelease(model);
        }

        IOObjectRelease(service);
    }

    return ""s;
}

tuple<string, string> GetDeviceIP() {
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    string interface, ip_address;

    if (getifaddrs(&ifap) == 0) {
        for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr != nullptr && ifa->ifa_addr->sa_family == AF_INET) {
                sa = (struct sockaddr_in *) ifa->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                interface =ifa->ifa_name ;
                ip_address = addr;
            }
        }
        freeifaddrs(ifap);
    }

    return make_tuple(interface, ip_address);
}


char* GetHostName() {
    // Максимальное значение HOST_NAME_MAX = 256, определенным в <limits.h>.
    char host_name[256];
    if (gethostname(host_name, sizeof(host_name)) == 0) {
        return host_name;
    } else {
        return nullptr;
    }
}