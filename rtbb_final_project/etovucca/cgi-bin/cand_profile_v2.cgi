#!/usr/bin/env python3
import os, sqlite3, cgi
from typing import Dict, List, Tuple

# Define the path to the introduction file
PATH_TO_UPLOADS = "./candidate_info"
PATH_TO_SQLITE = "./sqlite3"
PATH_TO_DB = "rtbb.sqlite3"
INTRO_FILE = os.path.join(PATH_TO_UPLOADS, "introduction.txt")

ELECTION_QUERY = "SELECT id, name FROM Office"
CANDIDATE_QUERY = "SELECT name, bio FROM Candidate WHERE office=?"


def get_all_candidates() -> Dict[str, List[Tuple[str, str]]]:
    connection = sqlite3.connect(PATH_TO_DB)
    cursor = connection.cursor()
    cursor.execute(ELECTION_QUERY)
    office_id_and_names = cursor.fetchall()
    candidate_dict = {}
    for office_id, office_name in office_id_and_names:
        cursor.execute(CANDIDATE_QUERY, (office_id,))
        candidate_dict[office_name] = cursor.fetchall()
    connection.close()
    return candidate_dict


def generate_html() -> str:
    html_content = """
    <head>
        <style>
            .election { margin-top: 20px; }
            .candidate { padding-left: 20px; margin-bottom: 10px; }
            .bio { font-style: italic; color: #555; }
        </style>
    </head>
    <body>
    """
    candidate_dict = get_all_candidates()
    idx = 0
    for election_name, candidate_list in candidate_dict.items():
        if idx != 0:
            html_content += "</div>"
        html_content += f'<div class="election"><h2>{election_name}</h2>'
        for candidate_name, candidate_bio in candidate_list:
            html_content += f"""
                <div class="candidate">
                    <strong>{candidate_name}</strong>
                    <p class="bio">{candidate_bio}</p>
                </div>
            """
        idx += 1
    html_content += "</div>"
    return html_content


print("Content-Type: text/html")
print()
print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
print('<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>')
print('<h1 id="candidate">Candidate Profile</h1>')
print(generate_html())
print('<br><a href="home.cgi">Return to Homepage</a>')
print("</body></html>")
