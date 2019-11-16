#! /usr/bin/env python
# coding: utf-8

import os
import requests
import json

import argparse
parser = argparse.ArgumentParser (description='Suivi des mises à jour de bases', add_help=False)
parser.add_argument ('-l', '--login', metavar='cpt', help='Compte administrateur')
parser.add_argument ('-p', '--pass', metavar='pwd', dest='pwd', help='Mot de passe')
parser.add_argument ('-h', '--help', action='help', help='Affiche ce message et quitte le programme')
args = parser.parse_args ()

login = args.login if args.login is not None else os.getenv ('COUCH_ADMIN')
pwd = args.pwd if not args.pwd is None else os.getenv ('COUCH_ADMIN_PWD')

if login is None or pwd is None:
    print ('Infos de connexion manquantes')
    exit (1)

header = {'Content-type': 'application/json'}
param = {'since':'now', 'feed':'continuous', 'heartbeat':'true'}

# abonnement aux évènements sur les bases de données
r = requests.get (
    'http://localhost:5984/_db_updates', 
    headers=header, 
    params=param, 
    auth=(login, pwd), 
    stream=True
    )

if r.status_code != 200:
    print (r.status_code, ': ',r.json())
    exit (1)

# lecture en continue des changements
try:
    for line in r.iter_content(chunk_size=512):
        # omission ligne vide du heartbeat
        if line != b'\n':
            try:
                print (json.loads(line))
            except json.JSONDecodeError as e:
                print ('Opps! Erreur décodage JSON')
except KeyboardInterrupt as e:
    print ('Arrêt')
    exit (0)

