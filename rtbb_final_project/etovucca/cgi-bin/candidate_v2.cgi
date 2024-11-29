#!/usr/bin/env python3

import cgi, sqlite3, os
from urllib.parse import parse_qs
from typing import Dict

PATH_TO_MACHINE = "./etovucca"
PATH_TO_SQLITE = "./sqlite3"
PATH_TO_DB = "rtbb.sqlite3"


print("Content-Type: text/html")
print("Cache-Control: no-store, must-revalidate")
print()

query_string = os.environ.get("QUERY_STRING", "")
params = parse_qs(query_string)
print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
print('<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>')
print('<h1 id="candidate">Candidate Interface</h1>')

GET_BIO = "SELECT bio FROM Candidate WHERE name=? AND office=?"
UPDATE_BIO = "UPDATE Candidate SET bio=? WHERE name=? AND office=?"


def get_introduction(params: Dict[str, any]) -> str:
    connection = sqlite3.connect(PATH_TO_DB)
    cursor = connection.cursor()
    cursor.execute(GET_BIO, (params["candidate"][0], int(params["election"][0])))
    result = cursor.fetchone()
    connection.close()
    return result


def update_introduction(params: Dict[str, any], new_bio: str) -> str:
    connection = sqlite3.connect(PATH_TO_DB)
    cursor = connection.cursor()
    cursor.execute(
        UPDATE_BIO, (new_bio, params["candidate"][0], int(params["election"][0]))
    )
    connection.commit()
    if cursor.rowcount > 0:
        return_statement = "Bio updated successfully."
    else:
        return_statement = "Candidate not found or no changes made."
    connection.close()
    return return_statement


print("<p>Introduction</p>")

form = cgi.FieldStorage()
if "introduction" in form:
    introduction = form.getvalue("introduction")
    message = update_introduction(params, introduction)
    print("<p>{}</p>".format(message))

print(
    f"""
<form action="./candidate_v2.cgi?candidate={params['candidate'][0]}&election={params['election'][0]}" method="post">
    <p>
        <textarea name="introduction" rows="10" cols="30">{get_introduction(params)}</textarea>
    </p>
    <p><input type="submit" value="Save"></p>
</form>
"""
)
print('<br><a href="home.cgi">Return to Homepage</a>')
print("</body></html>")
