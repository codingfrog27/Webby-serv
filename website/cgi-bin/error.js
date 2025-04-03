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
<head>
	<title>Error ${code} - Team Mustache</title>
	<link rel="icon" type="image/png" sizes="32x32" href="/images/favicon-32x32.png">
</head>
<body>
	<header>
	<h1>whoopsie ;)</h1>
	</header>

	<main>
	<section>
		<h2>Error ${code}</h2>
		<img src="${image}" alt="Error ${code} - ${message}" width="420" style="height:auto;">
		<p>Sorry, but something went wrong. Please try again later.</p>
		<p>Return to the <a href="/index.html">homepage</a></p>
	</section>
	</main>

	<footer>
	<p>&copy; 2024 epic awesomesauce inc. All rights reserved.</p>
	</footer>
</body>
</html>
`;

//HTTP headers
const headers = `HTTP/1.1 ${code} ${message}
Connection: close
Content-Type: text/html; charset=utf-8
Content-Length: ${body.length}

`;

// Print the headers and body
console.log(headers + body);
// console.log(body);