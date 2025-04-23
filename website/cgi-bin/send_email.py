#!/usr/bin/env python3

import cgi, os, smtplib
from email.message import EmailMessage

# Get connection type from environment variable
connection = os.environ.get("CONNECTION", "keep-alive")

# Parse form data
form = cgi.FieldStorage()
name = form.getvalue("name").strip()
email = form.getvalue("email").strip()
subject = form.getvalue("subject").strip()
message = form.getvalue("message").strip()

if not name or not email or not subject or not message:
	status = "400 Bad Request"
	message = "Missing required fields! 😢"

else:
	SMTP_SERVER = "smtp.mail.me.com"
	SMTP_PORT = 587
	EMAIL_ADDRESS = "m.stegeman@me.com"
	EMAIL_PASSWORD = "ilnt-lthl-pdrc-rxee"

	# Generate email messages
	msg = EmailMessage()
	msg["From"] = EMAIL_ADDRESS
	msg["To"] = EMAIL_ADDRESS
	msg["Subject"] = "Contact Form Submission: " + subject
	msg.set_content(f"""
	Name: {name}
	Email: {email}
	Subject: {subject}

	Message: {message}
	""")

	# Send email
	try:
		server = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
		server.starttls()
		server.login(EMAIL_ADDRESS, EMAIL_PASSWORD)
		server.send_message(msg)
		server.quit()

		status = "200 OK"
		message = "Message sent successfully! 🎉"

	except Exception as e:
		status = "500 Internal Server Error"
		message = f"An error occurred: {e}"

body = f"""<!DOCTYPE html>
<head>
	<title>Contact Success - Team Mustache</title>
	<link rel="stylesheet" type="text/css" href="/css/styles.css">
	<link rel="icon" type="image/png" sizes="32x32" href="/images/favicon-32x32.png">
</head>
<body>
	<header>
		<h1>hey there ;)</h1>
		<nav>
			<ul>
				<li><a href="/index.html">Home</a></li>
				<li><a href="/about.html">About Us</a></li>
				<li><a href="/cgi-bin/list_files.py">Uploads</a></li>
				<li><a href="/contact.html">Contact</a></li>
			</ul>
		</nav>
	</header>

	<main>
		<section>
			<h2>Upload Status</h2>
			<p style='color:green;'>{message}</p>
			<p><a href="/contact.html">Back to Contact Form</a></p>
		</section>

		<section>
			<img src="/images/dancing_froggy.gif" alt="gif no loady :(" width="420" height="auto">
		</section>
	</main>

	<footer>
		<p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
</body>
</html>"""

# HTTP headers
headers = f"""HTTP/1.1 {status}\r
Connection: {connection}\r
Content-Type: text/html; charset=utf-8\r
Content-Length: {len(body.encode('utf-8'))}\r
"""

# Print the headers and HTML body
print(headers)
print()
print(body)