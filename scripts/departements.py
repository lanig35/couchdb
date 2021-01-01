#! /usr/bin/env python
# coding: utf-8

import json
import requests
import time

url_logo = 'https://www.regions-departements-france.fr/images/regions/logo/logo-region-'

# récupération liste des départements 
with open ('../dataset/depts.json', mode='r', encoding='utf8') as f:
    liste_depts = json.load(f)

headers = {'Content-type':'application/json'}

# ajout des regions dans la base
for dept in liste_depts:
    print (dept['nom'])

    dept['type'] = 'departement'

    # ajout du département
    r = requests.post (
            'http://127.0.0.1:5984/france/', 
            data=json.dumps (dept), 
            auth=('admin','reverse'), 
            headers=headers)
    rev = r.json()['rev']
    ident = r.json()['id']
    print (rev, ident)

    # ajout du logo en attachement
    param = {'rev': rev}
    try:
        data = open('../../Regions/Départements/'+dept["nom"]+'.svg','rb')
        logo_headers = {'Content-type':'image/svg+xml'}
    except IOError:
        try:
            data = open('../../Regions/Départements/'+dept["nom"]+'.jpg','rb')
            logo_headers = {'Content-type':'image/jpg'}
        except IOError:
            data = open('../../Regions/Départements/'+dept["nom"]+'.png','rb')
            logo_headers = {'Content-type':'image/png'}
    finally:
        print ("Fichier: ", data.name)

    r = requests.put ( 
            'http://127.0.0.1:5984/france/'+ident+'/logo', 
            params =param, 
            headers =logo_headers, 
            data = data,
            auth = ('admin','reverse'))
    print (r.text)
    time.sleep (1)
