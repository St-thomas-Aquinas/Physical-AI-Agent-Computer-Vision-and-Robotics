import tensorflow as tf
import cv2
import numpy as np
import requests
import time
from keras.preprocessing.image import load_img, img_to_array
from collections import deque

# === CONFIG ===

IMG_SIZE = (128, 128)

MODEL_PATH = "best_model (2).h5"  # saved model 
 
DEVICE_URL = "http://192.168.45.218/receive"  # ESP/Arduino endpoint

CONF_THRESHOLD = 0.87
COOLDOWN = 5  # seconds between sending alerts

# === LOAD MODEL ===
model = tf.keras.models.load_model(MODEL_PATH)

# === CLASS LABELS === (match your dataset order)
class_labels = ["Cows", "Goats", "Hippo", "Humans", "Hyenas", "Moneys" ,"elephant"]

# === Prediction history for smoothing ===
preds_history = deque(maxlen=5)

# === Predict Single Image ===
def predict_image(path, class_labels):
    img = load_img(path, target_size=IMG_SIZE)
    img_array = img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0) / 255.0

    prediction = model.predict(img_array, verbose=0)
    idx = np.argmax(prediction)
    conf = prediction[0][idx]

    print(f"Predicted: {class_labels[idx]} ({conf:.2%})")

    # --- If animal, send to Wi-Fi ---
    if class_labels[idx] != "Humans" and conf >= CONF_THRESHOLD:
        try:
            requests.get(f"{DEVICE_URL}")
            print(f" Alert sent: {class_labels[idx]} ({conf:.2%})")
        except:
            print("Could not reach WiFi device")

# === Predict Frame (for webcam) ===
def predict_frame(frame, model, class_labels):
    img = cv2.resize(frame, IMG_SIZE)
    img_array = img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0) / 255.0

    prediction = model.predict(img_array, verbose=0)
    preds_history.append(prediction[0])  
    avg_pred = np.mean(preds_history, axis=0)

    predicted_index = np.argmax(avg_pred)
    predicted_label = class_labels[predicted_index]
    confidence = avg_pred[predicted_index]
    return predicted_label, confidence

# === Live camera ===
def run_camera(model, class_labels):
    cap = cv2.VideoCapture(0)
    last_sent = 0

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        label, conf = predict_frame(frame, model, class_labels)

        # === Send to WiFi receiver if animal detected (with cooldown) ===
        if label != "Humans" and conf >= CONF_THRESHOLD:
            if time.time() - last_sent > COOLDOWN:
                try:
                    requests.get(f"{DEVICE_URL}?animal={label}&conf={conf:.2f}")
                    print(f"Alert sent: {label} ({conf:.2%})")
                    last_sent = time.time()
                except:
                    print("Could not reach WiFi device")

        # === Display ===
        cv2.putText(frame, f"{label} ({conf:.2%})",
                    (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
                    1, (0, 255, 0), 2)
        cv2.imshow("Live Camera Classification", frame)

        # Press "q" or "ESC" to quit
        if cv2.waitKey(1) & 0xFF in [ord("q"), 27]:
            break

    cap.release()
    cv2.destroyAllWindows()

# === Example usage ===
if __name__ == "__main__":
    # Option 1: predict a static image
    #predict_image("MTU.jpeg", class_labels)

    # Option 2: run live webcam
    run_camera(model, class_labels)
