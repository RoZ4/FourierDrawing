#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>

const float scale = 0.5;

float readValue(std::string line, int* lineIndex, char readSign) {
	int numLen = 0;
	*lineIndex += 1;
	char currentChar = line[*lineIndex + numLen];

	if (line[*lineIndex + numLen] == '-') {
		return readValue(line, lineIndex, '-');
	}

	while ((line[*lineIndex + numLen] >= int('0') and line[*lineIndex + numLen] <= int('9')) or line[*lineIndex + numLen] == int('.')) {
		numLen++;
		currentChar = line[*lineIndex + numLen];
	}

	*lineIndex += numLen;

	float value = std::stof(line.substr(*lineIndex-numLen, numLen))*scale;

	if (readSign == '-') {
		return -value;
	}

	return value;


	std::cout << "ERROR FINDING READTYPE.   READTYPE: " << readSign << std::endl;
	return -1;
}

std::vector<SDL_FPoint> SVGToPoints(const char* file) {

	std::cout << file << std::endl;
	
	std::vector<SDL_FPoint> points;
	std::ifstream fileToRead(file);
	std::string line;
	bool readingValues = false;
	float numX = 0;
	float numY = 0;

	while (std::getline(fileToRead, line)) {

		for (int i = 0; i < line.size(); i++) {
			char currentChar = line[i];

			if (line[i] == '\t') {
				continue;
			}

			else if (!readingValues and line[i] == ' ' and line[i + 1] == 'd' and line[i + 2] == '=' and line[i + 3] == '"') {
				readingValues = true;
				i += 3;
				currentChar = line[i];
			}

			else if (readingValues) {

				switch (line[i]) {
				case 'M':
					numX = readValue(line, &i, line[i]) +150;
					numY = readValue(line, &i, line[i]);
					break;

				case 'c':
					float cGarbageValue;
					for (int j = 0; j < 4; j++) {
						cGarbageValue = readValue(line, &i, line[i]);
					}
					numX += readValue(line, &i, line[i]);
					numY += readValue(line, &i, line[i]);
					break;

				case 'C':
					cGarbageValue;
					for (int j = 0; j < 4; j++) {
						cGarbageValue = readValue(line, &i, line[i]);
					}
					numX = readValue(line, &i, line[i]) + 150;
					numY = readValue(line, &i, line[i]);
					break;

				
				case 'l':
					numX += readValue(line, &i, line[i]);
					numY += readValue(line, &i, line[i]);
					break;

				case 's':
					float sGarbageValue;
					for (int j = 0; j < 2; j++) {
						sGarbageValue = readValue(line, &i, line[i]);
					}
					numX += readValue(line, &i, line[i]);
					numY += readValue(line, &i, line[i]);
					break;

				case 'z':
					return points;
				}


				i--;
				points.push_back({ numX, numY });

			}


		}
	}

	return points;
}

