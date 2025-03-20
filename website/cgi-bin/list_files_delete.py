#!/usr/bin/env python3

import cgi, os, cgitb

cgitb.enable(display=1)

# Set the upload directory
UPLOAD_DIR = "website/uploads"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR, exist_ok=True)

files = os.listdir(UPLOAD_DIR) if os.path.exists(UPLOAD_DIR) else []
files_delete = ""

if files:
	for file in files:
		safe_filename = file.replace('"', '&quot;')
		file_url = f"/uploads/{safe_filename}"
		files_delete += f'<option value="{file}">{safe_filename}</option>\n'
else:
	files_delete = '<option disabled>No files available</option>'

# Start HTML output
body = f"""<!DOCTYPE html>
<head> <!-- head is for meta info (running theme ig xd) -->
	<title>Delete file - Team Mustache</title>
	<link rel="stylesheet" type="text/css" href="/css/styles.css">
	<link rel="icon" type="image/png" sizes="32x32" href="/images/favicon-32x32.png">
</head>
<body> <!-- all the actual page content -->
	<header> <!-- usually used as introduction of page with links or a logo -->
		<h1>hey there ;)</h1> <!-- h1 == bigger text -->
		<nav> <!-- nav == navigation links -->
			<ul> <!-- unorder list -->
				<li><a href="index.html">Home</a></li> <!-- li == list item, A is for links -->
				<li><a href="about.html">About Us</a></li>
				<li><a href="cgi-bin/list_files.py">Uploads</a></li>
				<li><a href="contact.html">Contact</a></li>
			</ul>
		</nav>
	</header>

	<main> <!-- main page content -->
		<section>
			<h2>Delete a file</h2>
			<form action="/cgi-bin/delete.py" method="post">
				<select id="file" name="file" size="1">
					{files_delete}
				</select>
				<input type="submit" value="Delete">
			</form>
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
Connection: keep-alive\r
Content-Type: text/html; charset=utf-8\r
Content-Length: {len(body.encode('utf-8'))}\r
"""

# Print the headers and HTML body
print(headers)
print()
print(body)
