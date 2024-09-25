<h1 align="center">TrackVision :racing_car:</h1> 

![banner](https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/153787379/132308ff-27a0-45e7-8323-80d9103f2390)

<h1 align="center">Other Languages</h1>

<p align="center">
<a href="README.md" align="center">Portuguese</a> •
<a href="README-EN.md" align="center">English</a> 
</p>

## Index :page_with_curl:

* [Project Description](#project-description-memo)
   * [Introduction](#introduction-1st_place_medal)
   * [ESP32](#ESP32-pager)
* [Project Goals](#project-goals-dart)
* [Access to the Project](#access-to-the-project-file_folder)
* [Tools Used](#tools-used-hammer_and_wrench)
* [Libraries Used](#libraries-used-books)
* [Required Components](#required-components-toolbox)
* [Assembly](#assembly-wrench)
   * [Precautions during assembly](#precautions-during-assembly-warning)
* [Reproduction](#reproduction-gear)
* [How to use TrackVision](#how-to-use-trackVision-pushpin)
* [Project Developers](#project-developers-globe_with_meridians)


## Project Description :memo:

<h3>Introduction :1st_place_medal:</h3>
<p>
We, from L.A.N.E., developed a race timing system using the ESP32, an advanced microcontroller with built-in Wi-Fi connectivity. The goal is to create an IoT device that captures lap times in real time and sends this information to the cloud, enabling performance monitoring and analysis.
</p>

<h3>ESP32 :pager:</h3>
<p>
The ESP32 is a microcontroller that facilitates internet connection, allowing data collection and transmission bidirectionally. We use technologies like Fiware for data management and Docker platform on Microsoft Azure for container orchestration and execution. Fiware provides a robust infrastructure for building smart applications, while Docker allows scalable environment management. Microsoft Azure hosts and scales these services, ensuring high availability and performance.
</p>

<p>
To monitor and visualize the data flow from the ESP32, we used Postman, a tool that facilitates executing HTTP requests and analyzing responses. With Postman, we tested the APIs, verified data integration, and ensured communication between the device and the server worked as expected.
</p>

## Project Goals :dart:

<p>
The main goal of the TrackVision project is to develop a race timing system that is efficient and easy to use. The specific objectives include:
</p>

- ``1.`` Real-Time Lap Capture:
    - ``1.1`` Implement the ability to capture lap times in real-time using the ESP32 and suitable sensors.
- ``2.`` Data Transmission:
    - ``2.1`` Create an infrastructure to send lap data to the cloud, enabling real-time monitoring and later analysis.
- ``3.`` User-Friendly Interface:
    - ``3.1`` Develop an intuitive user interface that provides clear information on the racers' performance.

- ``4.`` Scalability:
    - ``4.1`` Ensure that the system can be easily scaled to handle different types of races and numbers of participants.

- ``5.`` Robustness and Reliability:
    - ``5.1`` Ensure the system works consistently in various racing conditions and environments.

## Access to the Project :file_folder:

You can access the [project code](code/TrackVisionCode.ino) or the [simulation made in Wokwi](https://wokwi.com/projects/409286303085618177)

## Tools Used :hammer_and_wrench:

- `Arduino IDE`

## Libraries Used :books:

- ``LiquidCrystal_I2C``
- ``IRremote``
- ``Wire``
- ``RTClib``
- ``WiFi``
- ``PubSubClient``

## Required Components :toolbox:

| Component | Quantity |
|:---------:|:--------:|
|    Cables    |    10    |
|    ESP32     |    1     |
| IR Receiver  |    1     |
| IR Remote    |    1     |
| LCD 16x2 with I2C module |    1     |
| Real-Time Clock - RTC |    1     |
|    USB Cable    |    1     |

## Assembly :wrench:

<details>
  <summary>Assembly Image</summary>
  <img src="https://github.com/user-attachments/assets/d7734bf3-a731-49b7-b120-20c9a60e2b51" alt="assembly-image">
</details>

<h3>Precautions during assembly :warning:</h3>

**Attention!**
**Remember to connect the negative and positive terminals with the ESP32 to pass the current!**

- ``1.`` Connecting LCD:
    - ``1.1.`` **Attention!** We are using a 16x2 LCD with an I2C module!;
    - ``1.2.`` Connect the VCC to the positive terminal (5V), GND to the negative terminal (GND), the SDA to pin 21, and the SCL to pin 22;
    - ``1.3.`` Test to see if the display is working, if there are issues with the display, it could be some of these possibilities: the LCD is broken, has poor contact, or the contrast is too low;
        - ``1.3.1.`` To increase the display contrast, turn the contrast adjustment trimpot counterclockwise. To decrease the contrast, turn it clockwise.

        <details>
            <summary>Image showing where the contrast adjustment trimpot is located</summary>
            <img src="https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/101829188/50648d65-2402-4508-a47d-1d38bbf663e5" alt="DHT11 Terminals">
        </details>

- ``2.`` Connecting the RTC:
  - ``2.1.`` Connect the SCL to pin 22 (together with the LCD's SCL) of the Arduino, the SDA to pin 21 (together with the LCD's SDA), the 5V to the positive terminal of the breadboard, and the GND to the negative terminal of the breadboard.
  - ``2.2.`` The RTC is a delicate component. Handle it with care to avoid damage.
  - ``2.3.`` Ensure that the RTC is set with the correct time. An incorrect configuration can lead to errors in your project’s functionality.
  - ``2.4.`` Make sure the RTC is receiving the appropriate power. Insufficient or excessive power can damage the RTC or affect its operation.

- ``3.`` Connecting the IR-receiver:
  - ``3.1.`` Connect the 5V to the breadboard's positive terminal and the GND to the breadboard's negative terminal, and connect the DATA pin to pin 15.
  - ``3.2.`` The IR-receiver is a delicate component, handle it carefully.
  - ``3.3.`` Ensure the IR-receiver is receiving the proper power.

- ``4.`` Using the IR-remote:
    - ``4.1.`` Check the commands sent by each button on the remote in the documentation.

## Reproduction :gear:

- ``1.`` After assembling the project, you need to upload the code using a computer that has the Arduino IDE installed;
- ``2.`` When opening the IDE, some steps are needed to select the ESP32:
  - ``2.1.`` Click on **File** > **Preferences**.
    - ``2.1.1.`` In the **Additional Board Manager URLs** field, add the following link: *https://dl.espressif.com/dl/package_esp32_index.json*;
  - ``2.2.`` Click on **Tools** > **Boards** > **Board Manager**.
    - ``2.2.1.`` In the window that opens, type *ESP32* in the search box;
    - ``2.2.2.`` Select the *esp32* platform from the list and click Install;
  - ``2.3.`` After installation, go again to **Tools** > **Boards**.
    - ``2.3.1.`` You will see a new option to select the ESP32 boards. Choose the specific board you are using;
- ``3.`` Download the [necessary libraries](#bibliotecas-utilizadas-books) in the Arduino IDE;
- ``4.`` Make the necessary changes to the available code:
  
  ```cpp
    const char* default_SSID = "YOUR_INTERNET"; // Wi-Fi network name
    const char* default_PASSWORD = "YOUR_INTERNET_PASSWORD"; // Wi-Fi network password
    const char* default_BROKER_MQTT = "PUBLIC_IP"; // MQTT Broker IP
    ```

    - ``4.1.`` Replace the placeholders with your Wi-Fi network name and password, and your IP.
        - ``4.1.1`` We do not provide the IP for security reasons. To test this code, you will need a cloud service like Azure or AWS. Additionally, you will need to install FIWARE and Docker on this service, and finally, open the necessary ports.
- ``5`` Transfer the code from the computer to the ESP32 using a USB cable;
- ``6`` Test the system to verify if it is receiving commands and sending data via Postman;
- ``7``  Once everything is set up and ready, it is necessary to take the system to the environment where it will be implemented and power it up with a power source;

<p align='center'><i>NOTE: If the ESP32 is an older version, it may be necessary to press the BOOT button on the board during code transfer </i></p>

## How to use TrackVision :pushpin:

- ``1.`` Wait for the internet connection, checking the serial monitor.
- ``2.`` Click the POWER button, and wait for the MQTT connection.
- ``3.`` Each click on the POWER button will count a lap and display its time.
- ``4.`` On the second-to-last lap, press 0 once. After pressing 0, the next click on the POWER button will end the race.
- ``5.`` A final message will be displayed with the total race time.
- ``6.`` If you started the race by mistake, to return to the MENU, press MENU on the IR-Controller.

## Project Developers :globe_with_meridians:

| [<img src="https://avatars.githubusercontent.com/u/101829188?v=4" width=115><br><sub>Alice Santos Bulhões</sub>](https://github.com/AliceSBulhoes) |  [<img src="https://avatars.githubusercontent.com/u/163866552?v=4" width=115><br><sub>Eduardo Oliveira Cardoso Madid</sub>](https://github.com/EduardoMadid) |  [<img src="https://avatars.githubusercontent.com/u/148162404?v=4" width=115><br><sub>Lucas Henzo Ide Yuki</sub>](https://github.com/LucasYuki1) | [<img src="https://avatars.githubusercontent.com/u/153787379?v=4" width=115><br><sub>Nicolas Haubricht Hainfellner</sub>](https://github.com/NicolasHaubricht) |
| :---: | :---: | :---: | :---: |
| RM:554499 | RM:556349 | RM:554865 | RM:556259 | 
