#!/usr/bin/env python
import csv, json
from slugify import slugify

pays = []

with open ("../dataset/démographie_pays.csv","r",encoding="ISO_8859-3") as f:
    data = csv.DictReader (f)
    for row in data:
        print (row["Pays"])
        item = {}
        item["nom"] = row ["Pays"]
        item["demographic"] = {
            "birthRate" : float(row ["Taux de natalité"]),
            "mortalityRate" : float(row ["Taux de mortalité"]),
            "lifeExpectancy" : float(row ["Espérance de vie"]),
            "infantMortality" : float(row ["Taux de mortalité infantile"]),
            "childPerWoman" : float(row["Nombre d?enfant(s) par femme"])
        }
        item["economicGrowth"] = float (row ["Taux de croissance"])
        pays.append(item)

with open ("../dataset/demographie.json", "w", encoding="utf-8") as f:
    f.write (json.dumps(pays, indent=2, ensure_ascii=False))
