# Webserv Project - 42 School

## Overview

The **Webserv Project** at 42 School is an implementation of a lightweight HTTP server built using C++. This project aimed us understand the key concepts behind web servers such as configuration file parsing, handling HTTP requests and responses, socket management, and CGI (Common Gateway Interface) scripting. The project adheres to HTTP/1.1 standards and supports multiple features such as handling different request methods (GET, POST, DELETE), managing multiple clients, and running CGI scripts.

## Table of Contents
1. [Project Structure](#project-structure)
2. [Features](#features)
   - [Config File Parsing](#config-file-parsing)
   - [Request Parsing](#request-parsing)
   - [Socket Management](#socket-management)
   - [Response Construction](#response-construction)
   - [CGI Execution](#cgi-execution)
3. [Usage](#usage)
4. [Examples](#examples)
5. [Contributing](#contributing)
6. [License](#license)

## Project Structure

```plaintext
Webserv/
├── src/
│   ├── main.cpp            # Entry point of the server
│   ├── ConfigParser.cpp    # Handles parsing of the configuration file
│   ├── RequestParser.cpp   # Parses incoming HTTP requests
│   ├── Response.cpp        # Constructs HTTP responses
│   ├── SocketManager.cpp   # Manages socket communication
│   ├── CGIHandler.cpp      # Manages CGI script execution
│   ├── ConfigParser.hpp
│   ├── RequestParser.hpp
│   ├── Response.hpp
│   ├── SocketManager.hpp
│   ├── CGIHandler.hpp
│   └── ...
├── config/
│   ├── default.conf        # Example valid configuration file
│   ├── invalid.conf        # Example invalid configuration file
│   ├── mime.typ            # Extensions
├── www/                    # CGI script directory
│   ├── content             # Testing resources
└── .gitignore
└── README.md               # Project documentation
└── Makefile                # Makefile
```

## Features

### Config File Parsing

The server's behavior is defined in a configuration file, typically located in the config/ directory. This file defines settings such as:

Ports and IP addresses to listen on
Server names and hostnames
Root directories for serving static files
Error pages
CGI script handling
The configuration parser (`ConfigParser.cpp`) reads the configuration file, parses its contents, and stores the settings in a data structure. Here's an example of a configuration file (`default.conf`):

```
server {
    listen 80;
    server_name localhost;
    root /var/www/html;
    error_page 404 /404.html;

    location /cgi-bin/ {
        cgi_pass /path/to/cgi-bin/;
    }
}
```

### Request Parsing

When a client sends an HTTP request, the `RequestParser.cpp` module parses the request based on the HTTP/1.1 standard. It handles:

- Request line: Method, URI, and HTTP version
- Headers: Host, Content-Type, Connection, etc.
- Body: (For POST and other methods that may contain a body)

Supported methods include:

- GET: Retrieve resources.
- POST: Submit data to the server.
- DELETE: Delete specified resources.

The parsed data is stored in a structured format and passed to the appropriate modules for processing.

## Socket Management

The `SocketManager.cpp` is responsible for managing client connections and handling the underlying TCP sockets:

- Opening and binding sockets to specified IP addresses and ports.
- Listening for incoming connections and accepting clients.
- Reading and writing data between the server and the clients.
- Handling multiple clients using non-blocking I/O and poll() for multiplexing.
- The socket manager uses a reactor pattern to efficiently handle multiple client connections and ensures that the server can serve requests concurrently.

## Response Construction
The `Response.cpp` module is responsible for constructing HTTP responses based on the parsed request and server configuration. It handles:

- Static file serving: Reads files from the filesystem and returns them as HTTP responses.
- Error handling: Sends appropriate error pages (404, 500, etc.) if a resource is not found or an internal error occurs.
- Content-Type resolution: Determines the correct MIME type based on the requested file extension.

Example of response construction:
```
http
Copy code
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 138

<html>
    <head><title>Welcome</title></head>
    <body><h1>Hello, World!</h1></body>
</html>
```

## CGI Execution

The CGI module (`Response.cpp`) is responsible for executing external scripts when a request is routed to a CGI-enabled location (e.g., `/cgi-bin/`). This allows dynamic content generation, typically through Python, PHP, or other scripting languages.

1. Environment Variables: The server sets up necessary environment variables (e.g., `QUERY_STRING`, `CONTENT_LENGTH`) to pass request data to the CGI script.
2. Process Creation: The server spawns a new process using `fork()` and `execve()` to execute the CGI script.
3. Output Handling: The script’s output (typically HTML or JSON) is captured and sent back to the client.

For example, a Python CGI script might look like this:

```
#!/usr/bin/env python3
import os

print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>CGI Script Output</h1>")
print(f"<p>QUERY_STRING: {os.environ.get('QUERY_STRING')}</p>")
print("</body></html>")
```

## Error Handling

The server gracefully handles errors and sends appropriate HTTP status codes to the client. The `error_page` directive in the configuration file allows custom error pages for codes such as 404 (Not Found) or 500 (Internal Server Error).

# Usage

## Compilation
To compile the project, navigate to the root directory of the project and run the following command:

```make```

This will create the executable webserv.

## Running the Server

Once compiled, you can run the server with a specified configuration file:

```./webserv```

The server will start listening on the ports defined in the configuration file. Open your browser and navigate to the IP address and port where the server is running to interact with it.

## Testing CGI

Place any CGI scripts in the specified CGI directory (as per your config file). Then, send a request to the server for the script:

```
curl http://localhost/cgi-bin/sample_script.py?name=42
```

## Examples
## Example Configuration File

```
server {
    listen 8080;
    server_name my_server;
    root /var/www/html;
    error_page 404 /404.html;

    location / {
        index index.html;
    }

    location /cgi-bin/ {
        cgi_pass /path/to/cgi-bin/;
    }
}
```

## Contributing

1. Fork the repository.
2. Create your feature branch: git checkout -b feature/my-feature.
3. Commit your changes: git commit -m 'Add my feature'.
4. Push to the branch: git push origin feature/my-feature.
5. Open a pull request.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
