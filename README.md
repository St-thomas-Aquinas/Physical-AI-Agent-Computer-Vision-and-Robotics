# 🛡️AI x Robotics

An AI + IoT system that uses CNN-based animal classification and an ESP32-controlled robotic arm to protect farms from wild animal intrusions.

## Problem Statement

Farmers face significant losses due to wild animals invading their farms. Traditional solutions like scarecrows, fences, or human patrols are costly, unsafe, and often ineffective. Our system provides a smart, automated, and non-lethal deterrent to protect crops while promoting human-wildlife coexistence.

## How It Works

A Convolutional Neural Network (CNN) model classifies detected objects into:

["Cows", "Goats", "Hippo", "Humans", "Hyenas", "Monkeys", "Elephants"]


If the detected class is a wild animal, the system triggers a shoot action (robotic arm or deterrent).

If the class is Human, the system does NOT activate (to prevent harm).

The CNN model runs in Python (Flask app), which sends a GET request to the ESP32 when a threat is detected.

The ESP32 receives the request and activates the servo motor to scare the animal away.

## 🛠️ Tech Stack

AI/ML: Python, TensorFlow/Keras (CNN model)

Backend: Flask (app.py) for serving predictions & sending commands

IoT: ESP32 + Servo motor

Communication: HTTP (Wi-Fi GET requests from Flask to ESP32)


## 🚀 Setup & Installation
1️⃣ Clone Repository
git clone https://github.com/Stella-055/animal_classification.git

cd animal_classification

2️⃣ Install Python Dependencies
pip install -r requirements.txt

3️⃣ Run Flask App
python app.py


This runs the CNN model for real-time classification.

If a wild animal is detected → app.py sends a GET request:

http://<ESP32-IP>/shoot


### 🎯 Example Workflow

CNN detects Elephant → Flask sends GET /shoot → ESP32 moves servo → Arm/scare device activates.

CNN detects Human → No action triggered.


