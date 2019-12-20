#! /usr/bin/env python
# coding: utf-8

import os
import pika 

import argparse
parser = argparse.ArgumentParser (description='lecture mqtt rabboitmq', add_help=False)
parser.add_argument ('-l', '--login', metavar='cpt', help='Compte')
parser.add_argument ('-p', '--pass', metavar='pwd', dest='pwd', help='Mot de passe')
parser.add_argument ('-h', '--help', action='help', help='Affiche ce message et quitte le programme')
args = parser.parse_args ()

login = args.login if args.login is not None else os.getenv ('PIKA_LOGIN')
pwd = args.pwd if not args.pwd is None else os.getenv ('PIKA_PWD')

if login is None or pwd is None:
    print ('Infos de connexion manquantes')
    exit (1)

cred = pika.credentials.PlainCredentials (login, pwd)
param = pika.ConnectionParameters(credentials=cred)
con = pika.BlockingConnection (param)

channel = con.channel ()
channel.exchange_declare (exchange='amq.topic', exchange_type='topic', durab
le=True)

r = channel.queue_declare ('alain', exclusive=True)
channel.queue_bind (exchange='amq.topic', queue='alain', routing_key='alain.
bridge')

def cb (ch, method, properties, body):
    print (body)

channel.basic_consume (queue='alain', on_message_callback=cb, auto_ack=True)
channel.start_consuming ()

