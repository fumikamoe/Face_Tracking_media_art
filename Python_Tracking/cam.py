import cv2
cap = cv2.VideoCapture(0)

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

def capture(H2,V2):
    ret, img = cap.read()

    H_margin = 60
    W_margin = int(H_margin * 4/3)

    img = cv2.flip(img, 1)
    captured = False

    #Capture Face
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    for (x, y, w, h) in faces:
        #img = cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
        H2 = int(x + w/2)
        V2 = int(y + h/2)
        cv2.circle(img, (H2, V2), 10, (255, 255, 255), -1)
        #print(H2,V2)
        captured = True
        break

    #print(img.shape)
    cv2.rectangle(img, (W_margin, H_margin), (640-W_margin , 480 - H_margin), (255, 255, 0), 2)
    #cv2.circle(img, (H2, V2), 10, (255, 255, 255), -1)
    # Display the resulting frame

    cv2.imshow('frame',img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        cap.release()
        cv2.destroyAllWindows()

    return H2, V2, captured