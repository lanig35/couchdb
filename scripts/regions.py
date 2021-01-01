#! /usr/bin/env python
# coding: utf-8

import json
import requests
import time

url_logo = 'https://www.regions-departements-france.fr/images/regions/logo/logo-region-'

# récupération liste des régions 
with open ('../dataset/regions.json', mode='r', encoding='utf8') as f:
    liste_regions = json.load(f)

headers = {'Content-type':'application/json'}

r = requests.get ('http://127.0.0.1:5984/france', auth=('admin','reverse'))
if r.status_code != 404:
    requests.delete ('http://127.0.0.1:5984/france', auth=('admin','reverse'))
requests.put ('http://127.0.0.1:5984/france', auth=('admin','reverse'))

# ajout des regions dans la base
for region in liste_regions:
    print (region['nom'])

    region['type'] = 'region'

    # récupération logo 
#    r = requests.get (url_logo+region['slug']+'-300.jpg')
#    logo_headers = {'Content-type': r.headers.get('content-type')}
#    print (logo_headers)
#    flag = open ('logo.jpg', 'wb')
#    flag.write (r.content)
#    flag.close ()

    # ajout de la région
    r = requests.post ('http://127.0.0.1:5984/france/', data=json.dumps (region), auth=('admin','reverse'), headers=headers)
    rev = r.json()['rev']
    ident = r.json()['id']
    print (rev, ident)

    # ajout du logo en attachement
    param = {'rev': rev}
    logo_headers = {'Content-type':'image/svg+xml'}
    r = requests.put ('http://127.0.0.1:5984/france/'+ident+'/logo', params=param, headers=logo_headers, data= open('../../Regions/logo/'+region["nom"]+'.svg','rb'), auth=('admin','reverse'))
    print (r.text)
    time.sleep (1)
