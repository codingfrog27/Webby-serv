#!/usr/bin/env python3
import cgi

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Connection: keep-alive\r\n")

form = cgi.FieldStorage()
name = form.getvalue("name")

print("<html>")
print("<head>")
print("<title>Success Page</title>")
print("</head>")
print("<body>")

print("<p><strong><span style='font-size: 20px;'>{}</span></strong> Congratulations !!</p>".format(name))

print("<p>You have successfully sent data from an HTML form to a CGI Python script.</p>")

print("</body>")
print("</html>")
