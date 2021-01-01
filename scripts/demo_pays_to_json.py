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
        item["natalite"] = float(row ["Taux de natalité"])
        item["mortalite"] = float(row ["Taux de mortalité"])
        item["esperance"] = float(row ["Espérance de vie"])
        item["croissance"] = float (row ["Taux de croissance"])
        item["enfant"] = float (row ["Nombre d?enfant(s) par femme"])
        pays.append(item)

with open ("../dataset/demographie.json", "w", encoding="utf-8") as f:
    f.write (json.dumps(pays, ensure_ascii=False))
