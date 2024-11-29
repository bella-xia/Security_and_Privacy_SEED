#!/usr/bin/env python3

import cgi, string, random, hashlib, sqlite3
from http.cookies import SimpleCookie

redirectURL = "./candidate_v2.cgi"
PATH_TO_SQLITE = "./sqlite3"
PATH_TO_DB = "rtbb.sqlite3"


def check_candidate_from_sql(name: str, office_num: int) -> int:
    # Please don't ever actually do this.
    conn = sqlite3.connect(PATH_TO_DB)
    cursor = conn.cursor()
    sql = "SELECT * FROM Candidate WHERE name=? AND office=?"
    cursor.execute(sql, (name, office_num))
    results = cursor.fetchall()
    row_count = len(results)
    conn.close()
    return row_count


def render_login(failure=False, logout=False, exception=""):
    print("Content-Type: text/html")
    if logout:
        print("Set-Cookie: user=LOGGEDOUT; expires=Thu, 01 Jan 1970 00:00:00 GMT")
    print()
    print(
        '<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">'
    )
    print(
        '<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>'
    )
    print('<h1 id="candidate-interface-login">	Candidate Interface Login</h1><br>')
    if failure:
        print("<b>Login Failed.</b><br>")
        print(f"<b> {exception} </b><br>")
    print('<form method="post">')
    print('<label for="name">Name:</label>')
    print('<input id="cand_name" name="cand_name"><br>')
    print('<label for="elec">Election:</label>')
    print('<input id="elec_num" name="elec_num"><br>')
    print('<label for="passwd">Password:</label>')
    print('<input type="password" id="passwd" name="passwd"><br>')
    print('<input type="submit" value="Login">')
    print("</form>")
    print("<a href='./home.cgi'>Return to Homepage</a>")


form = cgi.FieldStorage()

try:
    if "passwd" in form and "cand_name" in form and "elec_num" in form:
        check_instance = check_candidate_from_sql(
            form.getvalue("cand_name"), int(form.getvalue("elec_num"))
        )
        if check_instance != 1:
            raise Exception(
                f"failed to find the candidate name {form.getvalue('cand_name')} from the given election"
            )
        random.seed(len(form.getvalue("cand_name")))
        letters = string.ascii_letters + string.digits
        ounce = "".join(random.choice(letters) for _ in range(50))
        result_str = ounce + form.getvalue("cand_name") + form.getvalue("elec_num")
        hash_pswd = hashlib.md5(result_str.encode()).hexdigest()
        if hash_pswd == form.getvalue("passwd"):
            # CGI Redirect: https://stackoverflow.com/a/6123179
            print("Content-Type: text/html")
            url_plus_query = f"{redirectURL}?candidate={form.getvalue('cand_name')}&election={form.getvalue('elec_num')}"
            print("Location: %s" % url_plus_query)
            C = SimpleCookie()
            C["user"] = hash_pswd
            print(C)
            print("")
            print("<html>")
            print("<head>")
            print(
                '<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">'
            )
            print('<meta http-equiv="refresh" content="0;url=%s" />' % url_plus_query)
            print("<title>You are going to be redirected</title>")
            print("</head>")
            print("<body>")
            print(
                'Redirecting... <a href="%s">Click here if you are not redirected</a>'
                % url_plus_query
            )
            print("</body>")
            print("</html>")
        else:
            raise ValueError("incorrect hash")
    elif "logout" in form:
        render_login(logout=True)
    else:
        render_login()
except Exception as e:
    render_login(failure=True, exception=str(e))
