#!/usr/bin/env python3

import cgi
import cgitb
cgitb.enable()

form = cgi.FieldStorage()
param = form.getvalue("param")
num = int(form.getvalue("num"))

print("Content-Type: text/html")
print()
for i in range(num):
    print(f"<p>{param}</p>")
