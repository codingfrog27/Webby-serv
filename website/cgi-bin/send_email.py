#!/usr/bin/env python3

import cgi, os, smtplib
from email.message import EmailMessage

# Parse form data
form = cgi.FieldStorage()
name = form.getvalue("name").strip()
email = form.getvalue("email").strip()
subject = form.getvalue("subject").strip()
message = form.getvalue("message").strip()

if not name or not email or not subject or not message:
	status = 400
	body = f"""
	<!DOCTYPE html>
	<title>Error</title>
	<p>Missing required fields! ❌</p>
	<p><a href="/contact.html">Back to Contact Form</a></p>
	"""

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

		status = 200
		body = f"""
		<!DOCTYPE html>
		<title>Message Sent</title>
		<p>Message sent successfully! 📬</p>
		<p><a href="/contact.html">Back to Contact Form</a></p>
		"""
	except Exception as e:
		status = 500
		body = f"""
		<!DOCTYPE html>
		<title>Error</title>
		<p>Message not sent. Error: {e}</p>
		<p><a href="/contact.html">Back to Contact Form</a></p>
		"""

# HTTP headers
headers = f"""HTTP/1.1 {status}\r
Content-Length: {len(body)}\r
Content-Type: text/html; charset=utf-8\r
Connection: keep-alive\r\n\r"""

# Print the headers and HTML body
print(headers)
print(body)