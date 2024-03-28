#include "file_uploader.h"

using namespace std;

size_t SendFileWithPostRequest(const string& url, const string& filePath) {
    CURL *curl;
    CURLcode res;

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, filePath.c_str(),
                     CURLFORM_END);

        headerlist = curl_slist_append(headerlist, buf);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);

        curl_formfree(formpost);
        curl_slist_free_all(headerlist);
    }
    curl_global_cleanup();

    return res;
}

void CheckAndSendFile(const string& url, const string& filePath) {
    ifstream file(filePath, ios::binary | ios::ate);
    if (file.is_open()) {
        cout << "Sending file...\n";
        if (SendFileWithPostRequest(url, filePath) == CURLE_OK) {
            cout << "File sent successfully.\n";
            string cmd = "rm -rf " + filePath;
            system(cmd.c_str());
        } else {
            cerr << "Failed to send file.\n";
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filePath << endl;
    }
}
