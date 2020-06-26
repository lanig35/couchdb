#! /usr/bin/env python
# coding: utf-8

import json
import requests
import time

# récupération liste des pays
with open ('../dataset/countries.json', mode='r', encoding='utf8') as f:
    liste_pays = json.load(f)

# ajout des pays dans la base
for item in liste_pays.keys ():
    print (item.lower())
    r = requests.get ('https://restcountries.eu/rest/v2/alpha/'+item.lower())
    pays = r.json()

    # récupération drapeau
    r = requests.get (pays['flag'])
    headers = {'Content-type': r.headers.get('content-type')}
    flag = open ('flag.svg', 'wb')
    flag.write (r.content)
    flag.close ()

    # ajout du pays
    r = requests.put ('http://127.0.0.1:5984/pays/'+item.lower(), data=json.dumps (pays), auth=('admin','reverse'))
    rev = r.json()['rev']
    print (rev)

    # ajout du drapeau en attachement
    param = {'rev': rev}
    r = requests.put ('http://127.0.0.1:5984/pays/'+item.lower()+'/file', params=param, headers=headers, data= open('flag.svg','rb'), auth=('admin','reverse'))
    print (r.text)
    time.sleep (1)
