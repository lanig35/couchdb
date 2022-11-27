#! /usr/bin/env python
# coding: utf-8

import sqlite3
from hashids import Hashids
from flask import Flask, render_template, url_for, redirect, flash, request
from flask_bootstrap import Bootstrap

def get_db ():
    conn = sqlite3.connect ('short_url.db')
    conn.row_factory = sqlite3.Row
    return conn

app = Flask (__name__)
app.config ['SECRET_KEY'] = "random string"

Bootstrap (app)

Hash_Ids = Hashids (min_length=4, salt=app.config['SECRET_KEY'])

@app.route ('/', methods=['GET','POST'])
def index ():
    conn = get_db ()

    if request.method == 'POST':
        url = request.form ['url']

        if not url:
            flash ('URL required')
            return redirect (url_for('index'))

        url_data = conn.execute ('INSERT INTO urls (original_url) VALUES (?)', (url,))
        conn.commit ()
        conn.close ()

        url_id = url_data.lastrowid
        hash_id = Hash_Ids.encode (url_id)
        short_url = request.host_url + hash_id

        return render_template ('index.html', short_url=short_url)

    return render_template ('index.html')

@app.route ('/<id>', methods=['GET'])
def get_url (id):
    conn = get_db ()

    original_id = Hash_Ids.decode (id)
    if original_id:
        original_id = original_id[0]
        url_data = conn.execute ('SELECT original_url, clicks FROM urls \
                                  WHERE id = (?)', (original_id,)).fetchone()
        
        original_url = url_data['original_url']
        clicks = url_data['clicks']

        conn.execute ('UPDATE urls SET clicks = ? WHERE id = ?', (clicks+1, original_id,))

        conn.commit ()
        conn.close ()

        print (original_url)
        return redirect (original_url)
    else:
        flash ('invalid URL')
        return redirect (url_for('index'))

@app.route ('/stats', methods=['GET'])
def stats():
    conn = get_db ()

    db_urls = conn.execute ('SELECT id,created,original_url,clicks from urls').fetchall()
    conn.close ()

    urls = []
    for url in db_urls:
        url = dict(url)
        url['short_url'] = request.host_url + Hash_Ids.encode(url['id'])
        urls.append (url)

    return render_template ('stats.html', urls=urls)
