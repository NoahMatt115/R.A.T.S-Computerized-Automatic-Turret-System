import cv2
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    #pretend these are the values that the opencv is outputting
    x_angle = 50
    y_angle = 34
    distance = 65
    target_detected = True

    # Send all 3 values in one line
    sent_values = f"{x_angle},{y_angle},{distance},{target_detected}\n"
    message = sent_values.encode()
    ser.write(message)
    
    cv2.imshow("Frame", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
ser.close()
cv2.destroyAllWindows()
