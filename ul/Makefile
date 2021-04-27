FILE1 = HTTP_requester_ul.cpp

all: HTTP_requester_ul

HTTP_requester_ul: HTTP_requester_ul.cpp Makefile
	arm-linux-g++ -lcurl -Wno-psabi HTTP_requester_ul.cpp -o Twitch_IR

clean:
	rm -rf Twitch_IR*
