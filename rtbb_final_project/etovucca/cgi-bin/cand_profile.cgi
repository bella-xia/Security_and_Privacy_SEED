#!/usr/bin/env python3

import os
import cgi

# Define the path to the introduction file
PATH_TO_UPLOADS = "./candidate_info"
INTRO_FILE = os.path.join(PATH_TO_UPLOADS, "introduction.txt")

print("Content-Type: text/html")
print()
print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
print('<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>')
print('<h1 id="candidate">Candidate Profile</h1>')

user_name = "[User name]"

print("<h2>{}</h2>".format(user_name))

if os.path.exists(INTRO_FILE):
    with open(INTRO_FILE, "r") as file:
        introduction = file.read()
    print("<h3>Introduction</h3>")
    print('<hr style="border: none; border-top: 2px solid #ccc;">')
    print("<p>{}</p>".format(introduction))
else:
    print("<p>No introduction found.</p>")

print("</body></html>")
