#!/usr/bin/env python3

import cgi, os, cgitb

cgitb.enable(display=1)

# Get connection type from environment variable
connection = os.environ.get("CONNECTION", "keep-alive")

# Set the upload directory
UPLOAD_DIR = "website/uploads"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR, exist_ok=True)

files = os.listdir(UPLOAD_DIR) if os.path.exists(UPLOAD_DIR) else []
files_list = ""

if files:
	for file in files:
		safe_filename = file.replace('"', '&quot;')
		file_url = f"/uploads/{safe_filename}"
		files_list += f'<li><a href="{file_url}" target="_blank">{safe_filename}</a></li>\n'
else:
	files_list = "<li>No files uploaded yet.</li>"

# Start HTML output
body = f"""<!DOCTYPE html>
<head>
	<title>Uploaded Files - Team Mustache</title>
	<link rel="stylesheet" type="text/css" href="/css/styles.css">
	<link rel="icon" type="image/png" sizes="32x32" href="/images/favicon-32x32.png">
</head>
<style>
	.button {{
		border: none;
		color: white;
		padding: 10px 24px;
		text-align: center;
		text-decoration: none;
		display: inline-block;
		font-size: 16px;
		margin: 4px 2px;
		cursor: pointer;
		background-color: #88c29f;
	}}
	.button:hover {{
		background-color: #31c16a;
	}}
</style>
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
			<h2>Uploaded Files</h2>
			<ul>
				{files_list}
			</ul>
			<button class="button" onclick="window.location.href='/upload.html'">Upload a File 📤 </button>
			<button class="button" onclick="window.location.href='/cgi-bin/list_files_delete.py'">Delete a File 🗑️</button>
		</section>
	</main>

	<footer>
		<p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
</body>
</html>"""

# HTTP headers
headers = f"""HTTP/1.1 200 OK\r
Connection: {connection}\r
Content-Type: text/html; charset=utf-8\r
Content-Length: {len(body.encode('utf-8'))}\r
"""

# Print the headers and HTML body
print(headers)
print()
print(body)
