/*
 * MathCalcs.h
 *
 *  Created on: May 20, 2020
 *      Author: IulianPopa
 */

#ifndef MATHCALCS_H_
#define MATHCALCS_H_

class MathCalcs {
public:
	static float sin(float clockWiseAngle) {
		const unsigned int cachedIdx = normalizeDeg(clockWiseAngle) * 10.0f;
		return sinCache[cachedIdx];
	}

	static float sinRad(float clockWiseAngle) {return sin(rad2deg(clockWiseAngle)); }

	static float asinRad(float x);
	static float atanRad(float x);

	static float cos(float clockWiseAngle) {
		const unsigned int cachedIdx = normalizeDeg(clockWiseAngle) * 10.0f;
		return cosCache[cachedIdx];
	}
	static float cosRad(float clockWiseAngle) {return cos(rad2deg(clockWiseAngle)); }

	static constexpr float rad2deg (const float rad) { return rad * 180.0f / 3.1415f; }
	static constexpr float deg2rad (const float deg) { return deg * 3.1415f / 180.0f; }

	static constexpr float normalizeDeg(float deg) {
		while (deg < .0f) deg += 360.0f;
		while (deg >= 360.0f) deg -= 360.0f;
		return deg;
	}

	static constexpr float normalizeRad(float rad) {return deg2rad(normalizeDeg(rad2deg(rad))); }

	static void InitCaches();

protected:
	MathCalcs() = delete;

	static float sinCache[3600];
	static float cosCache[3600];
};


#endif /* MATHCALCS_H_ */
