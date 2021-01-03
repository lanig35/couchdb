#! /usr/bin/env python
# coding: utf-8

import json
import requests
import time

url_view = 'http://127.0.0.1:5984/pays/_design/Name/_view/byFrenchName'
auth=('admin','reverse')
view_headers = {"Accept": "application/json"}
put_headers = {"Content-type": "application/json"}

# récupération liste des pays
with open ('../dataset/demographie.json', mode='r', encoding='utf8') as f:
    liste_pays = json.load(f)

# parcours des pays 
for item in liste_pays:
    print (item["nom"].lower())

    payload = {
            'key': '\"'+item["nom"]+'\"',
            'include_docs': True}
    r = requests.get (url_view, params=payload, headers=view_headers, auth=auth)

    if r.status_code == 200:
        if r.json()['rows']:
            doc = r.json()['rows'][0]['doc']
            doc['demographic'] = item ['demographic']
            doc['economicGrowth'] = item ['economicGrowth']

            # mise à jour des données
            params = {'rev': doc['_rev']}
            r = requests.put (
                    'http://127.0.0.1:5984/pays/'+doc['_id'],
                    headers=put_headers,
                    auth=auth,
                    params=params,
                    data=json.dumps (doc))
            print (r.status_code)
        else:
            print ("pays inconnu")
    time.sleep (1)
