#include "utils.h"
#include <math.h>
#include <utility>
#pragma once

class find_source {
public:
	find_source(){}


private:
	Coord micLF, micRF, micLB, micRB;
	const float v_sound = 331.0f;
	const double pi = 3.14159265358979323846;

	std::pair<float, float> calculation(float d1, float d2, Coord mic_ref, Coord mic1, Coord mic2);
	int angle_to_region(float angle);
};


std::pair<float, float> find_source::calculation(float tau1, float tau2, Coord mic_ref, Coord mic1, Coord mic2)
{
	float d_ref_to_1_sqrd = pow((mic_ref.x - mic1.x), 2) + pow((mic_ref.y - mic1.y), 2);
	float d_ref_to_2_sqrd = pow((mic_ref.x - mic2.x), 2) + pow((mic_ref.y - mic2.y), 2);

	float a = 2 * v_sound * abs(tau1);
	float b = 2 * v_sound * abs(tau2);
	float c = pow((v_sound * tau1), 2) - d_ref_to_1_sqrd;
	float d = pow((v_sound * tau2), 2) - d_ref_to_2_sqrd;
	float e = 4 * d_ref_to_1_sqrd;
	float f = 4 * d_ref_to_2_sqrd;
	float abc_a = f * pow(a, 2) + e * pow(b, 2) - f * e;
	float abc_b = -2 * b * e * d - 2 * a * c * f;
	float abc_d = pow(c, 2) * f + e * pow(d, 2);

	float abc_d = sqrt(pow(abc_b, 2) - 4 * abc_a * abc_d);
	if (abc_d < 0) {
		return {-1,-1};
	}

	float abc_x1 = (abc_b + abc_d) / (2 * abc_a);
	float abc_x2 = (abc_b - abc_d) / (2 * abc_a);

	//ANG(1, ff) = acos((a1 * B(ff, 1) + c1) / (2 * B(ff, 1) * d13 * dx)); ANG(2, ff) = acos((b1 * B(ff, 2) + d1) / (2 * B(ff, 2) * d13 * dx));

	float angle_x1 = acos((a * abc_x1 + c) / (2 * abc_x1 * sqrt(d_ref_to_2_sqrd)));
	float angle_x1 = acos((b * abc_x2 + d) / (2 * abc_x1 * sqrt(d_ref_to_2_sqrd)));


	return {0,0};
}

int find_source::angle_to_region(float angle) {
	//angle range expected between 0 and 2pi

	if (angle < pi * float(1/16)) {
		return 1;
	}
	else if (angle < pi * float(3 * 16)) {
		return 2;
	}
	else if (angle < pi * float(5 * 16)) {
		return 3;
	}
	else if (angle < pi * float(7 * 16)) {
		return 4;
	}
	else if (angle < pi * float(9 * 16)) {
		return 5;
	}
	else if (angle < pi * float(11 * 16)) {
		return 6;
	}
	else if (angle < pi * float(13 * 16)) {
		return 7;
	}
	else if (angle < pi * float(15 * 16)) {
		return 8;
	}
	else {
		return 1;
	}
}