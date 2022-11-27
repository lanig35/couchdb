#! /usr/bin/env python
# coding: utf-8

import sqlite3

conn = sqlite3.connect ('short_url.db')

with open ('schema.sql') as f:
    conn.executescript (f.read())

conn.commit ()
conn.close ()
