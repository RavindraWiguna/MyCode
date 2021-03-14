import cv2
import copy

#load img
img = cv2.imread("street.jpg")

#resize
scalePercent = 40
width = int(img.shape[1] * scalePercent/100)
height = int(img.shape[0] * scalePercent/100)
dim = (width, height)
resizeImg = cv2.resize(img, dim, interpolation=cv2.INTER_AREA)

#gray
gray = cv2.cvtColor(resizeImg, cv2.COLOR_BGR2GRAY)


'''
functions
'''
def nothing(x):
    pass

def threshold_generator(source, thresh, maxval, kind):
    #len types = 8
    #to preview what this function does
    types = (cv2.THRESH_BINARY, cv2.THRESH_BINARY_INV, cv2.THRESH_MASK, cv2.THRESH_OTSU,
             cv2.THRESH_TOZERO, cv2.THRESH_TOZERO_INV, cv2.THRESH_TRIANGLE, cv2.THRESH_TRUNC)
    
    _, threshold = cv2.threshold(source, thresh, maxval, types[kind])
    return threshold

def welcome():
    print("Welcome to Thresholder Picker!")
    print("Here is the type:")
    print("0:BINARY\n1:BINARY_INV\n2:MASK\n3:OTSU\n4:TOZERO\n5:TOZERO_INV\n6:TRIANGLE\n7:TRUNC")
    print("To Threshold Custom Image, Press c :)")
    print("To Exit the Program, Press Esc")

def showTrackbar():
    #Create Trackbar
    cv2.namedWindow("Thresholder")
    cv2.createTrackbar("Thresh", "Thresholder", 127, 255, nothing)
    cv2.createTrackbar("Max Value", "Thresholder", 255, 255, nothing)
    cv2.createTrackbar("Type", "Thresholder", 0, 7, nothing)

    # create switch for ON/OFF functionality
    switch = "0:OFF\n1:ON"
    cv2.createTrackbar(switch, "Thresholder",1,1,nothing)

def main():
    showTrackbar()
    welcome()
    imgGray = copy.deepcopy(gray)
    source = imgGray
    isCustom = False
    while True:
        cv2.imshow('Image', source)

        isOn = cv2.getTrackbarPos("0:OFF\n1:ON", "Thresholder")
        if isOn == 1:
            thresh = cv2.getTrackbarPos("Thresh", "Thresholder")
            maxval = cv2.getTrackbarPos("Max Value", "Thresholder")
            kind = cv2.getTrackbarPos("Type", "Thresholder")
            source = threshold_generator(imgGray, thresh, maxval, kind)
        else:
            if isCustom:
                source = custom
            else:
                source = resizeImg

        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            print(f'Last Settings: Thresh={thresh}, Max Value={maxval}, Type={kind}')
            break
        elif k == ord('c'):
            cv2.destroyAllWindows()
            try:
                path = input("Please input an absolute path to the image:")
                custom = cv2.imread(f'{path}')
                imgGray = cv2.cvtColor(custom, cv2.COLOR_BGR2GRAY)
                source = imgGray
                isCustom = True
                showTrackbar()
            except cv2.error:
                print("That is an invalid path")
                isCustom = False
                showTrackbar()
  
    cv2.destroyAllWindows()


main()
