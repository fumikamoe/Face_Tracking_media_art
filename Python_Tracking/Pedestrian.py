from imutils.object_detection import non_max_suppression
from imutils import paths
import numpy as np
import imutils
import cv2


hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())


cap = cv2.VideoCapture(0)

def capture(V,H):
    ret, img = cap.read()

    image = img
    image = imutils.resize(image, width=min(400, image.shape[1]))
    orig = image.copy()

    # detect people in the image
    (rects, weights) = hog.detectMultiScale(image, winStride=(4, 4),
                                            padding=(8, 8), scale=1.05)

    for (x, y, w, h) in rects:
        cv2.rectangle(orig, (x, y), (x + w, y + h), (0, 0, 255), 2)
        break


    rects = np.array([[x, y, x + w, y + h] for (x, y, w, h) in rects])
    pick = non_max_suppression(rects, probs=None, overlapThresh=0.65)


    for (xA, yA, xB, yB) in pick:
        cv2.rectangle(image, (xA, yA), (xB, yB), (0, 255, 0), 2)
        print("xA : {}, yA : {}, xB : {}, yB : {}".format(xA,yA,xB,yB))
        H = int((xB-xA) / 2 + xA)
        V = int((yB-yA) / 2 + yA)
        cv2.circle(image,(xA,yA),10,(0,0,0), -1)
        cv2.circle(image, (xB, yB), 10, (255,255,255), -1)
        cv2.circle(image, (x, y), 10, (0, 255, 0), -1)

    cv2.imshow("Before", orig)
    cv2.imshow("After", image)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        cap.release()
        cv2.destroyAllWindows()
        break

    return V,H
