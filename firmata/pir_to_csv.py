#!/usr/bin/env python
import csv
import pyfirmata
import time

def blink (fiche):
    board.digital[fiche].write (1)
    time.sleep (1)
    board.digital[fiche].write (0)
    time.sleep (1)

port = 'COM10'
board = pyfirmata.Arduino(port)

it = pyfirmata.util.Iterator (board)
it.start ()

a0 = board.get_pin ('a:0:i')
fiche_PIR = board.get_pin ('d:7:i')
fiche_Rouge = 9
board.digital[fiche_Rouge].mode = pyfirmata.OUTPUT
fiche_Vert = 8
board.digital[fiche_Vert].mode = pyfirmata.OUTPUT
fiche_PWM = 3
board.digital[fiche_PWM].mode = pyfirmata.PWM

blink (fiche_Rouge)
print ("lancement: ", board.get_firmata_version())

try:
    with open ('./sensor.csv', 'w', newline='\n') as f:
        w = csv.writer (f)
        w.writerow (["Number", "Pot", "Motion"])
        i = 1

        while True:
            motion = fiche_PIR.read ()
            if motion is not None:
                if motion is True:
                    blink (fiche_Rouge)
                else:
                    blink (fiche_Vert)
                
            pot = a0.read ()
        
            print ("pot: ", pot)
            board.digital[fiche_PWM].write (pot)

            if motion is not None:
                row = [i, pot, motion]
                w.writerow (row)
                i += 1

except KeyboardInterrupt:
    board.digital[fiche_Rouge].write (0)
    board.digital[fiche_Vert].write (0)
    board.digital[fiche_PWM].write (0)
    board.exit ()

