#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include "RenderWindow.hpp"
#include "Fourier.hpp"
#include "SVGParse.hpp"

const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 720;
const int SCREENCENTERX = SCREENWIDTH / 2;
const int SCREENCENTERY = SCREENHEIGHT / 2;
const int circRadius = 100 * (4 / M_PI);
const float scaler = 2.0;
const float xOffSet = 50.0;
const float yOffSet = -200.0;

typedef struct pointDouble {
	double x;
	double y;
} pointDouble;

int main(int argc, char* argv[]) {


	int drawing = 0;
	double time = 0;

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL_Init Failed. Error: " << SDL_GetError() << std::endl;
	}

	RenderWindow window("Fourier Drawing", SCREENWIDTH, SCREENHEIGHT);

	bool windowRunning = true;
	SDL_Event event;

	SDL_Point mousePos = { 0,0 };
	

	std::vector<SDL_FPoint> drawnPoints = {};
	std::vector<SDL_FPoint> drawnPathPoints;
	std::vector<complexNumber> complexPoints;
	std::vector <fourieredValues> transformedValues;

	for (int i = 0; i < argc; i++) {
		std::cout << "Argv[" << i << "] " << argv[i] << " Compare: " << strcmp(argv[i], "-csv") << std::endl;
	}

	if ((argc == 3) and strcmp(argv[1], "-svg") == 0) {
		
		drawing = 1;
		drawnPoints = SVGToPoints(argv[2]);
		
	}

	// x, y separated values
	else if ((argc == 3) and strcmp(argv[1], "-xysep") == 0) {
		drawing = 1;
		std::ifstream fileToRead(argv[2]);
		std::string line;
		while (std::getline(fileToRead, line)) {
			float numX = 0;
			float numY = 0;
			bool writingX = false, writingY = false;
			int numLen = -2;

			for (int i = 0; i < line.size(); i++) {

				if (line[i] == 'x') {
					writingX = true;
				}
				else if (line[i] == 'y') {
					writingY = true;
				}

				else if (line[i] == ',') {
					if (writingX) {
						numX = std::stof(line.substr(i - numLen, numLen)) + SCREENCENTERX;
					}
					else if (writingY) {
						numY = std::stof(line.substr(i - numLen, numLen - 2)) + SCREENCENTERY;
					}
					writingX = false;
					writingY = false;
					numLen = -2;
				}

				if (writingX or writingY) {
					numLen++;
				}
			}

			drawnPoints.push_back({ numX, numY });
		}
	}

	
	// Comma Separated Values
	else if ((argc == 3) and strcmp(argv[1], "-csv") == 0) {
		std::cout << "hello" << std::endl;
		drawing = 1;
		std::ifstream fileToRead(argv[2]);
		std::string line;
		while (std::getline(fileToRead, line)) {
			float numX = 0;
			float numY = 0;
			bool writingX = false, writingY = false;
			int numLen = 0;

			for (int i = 0; i < line.size(); i++) {


				if (line[i] == ',') {
					numX = (std::stof(line.substr(i - numLen, numLen)) / scaler) + SCREENCENTERX / 2;
					numLen = -1;
				}

				if (&line[i] == &(line.back())) {
					numY = (std::stof(line.substr(i - numLen, numLen)) / scaler) + SCREENCENTERY / 2;
				}

				numLen++;
			}

			drawnPoints.push_back({ numX, numY });
		}
	}
	
	while (windowRunning) {

		SDL_UpdateTexture(window.texture, NULL, window.pixels, SCREENHEIGHT * sizeof(Uint32));

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				windowRunning = false;
			}
		}

		window.clear();

		if (SDL_GetMouseState(&(mousePos.x), &(mousePos.y)) == 1 and drawing == 2){
			drawnPoints.clear();
			time = 0;
			drawnPathPoints.clear();
		}

		if (SDL_GetMouseState(&(mousePos.x), &(mousePos.y)) == 1) {
			drawing = 1;

			SDL_FPoint newPoint = { mousePos.x, mousePos.y};

			drawnPoints.push_back(newPoint);
			window.drawLines(255, 255, 255, 100, &drawnPoints[0], drawnPoints.size());
		}



		if (SDL_GetMouseState(&(mousePos.x), &(mousePos.y)) == 0 and drawing == 1) {
			drawing = 2;
			complexPoints = ComplexifyPoints(drawnPoints, SCREENCENTERX, SCREENCENTERY);
			transformedValues = Fourierizer(complexPoints);

			std::sort(transformedValues.begin(), transformedValues.end());
			//transformedValues[0] ={xOffSet, yOffSet, 0, (sqrt(xOffSet*xOffSet + yOffSet *yOffSet)), atan2(yOffSet, xOffSet)};

			// Adjust each frequency to be scalable without noise
			for (fourieredValues& circle : transformedValues) {
				while (circle.frequency > (transformedValues.size())/2) {
					circle.frequency -= transformedValues.size();
				}
			}
			

		}

		if (drawing == 2) {
			SDL_FPoint currentEndPoint = { SCREENCENTERX, SCREENCENTERY }, previousEndPoint = { SCREENCENTERX, SCREENCENTERY };


			for (fourieredValues& circle : transformedValues) {
				window.drawCircle(255, 0, 0, 100, currentEndPoint.x, currentEndPoint.y, circle.amplitude);

				currentEndPoint.x += (circle.amplitude) * cos(circle.frequency * time + circle.phase);
				currentEndPoint.y += (circle.amplitude) * sin(circle.frequency * time + circle.phase);

				
				window.drawLine(0, 0, 255, 100, previousEndPoint.x, previousEndPoint.y, currentEndPoint.x, currentEndPoint.y);
				previousEndPoint.x = currentEndPoint.x;
				previousEndPoint.y = currentEndPoint.y;

				//std::cout << " Re: " << circle.re << " Im: " << circle.im << " Freq: " << circle.frequency << " Amp: " << circle.amplitude << " Phase: " << circle.phase << std::endl;
			}
			
			drawnPathPoints.push_back({ currentEndPoint.x, currentEndPoint.y });
			window.drawLines(0, 255, 255, 100, & drawnPathPoints[0], drawnPathPoints.size());

			const float dt = ((2 * M_PI) / (transformedValues.size()));
			time += dt;

			if (time > 2.0 * M_PI) {
				time = 0;
				drawnPathPoints.clear();
			}

		}


		//std::cout << "Mouse State:" << SDL_GetMouseState(&(mousePos.x), &(mousePos.y)) << " Xpos: " << mousePos.x << " Ypos: " << mousePos.y << std::endl;



		if (!drawnPoints.empty()) {
			//window.drawLines(255, 255, 255, 100, &drawnPoints[0], drawnPoints.size());

		}

		
		window.display();
		
	}


	window.cleanUp();
	SDL_Quit();
	return 0;
}