#! /usr/bin/env python
# coding: utf-8

import click

from flask import Flask, render_template, url_for, redirect, flash, request, make_response

from flask_bootstrap import Bootstrap

import logging
from pathlib import Path

CHUNK_SIZE = 1024 * 1024
video_path = Path ("C:/Users/blanc/Videos/Nena_-_99_Luftballons_with_Lyrics.mp4")

app = Flask (__name__)
app.config ['SECRET_KEY'] = "random string"
Bootstrap (app)

logging.basicConfig (level=logging.DEBUG)

@app.cli.command ('create-db', help='db name')
@click.argument ('name')
def create_db (name):
    pass

@app.route ('/', methods=['GET'])
def index ():

    return render_template ('video.html')

@app.route ('/video', methods=['GET'])
def video ():
    plage = request.headers.get ('range')
    start,end = plage.replace ('bytes=','').split ('-')

    start = int(start)
    end = int(end) if end else start+CHUNK_SIZE
    
    with open (video_path,'rb') as video:
        video.seek (start)
        data = video.read (end - start)
        filesize = str (video_path.stat().st_size)

        response = make_response (data, 206)
        response.headers['Content-Range'] = f'bytes {str(start)}-{str(end)}/{filesize}'
        response.headers['Accept-Ranges'] = 'bytes'
        response.headers['Content-Type'] = 'video/mp4'

        return response
