#pragma once

#include <random>
#include <vector>
#include <iostream>

using namespace std;

const auto IMAGE_WIDTH = 400;
const auto IMAGE_HEIGHT = 300;
const auto TEXT_LEFT = 20;
const auto TEXT_BASELINE = 20;
const auto TEXT_WIDTH = 50;
const auto BIN_HEIGHT = 30;
const auto BLOCK_WIDTH = 10;


string get_random_color();

void svg_begin(double width, double height);

void svg_end();

void svg_text(double left, double baseline, string text);

void svg_rect(double x, double y, double width, double height, string stroke = "black", string fill = "black");