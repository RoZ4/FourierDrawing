#pragma once

#include <SDL.h>
#include <math.h>
#include <vector>

#include "Fourier.hpp"

std::vector <fourieredValues> Fourierizer(std::vector<complexNumber> inputVals) {
	int numberOfValues = inputVals.size();
	std::vector<fourieredValues> outputValues;

	for (int k = 0; k < numberOfValues; k++) {
		complexNumber currentComplexNumber = { 0,0 };
		
		for (int n = 0; n < numberOfValues; n++) {
			double angleOffSet = (2 * M_PI * k * n) / numberOfValues;
			complexNumber intermediateComplexNumber = { cos(angleOffSet), -sin(angleOffSet) };
			currentComplexNumber = currentComplexNumber.add(inputVals[n].multiply(intermediateComplexNumber));
		}

		currentComplexNumber.re /= numberOfValues;
		currentComplexNumber.im /= numberOfValues;

		double frequency = k;
		double amplitude = currentComplexNumber.getModulos();
		double phase = currentComplexNumber.getPhaseShift();

		fourieredValues currentVals = { currentComplexNumber.re, currentComplexNumber.im, frequency, amplitude, phase };
		outputValues.push_back(currentVals);
	}

	return outputValues;
}

std::vector <complexNumber> ComplexifyPoints(std::vector<SDL_FPoint> points, int ScreenCenterX, int ScreenCenterY) {

	std::vector <complexNumber> complexPoints;

	for (SDL_FPoint& point : points) {
		complexPoints.push_back({ point.x - ScreenCenterX, point.y - ScreenCenterY});
	}

	return complexPoints;
}