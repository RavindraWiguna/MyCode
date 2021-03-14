import cv2
import copy
import os

img = cv2.imread("misellia.jpg")

#just a normal function to crop and resize in case invalid path is given
def crop_resize_misel():
    global img
    img = cv2.imread("misellia.jpg")
    #crop
    crop_img = img[:, 100:650]

    #resize
    scalePercent = 75
    width = int(crop_img.shape[1] * scalePercent/100)
    height = int(crop_img.shape[0] * scalePercent/100)
    dim = (width, height)
    img = cv2.resize(crop_img, dim, interpolation=cv2.INTER_AREA)
    return img

#do this! :D
crop_resize_misel()
backup = copy.deepcopy(img)

def split(img):
    #copy the image data
    red = copy.deepcopy(img)
    blue = copy.deepcopy(img)
    green = copy.deepcopy(img)

    '''
    BGR
    to get B, G and R need to be 0 and so on
    '''
    red[:,:,0] = 0
    red[:,:,1] = 0

    blue[:,:,1] = 0
    blue[:,:,2] = 0

    green[:,:,0] = 0
    green[:,:,2] = 0

    return red, green, blue, img

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
    global img, backup
    print("Welcome to BGR Splitter!")
    print("Press c for Custom, Esc for closing, m for Misel :D")
    while True:
        showImgs(split(img))

        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
        elif k == ord('c'):
            cv2.destroyAllWindows()
            try:
                path = input("Please input an absolute path to the image:")
                img = cv2.imread(f'{path}')
                showProp(img, path)
            except AttributeError:
                print("That is an invalid path")
                img = backup

        elif k == ord('m'):
            crop_resize_misel()

main()
