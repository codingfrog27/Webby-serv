#!/usr/bin/env python3

import cgi, os, cgitb

# Enable error reporting in browser
cgitb.enable(display=1)

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
            status = 200
            message = f"File '{filename}' uploaded successfully! 🎉"
        except Exception as e:
            status = 500
            message = f"An error occurred: {e}"
    else:
        status = 400
        message = "File upload failed. 😢"
else:
    status = 200
    message = "No file uploaded. 😬"

# Get the HTTP status code and message

# HTML template function
body = f"""<!DOCTYPE html>
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
            <form action="/cgi-bin/list_uploads.py" method="post" enctype="multipart/form-data">
                <input type="file" name="file" required>
                <input type="submit" value="Upload">
            </form>
        </section>

        <section>
            <h2>hello gamerzz</h2>
        </section>
    </main>

    <footer>
        <p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
    </footer>
</body>
</html>"""

# HTTP headers
headers = f"""HTTP/1.1 {status}\r
Content-Length: {len(body)}\r
Content-Type: text/html; charset=utf-8\r
Connection: keep-alive\r\n\r"""

# Print the headers and HTML body
print(headers)
print(body)
print("\0")
