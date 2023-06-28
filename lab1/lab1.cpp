#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "histogram.h"

using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 4;

vector<double> input_numbers(size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        cin >> result[i];
    }
    return result;
}

vector<size_t> make_histogram(const vector<double>& numbers, size_t bin_count) {
    vector<size_t> result(bin_count);
    double min, max = 0;
    //Поиск минимального и максимального
    find_minmax(numbers, min, max);

    size_t number_count = numbers.size();

    double bin_size = (max - min) / bin_count;

    for (size_t i = 0; i < number_count; i++) {
        bool found = false;
        for (size_t j = 0; (j < bin_count - 1) && !found; j++) {
            auto lo = min + j * bin_size;
            auto hi = min + (static_cast<double>(j) + 1) * bin_size;
            if ((lo <= numbers[i]) && (numbers[i] < hi)) {
                result[j]++;
                found = true;
            }
        }
        if (!found) {
            result[bin_count - 1]++;
        }
    }
    return result;
};

vector<std::string> COLORS = { "#DC143C", "#98FB98", "#20B2AA", "#00FFFF", "#FFFF00", "#FF00FF",
                                   "#FFDEAD", "#D2691E", "#8000FF", "#987654", "#7B68EE", "#FFEC8B",
                                   "#9FE2BF", "#D2B48C", "#464531", "#C1876B", "#CDB891", "#734A12" };

string get_random_color() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, COLORS.size() - 1);
    return COLORS[dist(rng)];
}

void show_histogram_text(const vector<size_t>& bins, const vector<double>& numbers) {

    double min, max;
    //Поиск минимального и максимального
    find_minmax(numbers, min, max);

    size_t max_count = 0;

    size_t bin_count = bins.size();

    double bin_size = (max - min) / bin_count;

    for (size_t i = 0; i < bins.size(); i++) {
        if (bins[i] > max_count)
            max_count = bins[i];
    }

    for (size_t i = 0; i < bins.size(); i++) {
        if (bins[i] < 1000)
            cout << "   ";
        if (bins[i] < 100)
            cout << " "; // Поменять пробел для тестов вне варианта
        if (bins[i] < 10)
            cout << " ";  // Поменять пробел для тестов вне варианта
        cout << bins[i] << "|";

        size_t count = bins[i];

        size_t height = MAX_ASTERISK * (static_cast<double>(count) / max_count);

        for (size_t j = 0; j < height; j++) {
            cout << "*";
        }
        cout << endl;
        auto hi = min + (static_cast<double>(i) + 1) * bin_size;
        cout.precision(2);
        cout << hi << endl;
    }
};

void svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text) {
    cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke = "black", string fill = "black") {
    cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' stroke='" << stroke;
    cout << "' fill='" << fill;
    cout << "'/>";
}

void show_histogram_svg(const vector<size_t>& bins) {
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

    size_t max_count = bins[0];
    size_t bin_count = bins.size();
    for (size_t i = 1; i < bin_count; i++) {
        if (max_count < bins[i])
            max_count = bins[i];
    }

    double top = 0;
    for (size_t bin : bins) {
        const double bin_width = static_cast<size_t>(IMAGE_WIDTH * 0.5 * (static_cast<double>(bin) / max_count));
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "#6699FF", get_random_color());
        top += BIN_HEIGHT;
    }

    svg_end();
}


int main()
{
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;

    if (number_count <= 0) {
        cerr << "number count cannot be equal or below zero";
        return 0;
    }

    const auto numbers = input_numbers(number_count);

    if (numbers.size() == 0) {
        cerr << "numbers cannot be empty";
    }

    size_t bin_count;
    cerr << "Enter bin count: ";
    cin >> bin_count;

    if (bin_count <= 0) {
        cerr << "bin count cannot be equal or below zero";
        return 0;
    }

    const auto bins = make_histogram(numbers,bin_count);

   // show_histogram_text(bins, numbers);
    show_histogram_svg(bins);

    return 0;
}
