#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include <nlohmann/json.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;
using json = nlohmann::json;

int main() {
    client c;

    try {
        c.init_asio();

        c.clear_access_channels(websocketpp::log::alevel::all);
        c.clear_error_channels(websocketpp::log::elevel::all);
        websocketpp::connection_hdl hdl;

        // random generator
        std::default_random_engine gen;
        std::uniform_real_distribution<float> dist(0.0, 0.6);

        // 🔥 on_open
        c.set_open_handler([&](websocketpp::connection_hdl handle) {
            std::cout << "Connected to server\n";
            hdl = handle;

            // start sending loop
            std::thread([&]() {
                while (true) {
                    json msg;

                    msg["boxes"] = {
                        {
                            {"x", dist(gen)},
                            {"y", dist(gen)},
                            {"w", 0.2},
                            {"h", 0.2},
                            {"label", "CPP"}
                        }
                    };

                    std::string payload = msg.dump();

                    try {
                        c.send(hdl, payload, websocketpp::frame::opcode::text);
                        // std::cout << "SEND: " << payload << std::endl;
                    } catch (const std::exception& e) {
                        std::cout << "Send error: " << e.what() << std::endl;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }).detach();
        });

        // error handler
        c.set_fail_handler([](websocketpp::connection_hdl) {
            std::cout << "Connection failed\n";
        });

        c.set_close_handler([](websocketpp::connection_hdl) {
            std::cout << "Connection closed\n";
        });

        websocketpp::lib::error_code ec;
        auto con = c.get_connection("ws://localhost:9000", ec);

        if (ec) {
            std::cout << "Connect error: " << ec.message() << std::endl;
            return 1;
        }

        c.connect(con);

        c.run(); // block here

    } catch (std::exception& e) {
        // std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}