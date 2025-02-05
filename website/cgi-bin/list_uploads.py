#!/usr/bin/env python3

import cgi
import os

# Set the upload directory
UPLOAD_DIR = "website/uploads"  # Change this path based on your server setup

# Ensure the upload directory exists
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR, exist_ok=True)

# Print HTTP headers
print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Connection: keep-alive\r\n")

# Parse form data
form = cgi.FieldStorage()

# HTML template function
def print_html(message=""):
    print(f"""<!DOCTYPE html>
<html>
<head>
    <title>File Upload - Team Mustache</title>
    <link rel="stylesheet" type="text/css" href="/css/styles.css">
    <link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
</head>
<body>
    <header>
        <h1>hey there ;)</h1>
        <nav>
            <ul>
                <li><a href="index.html">Home</a></li>
                <li><a href="about.html">About</a></li>
                <li><a href="services.html">Services</a></li>
                <li><a href="contact.html">Contact</a></li>
            </ul>
        </nav>
    </header>

    <main>
        <section>
            <h2>Upload a File</h2>
            <p>here lies Team Mustache's epic file upload zone</p>
            {f"<p style='color:green;'>{message}</p>" if message else ""}
            <form action="/cgi-bin/upload.py" method="post" enctype="multipart/form-data">
                <input type="file" name="file" required>
                <input type="submit" value="Upload">
            </form>
        </section>

        <section>
            <h2>hello gamerzz</h2>
            <img src="/images/dancing_froggy.gif" alt="gif no loady :(" width="480" height="480">
        </section>
    </main>

    <footer>
        <p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
    </footer>
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

        print_html(f"File '{filename}' uploaded successfully! 🎉")
    else:
        print_html("<p style='color:red;'>File upload failed. 😢</p>")
else:
    print_html("<p style='color:red;'>No file uploaded. 😬</p>")
    