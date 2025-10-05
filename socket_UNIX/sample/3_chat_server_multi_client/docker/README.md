docker build -t chat-server -f Dockerfile.server .

docker build -t chat-client -f Dockerfile.client .

docker network create chatnet

docker network create chatnet

docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' server
172.18.0.2

docker network inspect chatnet

docker run -it --rm --network chatnet --name client1 chat-client ./client server 8080

docker run -it --rm --network chatnet --name client2 chat-client ./client server 8080

🔹 Giả lập “khác mạng”

Nếu bạn muốn giả lập như khác mạng thực sự (client không cùng mạng Docker):

Run server với port mapping ra host:

docker run -it --rm -p 8080:8080 chat-server


Client container connect qua host.docker.internal (nếu client ở container trên cùng máy) hoặc IP public/host (nếu ở máy khác).

Ví dụ:

docker run -it --rm chat-client host.docker.internal 8080
