#!/usr/bin/env python
import csv, json
from slugify import slugify

depts = []

with open ("../dataset/departements-francais.csv","r",encoding="utf-8") as f:
    data = csv.DictReader (f)
    for row in data:
        print (row["Nom"])
        dept = {}
        dept["nom"] = row ["Nom"]
        dept["slug"] = slugify (row ["Nom"])
        dept["date"] = row ["Date Création"]
        dept["Organisation"] = {
                "communes" : int(row["Communes"]),
                "cantons" : int(row["Cantons"]),
                "circonscriptions" : int(row["Circonscriptions"]),
                "arrondissements" : {
                    "nombre" : int(row["Arrondissements"]), 
                    "noms" : row["Chef-lieux"].split("\n")
                    }
                }
        dept["region"] = slugify(row["Région"])
        dept["demographie"] = {
            "gentile" : row["Gentilé"].split(","),
            "population" : int(row["Population"]),
            "densite" : int(row["Densité"]),
            "variation" : float(row["Variation"])}
        dept["superficie"] = int(row ["Superficie"])
        dept["chef-lieu"] = row ["Préfecture"]
        dept["code"] = row["Code"] 
        dept["direction"] = {
                "president" : row["Président"], 
                "parti" : row["Parti"]}
        depts.append(dept)

with open ("../dataset/depts.json", "w", encoding="utf-8") as f:
    f.write (json.dumps(depts, indent=2, ensure_ascii=False))
