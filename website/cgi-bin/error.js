#!/usr/local/bin/node

// Set messages
const messages = {
	300: "Multiple Choices",
	301: "Moved Permanently",
	302: "Found",
	303: "See Other",
	307: "Temporary Redirect",
	308: "Permanent Redirect",
	400: "Bad Request",
	401: "Unauthorized",
	402: "Payment Required",
	403: "Forbidden",
	404: "Not Found",
	405: "Method Not Allowed",
	408: "Request Timeout",
	409: "Conflict",
	410: "Gone",
	411: "Length Required",
	412: "Precondition Failed",
	413: "Payload Too Large",
	444: "Connection Closed Without Response",
	500: "Internal Server Error",
	501: "Not Implemented"
  };

// Get status code from environment variable
const code = process.env.STATUS_CODE || "403";
const message = messages[code] || "Unknown Error";
const image = `/images/${code}.jpg`;

// Generate HTML response
const body = `<!DOCTYPE html>
  <html>
  <head>
	<title>Error ${code} - Team Mustache</title>
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
		<h2>Error ${code} - ${message}</h2>
		<img src="${image}" alt="Error ${code}" width="480" style="height:auto;">
	  </section>
	</main>

	<footer>
	  <p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
  </body>
  </html>`;

//HTTP headers
const headers = `HTTP/1.1 ${code} ${message}\n' +
  'Content-Type: text/html; charset=utf-8\n' +
  'Content-Length: ${body.length}\n`;

// Print the headers and body
console.log(headers);
console.log(body);
