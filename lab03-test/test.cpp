#include "histogram.h"
#include "svg.h"

#include <cassert>

void test_positive() {
    double min = 0;
    double max = 0;
    find_minmax({ 1, 2, 3 }, min, max);
    assert(min == 1);
    assert(max == 3);
}

void test_negative() {
    double min = 0;
    double max = 0;
    find_minmax({ -1, -2, -3 }, min, max);
    assert(min == -3);
    assert(max == -1);
}

void test_identical() {
    double min = 0;
    double max = 0;
    find_minmax({ 5, 5, 5 }, min, max);
    assert(min == 5);
    assert(max == 5);
}

void test_single() {
    double min = 0;
    double max = 0;
    find_minmax({ 5 }, min, max);
    assert(min == 5);
    assert(max == 5);
}

void test_empty() {
    double min = 0;
    double max = 0;
    find_minmax({ }, min, max);
    assert(min == 0);
    assert(max == 0);
}

void test_pos() {
    double hi = 6.522342654;
    string str = svg_edges(hi);
    assert(str == "6.52");
}

void test_neg() {
    double hi = -6.522342654;
    string str = svg_edges(hi);
    assert(str == "-6.52");
}

int main() {

    test_positive();

    test_negative();

    test_identical();

    test_single();

    test_empty();

    test_pos();

    test_neg();
}