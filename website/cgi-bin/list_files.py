#!/usr/bin/env python3

import cgi, os, cgitb

cgitb.enable(display=1)

# Set the upload directory
UPLOAD_DIR = "website/uploads"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR, exist_ok=True)

files = os.listdir(UPLOAD_DIR) if os.path.exists(UPLOAD_DIR) else []
files_html = ""

if files:
	for file in files:
		safe_filename = file.replace('"', '&quot;')
		file_url = f"/uploads/{safe_filename}"
		files_html += f'<li><a href="{file_url}" target="_blank">{safe_filename}</a></li>\n'
else:
	files_html = "<li>No files uploaded yet.</li>"

# Start HTML output
body = f"""
<!DOCTYPE html>
<head>
	<title>Uploaded Files - Team Mustache</title>
	<link rel="stylesheet" type="text/css" href="/css/styles.css">
	<link rel="icon" type="image/png" sizes="32x32" href="/images/favicon-32x32.png">
</head>
<body>
	<header>
		<h1>hey there ;)</h1>
		<nav>
			<ul>
				<li><a href="index.html">Home</a></li>
				<li><a href="about.html">About</a></li>
				<li><a href="cgi-bin/list_files.py">Uploads</a></li>
				<li><a href="services.html">Services</a></li>
				<li><a href="contact.html">Contact</a></li>
			</ul>
		</nav>
	</header>

	<main>
		<section>
			<h2>Upload a File</h2>
			<form action="/cgi-bin/upload.py" method="post" enctype="multipart/form-data">
				<input type="file" name="file" required>
				<input type="submit" value="Upload">
			</form>
		</section>

		<section>
			<h2>Uploaded Files</h2>
			<ul>
				{files_html}
			</ul>
		</section>

	</main>

	<footer>
		<p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
</body>
</html>
"""

# HTTP headers
headers = f"""HTTP/1.1 200\r
Content-Length: {len(body)}\r
Content-Type: text/html; charset=utf-8\r
Connection: keep-alive\r\n\r"""

# Print the headers and HTML body
print(headers)
print(body)
