sudo apt install libwebsocketpp-dev libasio-dev

# g++ main.cpp -o cpp_ws -lboost_system -lpthread

openssl req -x509 -newkey rsa:2048 -nodes -keyout key.pem -out cert.pem -days 365