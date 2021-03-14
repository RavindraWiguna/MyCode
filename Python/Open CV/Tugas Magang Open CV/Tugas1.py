import cv2
import os
#load image
img = cv2.imread("misellia.jpg")

#crop
crop_img = img[:, 100:650]

#resize
scalePercent = 60
width = int(crop_img.shape[1] * scalePercent/100)
height = int(crop_img.shape[0] * scalePercent/100)
dim = (width, height)
resizeImg = cv2.resize(crop_img, dim, interpolation=cv2.INTER_AREA) 

'''
some other function
'''
#rotate
(h, w) = resizeImg.shape[:2]
center = (w/2, h/2)
Matrix = cv2.getRotationMatrix2D(center, 180, 1.0)
rotatedImg = cv2.warpAffine(resizeImg, Matrix, (w, h))

#blur
imgBlur = cv2.GaussianBlur(resizeImg, (7, 7), 0)

#canny
imgCanny = cv2.Canny(resizeImg, 100, 200)

#gray->blur->edges
grayImg = cv2.cvtColor(resizeImg, cv2.COLOR_BGR2GRAY)
blurred = cv2.medianBlur(grayImg, 5)
edges = cv2.adaptiveThreshold(blurred, 255, 
cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 9, 9)

#cartoonize
color = cv2.bilateralFilter(resizeImg, 9, 250, 250)
cartoon = cv2.bitwise_and(color, color, mask=edges)

#to show an array of images
def showImgs(images):
    for index, image in enumerate(images):
        cv2.imshow(str(index), image)

def detectNameFormat(string):
    string = string[::-1]
    point = string.index(".")
    slash = string.index("\\")
    name = string[slash-1:point:-1]
    formatFile = string[point-1::-1]
    return name, formatFile   

def showProp(image, path):
    print("Image Properties")
    print("Shape  :",image.shape)
    print("Size   :", os.path.getsize(path)/1000, "KB")
    print("Path   :", path)
    name, formatFile = detectNameFormat(path)
    print("Name   :", name)
    print("Format :", formatFile)

def main():
    print("Welcome to Image Viewer!, if you want to view other image, you can press c")
    print("1st Run: Show Some OpenCV Functions")
    print("\nPress Esc to Close Program")
    isCustom = False
    images = [img, resizeImg, rotatedImg, imgBlur, imgCanny, cartoon, grayImg]
    while True:
        if(not isCustom):
            showImgs(images)
        else:
            cv2.imshow("Press M to View Misellia :D", source)
            
        
         #ASCII esc = 27, and waitkey 1 = wait 1ms
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
        #c for custom
        elif k == ord('c'):
            isCustom = True
            cv2.destroyAllWindows()
            path = input("Please input an absolute path to the image:")
            try:
                source = cv2.imread(f'{path}')
                showProp(source, path)
            except AttributeError:
                print("That is an invalid path")
                isCustom = False
        #m for misel
        elif k == ord('m'):
            cv2.destroyAllWindows()
            isCustom = False
            continue

main()
