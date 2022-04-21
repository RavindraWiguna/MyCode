from os import system
import cv2
import numpy as np
ascii_col = '$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`\'.'
# b = '$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`\'.'

tc = len(ascii_col) #69
block = round(255/tc) #well 3 and 4
x_3 = 21
y_4 = 48
end_3 = x_3*3
print(end_3)

# just feel like it
def map_pix_ascii(pixel):
    if(pixel < end_3):
        # return ascii_col[round(pixel/3)]
        return ascii_col[68 - pixel//3]
    else:
        # return ascii_col[x_3 + pixel//4]
        return ascii_col[68  - (x_3-1 + (pixel - end_3)//4)]


dict_pix_ascii = {}
for i in range(256):
    # print(f'{i} = {map_pix_ascii(i)}')
    dict_pix_ascii[i] = map_pix_ascii(i)




# img = cv2.imread('download.jpeg', 0)
# cv2.imshow('test', img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

# font
font = cv2.FONT_HERSHEY_SIMPLEX

# org


# fontScale
fontScale = 0.5

# Blue color in BGR
color = (255, 255, 255)

# Line thickness of 2 px
thickness = 0

def blockize_img(img, w, h):
    ih, iw = img.shape[0], img.shape[1]
    # print(f'{iw}, {ih}')
    uw = iw//w
    uh = ih//h
    # print(f'{uw}, {uh}')
    new_frame = np.zeros((h, w), np.uint8)
    string_frame = []
    # string_frame=""
    new_frame = np.zeros((1700, 960), np.uint8)
    for row in range(h-1):
        for col in range(w-1):
            # new_frame[row:row+1, col:col+1] = int(np.average(img[uh*row: uh*(row+1), uw*col:uw*(col+1)]))
            string_frame.append(dict_pix_ascii[int(np.average(img[uh*row: uh*(row+1), uw*col:uw*(col+1)]))])
            # string_frame+=dict_pix_ascii[int(np.average(img[uh*row: uh*(row+1), uw*col:uw*(col+1)]))]
        string_frame.append("\n")
        # string_frame+="\n"
        # finish = "".join(string_frame)
        # string_frame.clear()
        # d, len = cv2.getTextSize(finish, font, fontScale, thickness)
        # org = (0, d[1]*row)
        # print(f'{d}, {len}')
        # new_frame = cv2.putText(new_frame, finish, org, font, fontScale, color, thickness, cv2.LINE_AA)
    # finish = "".join(string_frame)
    
    # cv2.getTextSize()

    
    # cv2.imshow('ggwp',new_frame)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    system("cls")
    print("".join(string_frame))
    # print(string_frame)
    # for row in range(h-1):
        # for col in range(w-1):
            # print(new_frame[row:row+1, col:col+1][0][0], end="")
            # print(dict_pix_ascii[new_frame[row:row+1, col:col+1][0][0]], end="")
        # print("")


cap = cv2.VideoCapture(0)
while cap.isOpened():
    ret, img = cap.read()
    # cv2.imshow("aa", img)
    blockize_img(img, img.shape[0]//3, img.shape[0]//6)
    k = cv2.waitKey(1) & 0xFF
    if(k == ord('q')):
        cap.release()
        cv2.destroyAllWindows()
        break

