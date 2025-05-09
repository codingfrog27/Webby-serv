#!/usr/bin/env python3

import cgi, os, cgitb

cgitb.enable(display=1)

# Get connection type from environment variable
connection = os.environ.get("CONNECTION", "keep-alive")

# Set the upload directory
UPLOAD_DIR = "website/uploads"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR, exist_ok=True)

# Parse form data
form = cgi.FieldStorage()

# Get the filename and sanitize it
def generateFilename(filePath) -> str:
	fileBaseName = os.path.basename(filePath)
	fileName = os.path.splitext(fileBaseName)[0]
	fileExtension = os.path.splitext(fileBaseName)[1]
	index = 0
	while os.path.exists(filePath):
		index += 1
		filePath = f"{UPLOAD_DIR}/{fileName}({index}){fileExtension}"
	return filePath

# Check if the user submitted a file
if "file" in form:
	file_item = form["file"]

	if file_item.filename:
		# Get the filename and sanitize it
		filename = os.path.basename(file_item.filename)
		filepath = os.path.join(UPLOAD_DIR, filename)
		if os.path.exists(filepath):
			filepath = generateFilename(filepath)

		# Save the file
		try:
			with open(filepath, "wb") as f:
				f.write(file_item.file.read())
			status = "200 OK"
			message = f"File '{filename}' uploaded successfully! 🎉"
		except Exception as e:
			status = "500 Internal Server Error"
			message = f"An error occurred: {e}"
	else:
		status = "400 Bad Request"
		message = "File upload failed. 😢"
else:
	status = "200 OK"
	message = "No file uploaded. 😬"

# HTML template function
body = f"""<!DOCTYPE html>
<head>
	<title>Upload Success - Team Mustache</title>
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
			<p><a href="/cgi-bin/list_files.py">Back to Uploads</a></p>
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
