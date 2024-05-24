#pragma once
#include <vector>
#include <SDL.h>
#include <string>

float readValue(std::string line, int* lineIndex, char readSign);
std::vector<SDL_FPoint> SVGToPoints(const char* file);
