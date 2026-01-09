#include <iostream>
#include <iomanip>
#include <numeric>
#include <chrono>
#include <thread>
#include "ParallelProcessor.h"

int main() {
    try {
        std::vector<int> pixelData(1000000);
        std::iota(pixelData.begin(), pixelData.end(), 0);

        ParallelProcessor processor(4);

        auto brightenedImage = processor.parallel_map(pixelData, [](int pixel) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            return std::min(255, pixel + 50);
        });

        auto pixelStrings = processor.parallel_map(pixelData, [](int pixel) -> std::string {
            return "pixel_" + std::to_string(pixel);
        });

        auto squaredPixels = processor.parallel_map(pixelData, [](int pixel) {
            return pixel * pixel;
        });

        std::cout << "brightenedImage[0] = " << brightenedImage[0] << "\n";
        std::cout << "brightenedImage[1] = " << brightenedImage[1] << "\n";
        std::cout << "brightenedImage[100] = " << brightenedImage[100] << "\n";
        std::cout << "brightenedImage[999999] = " << brightenedImage[999999] << "\n\n";

        std::cout << "pixelStrings[0] = \"" << pixelStrings[0] << "\"\n";
        std::cout << "pixelStrings[1] = \"" << pixelStrings[1] << "\"\n";
        std::cout << "pixelStrings[100] = \"" << pixelStrings[100] << "\"\n\n";

        std::cout << "squaredPixels[0] = " << squaredPixels[0] << "\n";
        std::cout << "squaredPixels[1] = " << squaredPixels[1] << "\n";
        std::cout << "squaredPixels[10] = " << squaredPixels[10] << "\n\n";

        auto func = [](int pixel) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            return std::min(255, pixel + 50);
        };

        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> sequential_result(pixelData.size());
        for (size_t i = 0; i < pixelData.size(); ++i) {
            sequential_result[i] = func(pixelData[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        auto parallel_result = processor.parallel_map(pixelData, func);
        end = std::chrono::high_resolution_clock::now();
        auto parallel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        double speedup = static_cast<double>(sequential_time) / parallel_time;

        std::cout << "Processing " << pixelData.size() << " elements with "
                  << processor.get_num_threads() << " threads\n";
        std::cout << "Sequential time: " << sequential_time << "ms\n";
        std::cout << "Parallel time: " << parallel_time << "ms\n";
        std::cout << "Speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
