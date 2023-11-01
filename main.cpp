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
    Mat image = imdecode(data_vec, IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error: Could not open or read the image." << std::endl;
    }

    // Convert the image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Apply Gaussian blur to reduce noise
    GaussianBlur(gray, gray, Size(5, 5), 0);

    // Apply Canny edge detection
    Mat edges;
    Canny(gray, edges, 50, 150);

    // Define a region of interest (ROI) mask to focus on the road area
    Mat mask = Mat::zeros(edges.size(), CV_8UC1);
    Point pts[4] = {Point(100, image.rows), Point(400, 300), Point(600, 300), Point(image.cols - 100, image.rows)};
    fillConvexPoly(mask, pts, 4, Scalar(255, 255, 255));
    bitwise_and(edges, mask, edges);

    // Apply Hough Line Transform to detect lines
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, 1, CV_PI / 180, 100);

    // Draw the detected lines on the original image
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        line(image, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    // Show the result
    imshow("Lane Detection", image);
    waitKey(0);

    imwrite("test.png", image);
}

int main() {
    crow::SimpleApp app;

    CROW_WEBSOCKET_ROUTE(app, "/")
        .onopen(ws_on_open)
        .onclose(ws_on_close)
        .onmessage(ws_on_message);

    app.port(8000).multithreaded().run();
}
