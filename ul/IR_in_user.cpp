#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

using std::string;
using std::copy;

int RED[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int ZERO_GAP = 90000;
int ONE_GAP = 1000500;

int build_NEC(string color, int arr[]){
	int code[32];
	if(color == "red") copy(std::begin(RED), std::end(RED), std::begin(code));
	else std::cout << "error\n";
	for (int i = 0; i < 32; i++){
		if(code[i] == 0) arr[i] = ZERO_GAP;
		if(code[i] == 1) arr[i] = ONE_GAP;
	}
	return 0;
}

int main()
{
	using namespace std::chrono;
	char url[] = "/sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/pwmchip7/pwm-7:1/enable";
	int NEC_CODE[32];
	build_NEC("red", NEC_CODE);
	for(int i = 0; i<32; i++){
		std::cout << NEC_CODE[i] << "\n";
	}
	int filedesc = open(url, O_WRONLY);
	write(filedesc, "1", 1);
	//file << 1;
	//file.close();
	microseconds timespan(8500);
	std::this_thread::sleep_for(timespan);
	//file.open(url);
	//file << 0;
	//file.close();
	write(filedesc, "0", 1);
	nanosleep((const struct timespec[]){{0,3800000}}, NULL);
	//nanoseconds nano_wait(4500000);
	//std::this_thread::sleep_for(nano_wait);
	for(int i = 0; i<32; i++){
		const struct timespec ts[]{{0, NEC_CODE[i]}};
		//file.open(url);
		//file << 1;
		//file.close();
		write(filedesc, "1", 1);
		nanosleep((const struct timespec[]){{0,350000}}, NULL);
		//nano_wait = nanoseconds{562500};
		//std::this_thread::sleep_for(nano_wait);

		//file.open(url);
		//file << 0;
		//file.close();
		write(filedesc, "0", 1);
		nanosleep(ts, NULL);
		//nano_wait = nanoseconds{NEC_CODE[i]};

		//std::this_thread::sleep_for(nano_wait);
	}
		//file.open(url);
		//file << 1;
		//file.close();
	write(filedesc, "1", 1);
	nanosleep((const struct timespec[]){{0,562500}}, NULL);
		//nano_wait = nanoseconds {562500};
		//std::this_thread::sleep_for(nano_wait);
		write(filedesc, "0", 1);
		//file.open(url);
		//file << 0;
		//file.close();
	/* code */
	return 0;
}