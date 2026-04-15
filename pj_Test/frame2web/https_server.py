import http.server
import ssl

PORT = 8300

handler = http.server.SimpleHTTPRequestHandler
httpd = http.server.HTTPServer(("0.0.0.0", PORT), handler)

httpd.socket = ssl.wrap_socket(
    httpd.socket,
    certfile="cert.pem",
    keyfile="key.pem",
    server_side=True
)

print(f"Serving HTTPS on 0.0.0.0:{PORT}")
httpd.serve_forever()