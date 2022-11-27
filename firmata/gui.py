#!/usr/bin/env python
import pyfirmata
import time
import tkinter

def onStartButton ():
    period = float (timePeriod.get())
    brillance = float (brightness.get ())
    startButton.config (state=tkinter.DISABLED)
    led_jaune.write (brillance/100.0)
    led_vert.write (vert.get ())
    led_rouge.write (rouge.get ())
    startButton.config (state=tkinter.ACTIVE)

def onStopButton ():
    led_jaune.write (0)
    led_vert.write (0)
    led_rouge.write (0)

def onExitButton ():
    led_jaune.write (0)
    led_vert.write (0)
    led_rouge.write (0)
    board.exit ()
    top.quit ()

port = 'COM10'
board = pyfirmata.Arduino (port)

led_jaune = board.get_pin ('d:3:p')
led_vert = board.get_pin ('d:8:o')
led_rouge = board.get_pin ('d:9:o')

top = tkinter.Tk ()
top.title ('LED')
top.minsize (300,200)

timePeriod = tkinter.Entry (top, bd=5, width=25)
timePeriod.grid (column=1, row=1)
tkinter.Label (top, text='Time').grid (column=2, row=1)

brightness = tkinter.Scale (top, from_=0, to=100, orient=tkinter.HORIZONTAL)
brightness.grid (column=1, row=2)
tkinter.Label (top, text="Brillance").grid (column=2, row=2)

rouge = tkinter.IntVar ()
rougeBox = tkinter.Checkbutton (top, text="LED ROUGE", variable=rouge)
rougeBox.grid (column=3, row=1)
vert = tkinter.IntVar ()
vertBox = tkinter.Checkbutton (top, text="LED Verte", variable=vert)
vertBox.grid (column=3, row=2)

startButton = tkinter.Button (top, text="Début", command=onStartButton)
startButton.grid (column=1, row=3)
stopButton = tkinter.Button (top, text="Arrêt", command=onStopButton)
stopButton.grid (column=2, row=3)   
exitButton = tkinter.Button (top, text="Sortie", command=onExitButton)
exitButton.grid (column=3, row=3)

timePeriod.focus_set ()
try:
    top.mainloop()
except KeyboardInterrupt:
    board.exit()
