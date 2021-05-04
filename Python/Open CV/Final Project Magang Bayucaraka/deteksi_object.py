import cv2
import numpy as np 

#for easier read to set camera's height and width
HEIGHT = 3
WIDTH = 4

def nothing(x):
    pass

#to create windows with trackbar needed
def showTrackbar():
    #Create Trackbar
    cv2.namedWindow("Track")
    cv2.createTrackbar("L-0", "Track", 33, 255, nothing)
    cv2.createTrackbar("L-1", "Track", 74, 255, nothing)
    cv2.createTrackbar("L-2", "Track", 18, 255, nothing)
    cv2.createTrackbar("U-0", "Track", 121, 255, nothing)
    cv2.createTrackbar("U-1", "Track", 144, 255, nothing)
    cv2.createTrackbar("U-2", "Track", 71, 255, nothing)
    cv2.createTrackbar("HSV/BGR", "Track", 1, 1, nothing)

    cv2.namedWindow("Masaka")
    cv2.createTrackbar("Mask ON", "Masaka", 0, 1, nothing)

#some math experience to detect how close a shape to a circle
def isCircle(area, parameter, cx, cy, tx, ty):
    fr = 2*area/parameter
    r = np.sqrt(( tx - cx )**2 + ( ty - cy )**2)
    margin = fr*100/r
    if margin > 80:
        return True, margin
    else:
        return False, margin


def main():
    #set the font
    font = cv2.FONT_HERSHEY_SIMPLEX
    #get the camera
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    # Check if the webcam is opened correctly
    if not cap.isOpened():
        raise IOError("Cannot open webcam")

    #set camera
    cap.set(HEIGHT, 640)
    cap.set(WIDTH, 480)

    showTrackbar()
    #some helps
    print("0 = Hue / Blue (Max Hue = 180 & Max Blue = 255)")
    print("1 = Saturation / Green")
    print("2 = Value / Red")
    print("esc to close")
    while True:
        #get input fom the camera
        _, frame = cap.read()
        #flip it, to make it like a mirror
        frame = cv2.flip(frame, 1)
        #get the trackbar position
        L0 = cv2.getTrackbarPos("L-0", "Track")
        L1 = cv2.getTrackbarPos("L-1", "Track")
        L2 = cv2.getTrackbarPos("L-2", "Track")
        U0 = cv2.getTrackbarPos("U-0", "Track")
        U1 = cv2.getTrackbarPos("U-1", "Track")
        U2 = cv2.getTrackbarPos("U-2", "Track")
        #set the lower and upper bound
        lower_bound = np.array([L0,L1,L2])
        upper_bound = np.array([U0, U1, U2])

        #if using HSV color scheme
        isHSV = cv2.getTrackbarPos("HSV/BGR", "Track") == 0
        if(isHSV):
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            mask = cv2.inRange(hsv, lower_bound, upper_bound)
        else:
            mask = cv2.inRange(frame, lower_bound, upper_bound)

        #set kernel for morph as 5*5 square
        kernel = np.ones((5,5), np.uint8)
        #do morph close and open to reduce noice outside and inside the shape
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
        #to help calibarate
        if(cv2.getTrackbarPos("Mask ON", "Masaka") == 1):
            cv2.imshow("mask", mask)
        else:
            cv2.destroyWindow("mask")

        #detect the contour
        contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        
        #total objek
        total = 0
        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            #if morph is not enough, filter with area and perimeter
            if( area > 275 and perimeter > 275):
                #approximate the polygon
                apporx = cv2.approxPolyDP(cnt, 0.032*cv2.arcLength(cnt, True), True)
                #top x and y coordinate for math experiment
                tx, ty = apporx.ravel()[0], apporx.ravel()[1]

                #detect the center
                M = cv2.moments(cnt)
                cx = int(M['m10']/M['m00'])
                cy = int(M['m01']/M['m00'])

                #how far from the center to make a square with the same area + 5 margin
                mov = round(np.sqrt(area/4))+5
                x1, y1 = cx+mov, cy + mov
                x2, y2 = cx-mov, cy-mov

                #detect is the shape is like a circle and how close to being one
                yescle, margin =isCircle(area, perimeter, cx, cy, tx, ty)
                
                #draw the contour, polygon and rectangle
                cv2.drawContours(frame, [cnt], 0, (0,255,0), 2)
                cv2.drawContours(frame, [apporx], 0, (0, 0, 0), 3)
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 255), 1)

                #if kinda like a cicle, put a text
                if(yescle):
                    cv2.putText(frame ,f'{round(margin)}% Circle', (cx+30, cy+30), font, 1, (255, 165, 0), 2)
                cv2.putText(frame ,f'{len(apporx)}', (cx, cy), font, 2, (255, 0, 0), 3)
                #add 1 shape
                total+=1
        
        #show how many object is detected
        cv2.putText(frame, f'Banyak objek :{total}', (25, 50), font, 1.5, (127, 255, 127), 3)
        #show the frame
        cv2.imshow("Frame", frame)
        k = cv2.waitKey(1) & 0xFF
        #to close the loop
        if k == 27:
            cap.release()
            cv2.destroyAllWindows()
            break

main()
