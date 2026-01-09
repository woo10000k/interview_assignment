#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "CircularBuffer.hpp"

int main() {
    try {
        CircularBuffer<double> tempBuffer(5);

        tempBuffer.push_back(23.5);
        tempBuffer.push_back(24.1);
        tempBuffer.push_back(23.8);
        tempBuffer.push_back(25.2);
        tempBuffer.push_back(24.7);
        tempBuffer.push_back(26.1);

        std::cout << "tempBuffer.size() = " << tempBuffer.size() << "\n";
        std::cout << "tempBuffer.capacity() = " << tempBuffer.capacity() << "\n";
        std::cout << "tempBuffer.empty() = " << std::boolalpha << tempBuffer.empty() << "\n";

        double maxTemp = *std::max_element(tempBuffer.begin(), tempBuffer.end());
        double avgTemp = std::accumulate(tempBuffer.begin(), tempBuffer.end(), 0.0) / tempBuffer.size();

        std::cout << std::fixed << std::setprecision(1);
        std::cout << "maxTemp = " << maxTemp << "\n";
        std::cout << std::setprecision(2);
        std::cout << "avgTemp = " << avgTemp << "\n";

        std::cout << std::setprecision(1);
        std::cout << "tempBuffer.front() = " << tempBuffer.front() << "\n";
        std::cout << "tempBuffer.back() = " << tempBuffer.back() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
