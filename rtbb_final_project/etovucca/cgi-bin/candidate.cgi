#!/usr/bin/env python3

import cgi
import subprocess
import json
from os import environ
from http.cookies import SimpleCookie

PATH_TO_MACHINE = "./etovucca"

PATH_TO_UPLOADS = "./candidate_info"


print("Content-Type: text/html")
print("Cache-Control: no-store, must-revalidate")
print()
print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
print('<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>')
print('<h1 id="candidate">Candidate Interface</h1>')




def save_introduction(text):
    with open('./candidate_info/introduction.txt', 'w') as file:
        file.write(text)
    return 'Your introduction was saved successfully.'


print("<p>Introduction</p>")

form = cgi.FieldStorage()
if "introduction" in form:
    introduction = form.getvalue("introduction")
    message = save_introduction(introduction)
    print("<p>{}</p>".format(message))

print("""
<form action="./candidate.cgi" method="post">
    <p> <textarea name="introduction" rows="10" cols="30"></textarea></p>
    <p><input type="submit" value="Save"></p>
</form>
""")
print("</body></html>")






