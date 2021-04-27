# FloodLight-IR
Using a beaglebone black, arduino send HTTP request to Twitch to get channel point redemptions to change IR RGB floodlight color.

# Required Materials
(Note, the use of an arduino is not necessary if you manage to find a library that works with your particular Kernel. In my case since I used a buildroot generated kernel that did not have a package manager. This might also be easier to do with Python.)

- Beaglebone-Black
- Arduino-Uno
- 470 Ohm Resistor
- IR LED
- Ethernet Cable(or Wifi connection)
- N2222 Transistor
- RGB IR Floodlight
- Floodlight Remote
- Breadboard
- IR Receiver

# Required Libraries

- /libcrypto.so
- /libcrypto.so.1.1
- /libssl.so
- /libssl.so.1.1
- /libcurl.so
- /libcurl.so.4
- /libcurl.so.4.7.0
- /libwebsockets.so
- /libwebsockets.so.16
- /libz.so
- /libz.so.1
- /libz.so.1.2.11
- ssl libray

You will need a g++ arm cross compiler as well as arm-linux-gnueabihf

# Step 1: Get Twitch Credentials
(Note, the best way to do this is to create a WebSocket using Pubsub, however if you find no way to do this you can use HTTP-Requester_ul.cpp changing some of the variables to make the requests).

1. Log into your Twitch console and go to the applications tab https://dev.twitch.tv/console/apps , click On (+ Register your Application).
2. Name your application any name you like, set your OAuth Redirect URL to http://localhost:8000/, select a category and register.
3. Click on the newly created application and save your ClientID and your Client Secret in a secure location.

The next step will require you to have a way to launch a server in your computer such as PHP.

4. Download the repository from https://github.com/BarryCarlyon/twitch_misc
5. Get your broadcaster ID by going to this site, https://barrycarlyon.github.io/twitch_misc/authentication/implicit_auth/ and clicking either authorization option, your ID should be under ID. Save this ID for further use.
6. In the downloaded repository navigate to /twitch_misc-main/authentication/implicit_auth and open index.html on your favorite editor. Replace the client-id variable for your client id and the redirect for http://localhost:8000/. In line 28 find this line scope=user:read:email and change the scope for what you will need channel:read:redemptions and channel:manage:redemptions.
7. Run a server on the folder with the Index file. The easiest way it to run
```php -S 127.0.0.1:8000 ```
8. You will be able to see your access key, save it in a secure location.

# Step 2: Create Channel Reward and sending Requests
Open a bash window and send the following request changing the values in between <> and ** ** following the API guide remeber to delete ** and <> :

```
curl -X POST 'https://api.twitch.tv/helix/channel_points/custom_rewards?broadcaster_id=<broadcaster_id>' \
-H 'client-id: <client_ID>' \
-H 'Authorization: Bearer <Auth ID> ' \
-H 'Content-Type: application/json' \
-d '{
    ** "title":"Change floodlight color",
    "cost":1500,
    "is_user_input_required": true **
}'
```
Find the reward ID by running the following request changing the values in <>:
```
curl -X GET 'https://api.twitch.tv/helix/channel_points/custom_rewards?broadcaster_id=<broadcaster_id>&only_manageable_rewards=true' \
-H 'Client-Id: <client_ID>' \
-H 'Authorization: Bearer <Auth ID>'

```
You can verify if the reward was created by checking your channel dashboard.

In the /ul folder you will be able to find the HTTP_requester_ul.cpp file, open it and change the required values.

**I do not own the JSON.hpp single header library this library was downloaded from https://github.com/nlohmann/json and thus you must download this rep as well, 
place the file under single_include/nlohmannin the same folder as the HTTP_requester_ul.cpp**

Cross-compile this library and place the full content of the folder in the beaglebone thorugh ssh or other methods. Makefile is provided for this file.

*Note IR_in_user.cpp was my attempt of running the PWM in the beaglebone through the user space. For this remember to export the pwm using sysfs

# Step 3: The Kernel Module and Beaglebone Preparation
Under the /km directory you will find IR_signal.ko, this is the binary for the kernel module if no changes were to be made. 

*IR_signal_fail.cpp is my failed attempt of running a PWM signal using the kernel module on P8_13 with the BB-UART4-00A0.dtbo*

Run this in the shell replacing for the right PATH:
```
mknod /dev/myIR c 61 0
insmod <PATH_TO_MODULE>/IR_signal.ko
```

You can visualize the current color of the IR by ```cat /dev/myIR```.

The following is the pinout diagram for the different colors supported.

Beaglebone Pin | Connecto To Arduino Pin | Connect to Bus |
--- | --- | --- 
P8_8 | 2 | N/A 
P8_10 | 4 | N/A
P8_12 | 5 | N/A
P8_14 | 6 | N/A 
P8_16 | 7 | N/A 
P8_18 | 8 | N/A
P8_17 | 13 | N/A
P8_26 | 12 | N/A
P9_5 | N/A | Positive
P9_2 | N/A | Ground(-)

You can echo {"red", "blue", "green", "aqua", "yellow", "orange", "purple", "pink"} into /dev/myIR to change RGB color;

To remove the module use ```rmmod IR_signal```
# Step 4: Getting Decoding IR Signal and Changing IR Color
For this you will need to download this library https://github.com/cyborg5/IRLib2 and follow the instructions to enable the libraries.
Follow the Diagram in this site https://learn.adafruit.com/using-an-infrared-library/hardware-needed but instead of using Pin 2 in The arduino use pin 3 or any other PWM pin.

Upload the Arduino Files/signal_receiver/signal_receiver.ino to your Arduino, open the serial monitor and press the buttons for the following colors:
{"red", "blue", "green", "aqua", "yellow", "orange", "purple", "pink"} 

This might be different for different users, my IR floodlight receives NEC signals with the following codes found in:
Arduino Files/signal_sender/signal_sender.ino

Adapt this last file to your need.
Setup your circuit as per the following page: https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes excluding connecting the 5V pin and the GND pin.

# Step 5: Running
Make sure that the Floodlight is on and close to where the IR LED is, if you used the makefile, navigate to where the ouput was placed and run it. You can run it in the background.

Take into account the Default color is blue, so by redeeming channel points make sure to change the color to see changes. If you open the serial monitor on the arduino you should be able to see when the Beaglebone sends a signal.




