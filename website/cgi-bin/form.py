#!/usr/bin/env python3
import cgi

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Connection: keep-alive\r\n")

form = cgi.FieldStorage()
name = form.getvalue("name")

print("<html><body>")
print("<h1>Hello, {}!</h1>".format(name if name else "Stranger"))
print('<form method="POST" action="/cgi-bin/form.py">')
print('Name: <input type="text" name="name">')
print('<input type="submit" value="Submit">')
print("</form>")
print("</body></html>")
