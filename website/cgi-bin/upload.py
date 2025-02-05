#!/usr/bin/env python3

import cgi
import os

# Set the upload directory
UPLOAD_DIR = "website/uploads"  # Change this to your server's upload directory

# Ensure the upload directory exists
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR, exist_ok=True)

# Print necessary HTTP headers
print("HTTP/1.1 200 OK")
print("Content-Type: text/html\n")
print("Connection: keep-alive\r\n")

# Parse form data
form = cgi.FieldStorage()

# HTML template function
def print_html(message=""):
	print(f"""
	<!DOCTYPE html>
	<html>
	<head>
		<title>File Upload</title>
	</head>
	<body>
		<h2>Upload a File</h2>
		{f"<p style='color:green;'>{message}</p>" if message else ""}
		<form action="/cgi-bin/upload.py" method="post" enctype="multipart/form-data">
			<input type="file" name="file" required>
			<input type="submit" value="Upload">
		</form>
	</body>
	</html>
	""")

# Check if the user submitted a file
if "file" in form:
	file_item = form["file"]

	if file_item.filename:
		# Get the filename and sanitize it
		filename = os.path.basename(file_item.filename)
		filepath = os.path.join(UPLOAD_DIR, filename)

		# Save the file
		with open(filepath, "wb") as f:
			f.write(file_item.file.read())

		print_html(f"File '{filename}' uploaded successfully!")
	else:
		print_html("File upload failed.")
else:
	print_html()  # Display form if no file is uploaded
	