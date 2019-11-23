#! /usr/bin/env python
# coding: utf-8

import paho.mqtt.client as mqtt
import argparse
import os

def on_connect (client, userdata, flag, rc):
    print ("connecté code " + str(rc))
    client.subscribe (userdata)

def on_message (client, userdata, msg):
    print (msg.topic + " : " + str(msg.payload))

parser = argparse.ArgumentParser (description='abonnement topic MQTT', add_help=False)
parser.add_argument ('-l', '--login', metavar='cpt', help='Compte')
parser.add_argument ('-p', '--pass', metavar='pwd', dest='pwd', help='Mot de passe')
parser.add_argument ('-h', '--help', action='help', help='Affiche ce message et quitte le programme')
parser.add_argument ('topic')

args = parser.parse_args ()

login = args.login if args.login is not None else os.getenv ('MQTT_USER')
pwd = args.pwd if not args.pwd is None else os.getenv ('MQTT_PASS')

if login is None or pwd is None:
    print ('Infos de connexion manquantes')
    exit (1)

client = mqtt.Client (userdata=args.topic)
client.username_pw_set (login, pwd)

client.on_connect = on_connect
client.on_message = on_message

client.connect ("127.0.0.1", port=1883)

client.loop_forever ()
