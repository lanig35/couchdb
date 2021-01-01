#!/usr/bin/env python
import csv, json
from slugify import slugify

regions = []

with open ("../dataset/regions-francaises.csv","r",encoding="utf-8") as f:
    data = csv.DictReader (f)
    for row in data:
        print (row["Région"])
        region = {}
        region["nom"] = row ["Région"]
        region["slug"] = slugify (row ["Région"])
        region["demographie"] = {
                "densite" : int(row ["Densité (hab./km²)"]),
                "population" : int(row ["Population en 2019 "])}
        region["superficie"] = int(row ["Superficie (km²)"])
        region["chef-lieu"] = row ["Chef-lieu de région"]
        region["lieu"] = row ["Lieu"]
        region["departements"] = {
                "nombre" : int(row["Nombre de départements dans la Région"]), 
                "noms" : row["Départements de la Région"].split("\n")}
        region["code"] = row["Code INSEE5"]
        region["direction"] = {
                "president" : row["Président(e)"], 
                "parti" : row["Partie"]}
        regions.append(region)

with open ("../dataset/regions.json", "w", encoding="utf-8") as f:
    f.write (json.dumps(regions, ensure_ascii=False))
