# Smart Sprinkle

![Smart Sprinkle Banner](images/banner.png)

## Project Overview

Smart Sprinkle is an innovative IoT project that revolutionizes irrigation using ESP8266 NodeMCU. It combines environmental sensors, real-time data display, and manual control features to optimize and automate your watering system.

## Features

- **Sensor Integration**: Smart Sprinkle integrates a DHT sensor for real-time temperature and humidity monitoring and a moisture sensor to gauge soil moisture levels.

- **Automated Irrigation**: The system constantly monitors environmental conditions and activates water pumps when predefined criteria are met.

- **Data Logging and Display**: Sensor data, including temperature, humidity, and soil moisture, is logged and displayed on a server for real-time monitoring and historical data analysis.

- **Manual Control**: Users can effortlessly control the irrigation process using a user-friendly web interface accessible from your computer or mobile device.

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) for uploading code to the ESP8266.
- [Node.js](https://nodejs.org/) for running the server locally.
- Components: ESP8266 NodeMCU, DHT sensor, moisture sensor, and a relay.

### Installation

1. Clone this repository to your local machine.
2. Upload the ESP8266 code to your NodeMCU.
3. Set up the server:
   - Install Node.js.
   - Navigate to the project directory and run `npm install`.
   - Start the server with `node server.js`.

### Usage

- Access the server interface by opening a web browser and navigating to `http://localhost:3000`.
- Monitor sensor data, utilize manual control, and review historical data.
- Access the server remotely from your phone's web browser for easy system control.

---

Feel free to customize this template further, including adding images, specific instructions, or any additional information that fits your project's unique requirements.
