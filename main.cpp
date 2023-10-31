#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <crow.h>
#include <opencv2/opencv.hpp>

using namespace cv;

std::vector<crow::websocket::connection*> connections;

void ws_on_open(crow::websocket::connection& conn) {
    connections.push_back(&conn);
    std::cout << "New connection" << std::endl;
}

void ws_on_close(crow::websocket::connection& conn, const std::string& reason) {
    connections.erase(std::remove(connections.begin(), connections.end(), &conn), connections.end());
    std::cout << "Removed connection" << std::endl;
}

void ws_on_message(crow::websocket::connection& conn, const std::string& data, bool is_binary) {
    std::vector<char> data_vec(data.begin(), data.end());
    Mat img = imdecode(data_vec, IMREAD_GRAYSCALE);
}

int main() {
    crow::SimpleApp app;

    CROW_WEBSOCKET_ROUTE(app, "/")
        .onopen(ws_on_open)
        .onclose(ws_on_close)
        .onmessage(ws_on_message);

    app.port(8000).multithreaded().run();
}
