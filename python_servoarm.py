import serial

import time

from tkinter import *

from math import *

import numpy as np

from matplotlib import pyplot as plt

from scipy import interpolate



# Constants (robot dimensions in mm)

baseHeight = 50

humerus = 50

ulna = 50

hand = 80



#ser = serial.Serial('COM3', 57600, timeout=1)



def findAngles(localHeight, localDepth, wristAngleToGround):

    global shoulder

    global elbow

    global gwristAngleToGround

    global wristAngle



    gwristAngleToGround = wristAngleToGround

    offsetsHeight = sin(radians(wristAngleToGround)) * hand

    offsetsDepth = cos(radians(wristAngleToGround)) * hand

    wristHeight = localHeight - offsetsHeight - baseHeight

    wristDepth = localDepth - offsetsDepth

    shoulderToWrist = sqrt((wristHeight ** 2) + (wristDepth ** 2))

    angle1 = atan2(wristHeight,wristDepth)

    angle2 = acos(((humerus * humerus) - (ulna * ulna) + (shoulderToWrist * shoulderToWrist)) / ((2 * humerus) * shoulderToWrist))

    shoulder = (angle1 + angle2) * 180/pi

    elbow = -(180 - (acos(((humerus * humerus) + (ulna * ulna) - (shoulderToWrist * shoulderToWrist)) / ((2 * humerus) * ulna))) * 180/pi)

    wristAngle = wristAngleToGround - elbow - shoulder



def createCommandString():

    shoulderOffset = 500

    stepsPerDegree = 2000/180

    shoulderValue = shoulder * stepsPerDegree + shoulderOffset

    elbowValue = 1500 + (elbow * stepsPerDegree)

    wristValue = 1500 + (wristAngle * stepsPerDegree)



    commandString = "a" + str("1500").zfill(4) + \

                    "b" + str(int(shoulderValue)).zfill(4) + \

                    "c" + str(int(elbowValue)).zfill(4) + \

                    "d" + str(int(wristValue)).zfill(4) + \

                    "e" + str(rotationSlider.get()).zfill(4)



    return commandString



def send_to_position():

    gotoPosition(heightSlider.get(), depthSlider.get(), angleSlider.get())



#Interface

master = Tk()

heightSlider = Scale(master, label="Height", from_=0, to=230, tickinterval=20, orient=VERTICAL, length=300)

heightSlider.set(130)

heightSlider.pack(side = 'left')



angleSlider = Scale(master, label="Angle", from_=-45, to=45, tickinterval=10, orient=VERTICAL, length=300)

angleSlider.set(0)

angleSlider.pack(side = 'left')



depthSlider = Scale(master, label="Depth", from_=0, to=180, tickinterval=20, orient=HORIZONTAL, length=300)

depthSlider.set(130)

depthSlider.pack()



rotationSlider = Scale(master, label="End Rotation", from_=500, to=2500, tickinterval=200, orient=HORIZONTAL, length=300)

rotationSlider.set(1500)

rotationSlider.pack()



B = Button(master, text ="Send to position", command = send_to_position)

B.pack()



def gotoPosition(height, depth, angle):

    try:

        findAngles(height, depth, angle)



        if (shoulder > 0 and elbow + 90 > 0):

            print(("shoulder: " + "%.2f" +

                   " elbow: " + "%.2f" +

                   " wristangle: " + "%.2f" +

                   " Commandstring: " + str(createCommandString()))

                        % (shoulder, elbow, wristAngle))

            #ser.write(createCommandString().encode())

        else:

            print("Out of boundry!")



    except ValueError:

        print("Value Error! Input new value!")



def task():

    gotoPosition(heightSlider.get(), depthSlider.get(), angleSlider.get())

    #master.after(60, task)



#master.after(60, task)

master.mainloop()
