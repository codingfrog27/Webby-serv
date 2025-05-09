#!/usr/bin/env python3

import os, cgi

UPLOAD_DIR = "website/uploads"

# Parse form data
form = cgi.FieldStorage()
selected_file = form.getvalue("file")

# Get connection type from environment variable
connection = os.environ.get("CONNECTION", "keep-alive")

# Delete the selected file
if selected_file:
	safe_filename = os.path.basename(selected_file)
	file_path = os.path.join(UPLOAD_DIR, safe_filename)

	if os.path.exists(file_path):
		try:
			os.remove(file_path)
			status = "200 OK"
			message = f"File '{safe_filename}' deleted successfully! 🗑️"
		except Exception as e:
			status = "500 Internal Server Error"
			message = f"An error occurred: {e}"
	else:
		status = "404 Not Found"
		message = f"File '{safe_filename}' does not exist! ❌"
else:
	status = "400 Bad Request"
	message = "No file selected for deletion! ❌"

# Generate HTML response
body = f"""
<!DOCTYPE html>
<head>
	<title>Delete File - Team Mustache</title>
	<link rel="stylesheet" type="text/css" href="/css/styles.css">
	<link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
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
			<h2>Delete Status</h2>
			<p style="color:red;">{message}</p>
			<p><a href="/cgi-bin/list_files.py">Back to Uploads</a></p>
		</section>

	</main>

	<footer>
		<p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
</body>
</html>
"""

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