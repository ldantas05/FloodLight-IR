
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char** argv)
{
    std::vector<std::string> accepted_values = {"red", "blue", "green", "aqua", "yellow", "orange", "purple", "pink"};
  CURLcode res;
  std::string current_code = "blue";
  struct curl_slist* headers = NULL;
  while(true){
    std::string readBuffer;
    CURL* curl = curl_easy_init();

    if (!curl) {
        cerr << "curl initialization failure" << endl;
        return 1;
    }
    //change broadcasterID and RewardID replacing the <> 
    
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitch.tv/helix/channel_points/custom_rewards/redemptions?broadcaster_id=<BROADCASTER_ID>&reward_id=<REWARD_ID>&status=UNFULFILLED&sort=NEWEST&first=1");

    headers = NULL;
    headers = curl_slist_append(headers, "Client-Id: <>"); // Add client ID here instead of <>
    headers = curl_slist_append(headers, "Authorization: Bearer <>"); // add Authorization id here instead of <>

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }
    json new_Request = json::parse(readBuffer);
    try {
        string x = new_Request["/data/0/user_input"_json_pointer].get<std::string>();;
        
        if (std::find(std::begin(accepted_values), std::end(accepted_values), x) != std::end(accepted_values)){

            if(current_code.compare(x) != 0){
            int fd = open("/dev/myIR", O_RDWR | O_NONBLOCK);
            char* c = const_cast<char*>(x.c_str());
            ssize_t written = write(fd, c, x.length());
            current_code = x;
         }
        }
        else{
            std::cout << "Value not recognized\n";
        }
    }
    catch(int e){
        std::cout << "No new channel redemptions\n";
    }

    std::this_thread::sleep_for (std::chrono::seconds(5));
    curl_easy_cleanup(curl);
    }
    

    return 0;
}