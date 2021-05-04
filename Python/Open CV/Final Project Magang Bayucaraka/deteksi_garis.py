import cv2
import numpy as np 
import copy

img = cv2.imread("tesline.jpg")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray, 75, 150, apertureSize=3)

backup = copy.deepcopy(img)
backupedge = copy.deepcopy(edges)
backupcustom = None

def nothing(x):
    pass

def showTrackbar(a, b, c):
    #Create Trackbar
    cv2.namedWindow("Track")
    cv2.createTrackbar("Thresh", "Track", a, 255, nothing)
    cv2.createTrackbar("line gap", "Track", b, 255, nothing)
    cv2.createTrackbar("line len", "Track", c, 255, nothing)
    cv2.createTrackbar("Searching", "Track", 3, 3, nothing)
    cv2.createTrackbar("Method", "Track", 0, 1, nothing)

#check is line is vertical with some tolerance
def isVertical(x1, x2):
    return abs( x1 - x2 ) < 5

#check is line is horizontal with some tolerance
def isHorizontal(y1, y2):
    return abs( y1 - y2 ) < 5

#to draw line in a list
def drawLine(img, alines, searching):
    for passline in alines[searching]:
        x1, y1, x2, y2 = passline[0]
        cv2.line(img, (x1, y1), (x2, y2), (0, 0, 255), 2)

#to get point for line when using Houghline method
def calc_point(rho, theta):
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a*rho
    y0 = b*rho
    x1 = int(x0 + 1000*(-b))
    y1 = int(y0 + 1000*(a))
    x2 = int(x0 - 1000*(-b))
    y2 = int(y0 - 1000*(a))
    return x1, y1, x2, y2

def main():
    #set the font
    font = cv2.FONT_HERSHEY_SIMPLEX
    #to show trackbar
    showTrackbar(99, 4, 97)
    #to help save time, if nothing is changed, dont re'detect' the line
    old_thresh, old_gap, old_len, old_srch, old_method = 0, 0, 0, 0, 0
    #to save lines according to its type
    horizontal = []
    vertical = []
    diagonal = []
    alines = []
    #for custom image
    isCustom = False
    #some help
    print("Searching:")
    print("0 = Horizontal")
    print("1 = vertical")
    print("2 = diagonal")
    print("3 = all lines")
    print("\nMethod:")
    print("0 = HoughlineP")
    print("1 = Houghline")
    print("esc to close, c for custom")
    while True:
        #get value from trackbar
        thresh = cv2.getTrackbarPos("Thresh", "Track")
        mxlngap = cv2.getTrackbarPos("line gap", "Track")
        mnlnlen = cv2.getTrackbarPos("line len", "Track")
        searching = cv2.getTrackbarPos("Searching", "Track")
        method = cv2.getTrackbarPos("Method", "Track")

        #check if is there something different
        if(old_thresh != thresh or old_gap != mxlngap or old_len != mnlnlen or old_method != method):
            #reset variable
            horizontal = []
            vertical = []
            diagonal = []
            alines = []
            old_thresh = thresh
            old_gap = mxlngap
            old_len = mnlnlen
            old_method = method

            #detect line
            if(not isCustom):
                if(method == 0):
                    lines = cv2.HoughLinesP(backupedge, 1, np.pi/180, thresh, maxLineGap=mxlngap, minLineLength=mnlnlen)
                else:
                    lines = cv2.HoughLines(backupedge, 1, np.pi/180, thresh)
            else:
                if(method == 0):
                    lines = cv2.HoughLinesP(edges, 1, np.pi/180, thresh, maxLineGap=mxlngap, minLineLength=mnlnlen)
                else:
                    lines = cv2.HoughLines(edges, 1, np.pi/180, thresh)
                
            #get points for the line detected and check its type
            try:
                for line in lines:
                    if(method == 0):
                        x1, y1, x2, y2 = line[0]
                    else:
                        rho, tetha = line[0]
                        x1, y1, x2, y2 = calc_point(rho, tetha)
                    if(isHorizontal(y1, y2)):
                        horizontal.append([(x1, y1, x2, y2)])
                    elif(isVertical(x1, x2)):
                        vertical.append([(x1, y1, x2, y2)])
                    else:
                        diagonal.append([(x1, y1, x2, y2)])

            except TypeError:
                print("No line Detected")

            #collect all line
            alines.append(horizontal)
            alines.append(vertical)
            alines.append(diagonal)

            #reset image with no line drawn
            if(not isCustom):
                img = copy.deepcopy(backup)
            else:
                img = copy.deepcopy(backupcustom)

            if(searching < 3):
                drawLine(img, alines, searching)
            else:
                #to draw all the line
                drawLine(img, alines, 0)
                drawLine(img, alines, 1)
                drawLine(img, alines, 2)

        #when searcing other type of line, use the one we already detect
        if(old_srch != searching):
            if(not isCustom):
                img = copy.deepcopy(backup)
            else:
                img = copy.deepcopy(backupcustom)
            old_srch = searching
            if(searching < 3):
                drawLine(img, alines, searching)
            else:
                drawLine(img, alines, 0)
                drawLine(img, alines, 1)
                drawLine(img, alines, 2)

        #show detected line/s
        cv2.imshow('tes', img)
        #to break the loop
        k = cv2.waitKey(1) & 0xFF
        if(k == 27):
            break
        elif(k == ord('c')):                                                #for custom image
            isCustom = True
            cv2.destroyAllWindows()
            path = input("Please input an absolute path to the image:")
            try:
                img = cv2.imread(f'{path}')
                gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                edges = cv2.Canny(gray, 75, 150, apertureSize=3)
                backupcustom = copy.deepcopy(img)
                showTrackbar(99, 4, 97)
                old_thresh, old_gap, old_len, old_srch, old_method = 0, 0, 0, 0, 0
            except cv2.error:
                print("That is an invalid path")
                isCustom = False
                img = copy.deepcopy(backup)
                showTrackbar(99, 4, 97)
        elif(k == ord('b')):                                        #if want to go back to original image
            isCustom = False
            img = copy.deepcopy(backup)
            showTrackbar(99, 4, 97)


main()
