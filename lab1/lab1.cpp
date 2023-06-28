#include <iostream>
#include <vector>
#include <string>

#include "histogram.h"
#include "svg.h"

using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 4;

struct Input {
    vector<double> numbers;
    size_t bin_count;
};

vector<double> input_numbers(istream& in, size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}

Input read_input(istream& in) {
    Input data;

    cerr << "Enter number count: ";
    size_t number_count;
    in >> number_count;

    cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);

    size_t bin_count;
    cerr << "Enter bin count: ";
    in >> bin_count;
    data.bin_count = bin_count;

    return data;
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

void show_histogram_svg(const vector<size_t>& bins, const vector<double>& numbers) {

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

    double min, max;
    //Поиск минимального и максимального
    find_minmax(numbers, min, max);

    size_t bin_count = bins.size();

    double bin_size = (max - min) / bin_count;

    size_t max_count = bins[0];
    for (size_t i = 1; i < bin_count; i++) {
        if (max_count < bins[i])
            max_count = bins[i];
    }

    double top = 0;
    size_t counter = 0;
    for (size_t bin : bins) {
        const double bin_width = static_cast<size_t>(IMAGE_WIDTH * 0.5 * (static_cast<double>(bin) / max_count));
        svg_text(TEXT_LEFT + EDGE_WIDTH, top + TEXT_BASELINE, to_string(bin));
        svg_rect(TEXT_WIDTH + EDGE_WIDTH, top, bin_width, BIN_HEIGHT, "#6699FF", get_random_color());
        auto hi = min + (static_cast<double>(counter) + 1) * bin_size;
        svg_text(TEXT_LEFT, top + TEXT_BASELINE + EDGE_HEIGHT, svg_edges(hi));
        top += (BIN_HEIGHT + EDGE_HEIGHT);
        counter++;
    }

    svg_end();
}


int main()
{

    Input data = read_input(cin);

    const auto bins = make_histogram(data.numbers, data.bin_count);

   // show_histogram_text(bins, numbers);
    show_histogram_svg(bins, data.numbers);

    return 0;
}
