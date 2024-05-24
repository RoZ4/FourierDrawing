#pragma once
#include <SDL.h>
#include <math.h>
#include <vector>

typedef struct fourieredValues {
	double re;
	double im;
	double frequency;
	double amplitude;
	double phase;

	bool operator <(const fourieredValues& a) const {
		return amplitude > a.amplitude;
	}
} fourieredValues;

typedef struct complexNumber {
	double re;
	double im;

	double getModulos() {
		return sqrt(re * re + im * im);
	}

	double getPhaseShift() {
		return atan2(im, re);
	}

	complexNumber add(complexNumber c) {
		return { re + c.re, im + c.im };
	}

	complexNumber multiply(complexNumber c) {
		return { ((re * c.re) - (im * c.im)), ((re * c.im) + (im * c.re)) };
	}
} complexNumber;

std::vector<fourieredValues> Fourierizer(std::vector<complexNumber> inputVals);
std::vector<complexNumber> ComplexifyPoints(std::vector<SDL_FPoint> points, int ScreenCenterX, int ScreenCenterY);
