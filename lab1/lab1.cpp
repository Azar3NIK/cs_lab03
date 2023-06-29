#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>

#include "histogram.h"
#include "svg.h"

using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 4;


vector<double> input_numbers(istream& in, size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}

Input read_input(istream& in, bool prompt) {
    Input data;

    if(prompt)
    cerr << "Enter number count: ";
    size_t number_count;
    in >> number_count;

    if(prompt)
    cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);

    if(prompt)
    cerr << "Enter bin count: ";
    size_t bin_count;
    in >> bin_count;
    data.bin_count = bin_count;

    return data;
}

vector<size_t> make_histogram(Input data) {
    vector<size_t> result(data.bin_count);
    double min, max = 0;
    //Поиск минимального и максимального
    find_minmax(data.numbers, min, max);

    size_t number_count = data.numbers.size();

    double bin_size = (max - min) / data.bin_count;

    for (size_t i = 0; i < number_count; i++) {
        bool found = false;
        for (size_t j = 0; (j < data.bin_count - 1) && !found; j++) {
            auto lo = min + j * bin_size;
            auto hi = min + (static_cast<double>(j) + 1) * bin_size;
            if ((lo <= data.numbers[i]) && (data.numbers[i] < hi)) {
                result[j]++;
                found = true;
            }
        }
        if (!found) {
            result[data.bin_count - 1]++;
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


int main(int argc, char* argv[])
{
    if (argc > 1) {

        CURL* curl = curl_easy_init();

        if (curl) {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
            res = curl_easy_perform(curl);
            cout << res << endl;
            curl_easy_cleanup(curl);
        }
    }
    

    curl_global_init(CURL_GLOBAL_ALL);

    const auto input = read_input(cin, true);

    const auto bins = make_histogram(input);

    show_histogram_svg(bins, input.numbers);

    return 0;
}
