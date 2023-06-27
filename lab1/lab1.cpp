#include <iostream>
#include <vector>

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

void find_minmax(const vector<double>& numbers, double& min, double& max) {
    min = numbers[0];
    max = numbers[0];

    for (double x : numbers) {
        if (x < min) {
            min = x;
        }
        else if (x > max) {
            max = x;
        }
    }
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

int main()
{
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;

    const auto numbers = input_numbers(number_count);

    size_t bin_count;
    cerr << "Enter bin count: ";
    cin >> bin_count;

    const auto bins = make_histogram(numbers,bin_count);

    show_histogram_text(bins, numbers);

    return 0;
}
