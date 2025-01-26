# CST357 Assignment 2 - Smart Agrriculture System

## Team Members

```
Eason Peng  (159767)
Lee Ter Qin  (159389)
```

## How to run this project

### Prerequisites

1. Arduino
2. GCP
3. MongoDB

### Clone the Project

1. Git clone the project and go to the directory

  ```
  git clone https://github.com/jake1ee/CPC357_IoT_Assignment2.git
  
  ```

#### Install Required Libraries in the Arduino IDE

2. Go to the Library Manager Tools -> Manage Libraries
   
![Image](https://github.com/user-attachments/assets/bae4a403-5d6d-4501-8004-58149c8e79bc)

3. Download the Libraries shown in the Image
   
![Image](https://github.com/user-attachments/assets/825b7e0a-d8d3-4029-96cb-93227c232bb9)
![Image](https://github.com/user-attachments/assets/d3c190c4-d695-4ab6-b908-4b99ac0018fc)

Notes for the Google Cloud Platform Setup Please Refer the Report Documentation

4. Run the Code GCP_Smart_Agriculture

  ```
  // WiFi credentials
  const char* WIFI_SSID = "ssid";  
  const char* WIFI_PASSWORD = "password";
  
  const char* MQTT_SERVER = "34.30.41.100";
  const int MQTT_PORT = 1883;  
  const char* MQTT_TOPIC_TELEMETRY = "sensor_data";
  ```
Notes: Change the Wifi SSID adn Password Based on your Wifi Connection, For MQTT Server do refer from the GCP VM Instance External IP
![Image](https://github.com/user-attachments/assets/f495398a-4f52-4ffa-b73a-33c243023e55)

5. After Run the Code open Your VM Instance SSH and Upload the app.py, mongodb_code.py

![Image](https://github.com/user-attachments/assets/d25d3242-a9a5-4d31-b3d7-cf77b267c97e)

6. Create a new Directory (templates) and upload the index.html into the directory

  ```
  leeterqin81@cpc357-assignment2-gn14:~$ ls
  app.py  mongodb_code.py  templates
  ```

7. Before runing the python code there are some Libraries are used and should be install before run the code execute the command in the ssh  :
![Image](https://github.com/user-attachments/assets/f31f0307-1d86-4178-bf29-27a63ca4a007)
  ```
  pip3 install pymongo
  ```
  ```
  sudo apt install python3-pip
  ```
  ```
   pip install paho-mqtt
  ```
  ```
  Pip install flask pymongo pandas plotly
  ```

8. Run the mongodb_code to connect the MQTT Pub/Sub
  ```
  python3 mongodb_code.py
  ```
  When the code is execute a messeage will be shown
  ![Image](https://github.com/user-attachments/assets/4104fbad-d69e-4caa-9615-dfc77906bd85)
  
9. Run the Flask App
  ```
  python3 app.py
  ```
  ![image](https://github.com/user-attachments/assets/f2206f5a-659a-46ff-9243-59bbb71ab17b)
  
10. Access the dashboard by Replace the IP address of the provided link with your VM Instance External IP
    ![image](https://github.com/user-attachments/assets/1595ddee-7ab4-4b94-aab1-eb4365718b1c)
    ![image](https://github.com/user-attachments/assets/47596f1b-5b47-4d57-801e-a2e21d4dfe4c)
    
    Dashboard
    ![image](https://github.com/user-attachments/assets/b24c0449-0092-4567-b1b7-29ee8c1eea84)


