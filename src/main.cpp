
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

struct GlucoseReading {
    std::string timestamp;
    int glucose;
};

int main() {
    const int LOW_THRESHOLD = 70;
    const int HIGH_THRESHOLD = 180;

    std::ifstream file("data/glucose.csv");

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open glucose.csv\n";
        return 1;
    }

    std::vector<GlucoseReading> readings;
    std::string line;

    // Skip CSV header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string timestamp, value;

        if (!std::getline(ss, timestamp, ',') ||
            !std::getline(ss, value, ',')) {
            continue;
        }

        try {
            size_t pos = 0;
            int glucose = std::stoi(value, &pos);

            // Ensures that the glucose values are positive, so they are valid
            // Otherwise, if they are negative, it is not possible
            if (pos != value.size() ||
                glucose <= 0 || glucose > 600) {
                std::cerr << "Negative row detected: " << line << ". Please ensure only valid glucose values are there!" << '\n';
                continue;
            }

            readings.push_back({timestamp, glucose});
        }

        // Ensures that the .csv follows the time stamp and glucose formats only
        // Anything else will invalidate the data row
        catch (const std::exception&) {
            std::cerr << "Skipping invalid row: "
                      << line << '\n';
        }
    }

    // Ensures that .csv is populated with data
    if (readings.empty()) {
        std::cerr << "No valid readings found.\n";
        return 1;
    }

    int total = 0;
    int minimum = readings[0].glucose;
    int maximum = readings[0].glucose;
    int low = 0, normal = 0, high = 0;

    for (const auto& reading : readings) {
        int value = reading.glucose;

        total += value;
        minimum = std::min(minimum, value);
        maximum = std::max(maximum, value);

        if (value < LOW_THRESHOLD) {
            low++;
        }
        else if (value > HIGH_THRESHOLD) {
            high++;
        }
        else {
            normal++;
        }
    }

    double average =
        static_cast<double>(total) / readings.size();

    double inRange =
        100.0 * normal / readings.size();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n=== Glucose Analysis Report ===\n";
    std::cout << "Total Readings: " << readings.size() << '\n';
    std::cout << "Average Glucose: " << average << " mg/dL\n";
    std::cout << "Minimum Glucose: " << minimum << " mg/dL\n";
    std::cout << "Maximum Glucose: " << maximum << " mg/dL\n";
    std::cout << "Low Readings: " << low << '\n';
    std::cout << "In-Range Readings: " << normal << '\n';
    std::cout << "High Readings: " << high << '\n';
    std::cout << "Readings In Range: " << inRange << "%\n";

    return 0;
}