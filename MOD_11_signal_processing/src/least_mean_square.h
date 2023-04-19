#include "Utils.h"
#include <math.h>

#pragma once
class least_mean_square {
public:
	least_mean_square(Coord MicLF, Coord MicRF, Coord MicRB, Coord MicLB) {
		micLF = MicLF;
		micRF = MicRF;
		micRB = MicRB;
		micLB = MicLB;
	}

	int find_region(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB,
					float tau_RF_LB, float tau_RF_RB, float tau_LB_RB);

private:
	Coord micLF, micRF, micRB, micLB;

	float v_sound = 343.0f;

	float Pi = 3.14159265358979323846;

	Coord get_difference(Coord mic1, Coord mic2);

	float get_reference_d(Coord mic1, Coord mic2, float angle);

	float get_least_mean_square(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB, 
								float tau_RF_LB, float tau_RF_RB, float tau_LB_RB,
								float angle);

	int compare_least_mean_square(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB,
								  float tau_RF_LB, float tau_RF_RB, float tau_LB_RB);

};

inline int least_mean_square::find_region(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB, float tau_RF_LB, float tau_RF_RB, float tau_LB_RB)
{
	int my_region = compare_least_mean_square(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);

	//Map region from 0 to 7 to 1 to 8
	my_region += 1;
	return my_region;
}

inline Coord least_mean_square::get_difference(Coord mic1, Coord mic2)
{
	Coord result{};
	result.x = mic1.x - mic2.x;
	result.y = mic1.y - mic2.y;
	return result;
}

inline float least_mean_square::get_reference_d(Coord mic1, Coord mic2, float angle)
{
	Coord difference = get_difference(mic2, mic1);
	Coord unit_vector{};
	unit_vector.x = pow(sin(angle), 2);
	unit_vector.y = pow(cos(angle), 2);
	float tau = unit_vector.x * difference.x + unit_vector.y * difference.y;

	return tau;
}

inline float least_mean_square::get_least_mean_square(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB, 
													  float tau_RF_LB, float tau_RF_RB, float tau_LB_RB,
													  float angle)
{
	float d_LF_RF_ref = get_reference_d(micLF, micRF, angle);
	float d_LF_LB_ref = get_reference_d(micLF, micLB, angle); 
	float d_LF_RB_ref = get_reference_d(micLF, micRB, angle);
	float d_RF_LB_ref = get_reference_d(micRF, micLB, angle); 
	float d_RF_RB_ref = get_reference_d(micRF, micRB, angle); 
	float d_LB_RB_ref = get_reference_d(micLB, micRB, angle);

	float e_sqrd_LF_RF = pow(tau_LF_RF * v_sound - d_LF_RF_ref, 2); 
	float e_sqrd_LF_LB = pow(tau_LF_LB * v_sound - d_LF_LB_ref, 2); 
	float e_sqrd_LF_RB = pow(tau_LF_RB * v_sound - d_LF_RB_ref, 2); 
	float e_sqrd_RF_LB = pow(tau_RF_LB * v_sound - d_RF_LB_ref, 2); 
	float e_sqrd_RF_RB = pow(tau_RF_RB * v_sound - d_RF_RB_ref, 2); 
	float e_sqrd_LB_RB = pow(tau_LB_RB * v_sound - d_LB_RB_ref, 2);

	float sum_e_sqrd = e_sqrd_LF_RF + e_sqrd_LF_LB + e_sqrd_LF_RB + e_sqrd_RF_LB + e_sqrd_RF_RB + e_sqrd_LB_RB;

	return sum_e_sqrd;
}

inline int least_mean_square::compare_least_mean_square(float tau_LF_RF, float tau_LF_LB, float tau_LF_RB, 
														float tau_RF_LB, float tau_RF_RB, float tau_LB_RB)
{
	float errors[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	for (int i = 0; i < 8; i++) {
		float angle = float(i) * Pi / 4;
		errors[i] = get_least_mean_square(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB, angle);
	}
	
	int min_error = 0;

	for (int i = 1; i < 8; i++) {
		if (errors[i] < errors[min_error]) {
			min_error = i;
		}
	}

	//Serial.print("errors:\t");
	//for(int i = 0; i < 8; i++){
		//Serial.print(errors[i]);
		//Serial.print('\t');
	//}
	//Serial.println(' ');

	
	//Serial.print("tau_LF_RF:\t");
    //Serial.print((tau_LF_RF*1000000));
    //Serial.print("\ttau_LB_RB:\t");
    //Serial.print((tau_LB_RB*1000000));
    //Serial.print("\ttau_LF_LB:\t");
    //Serial.print(tau_LF_LB*1000000);
    //Serial.print("\ttau_RF_RB:\t");
    //Serial.print(tau_RF_RB*1000000);
    //Serial.print("tau_LF_RF:\t");
    //Serial.print((tau_LF_RF*1000000));
    //Serial.print("\ttau_LB_RB:\t");
    //Serial.println((tau_LB_RB*1000000));

	float d_LF_RF_ref = get_reference_d(micLF, micRF, Pi / 4 * min_error);
	float d_LF_LB_ref = get_reference_d(micLF, micLB, Pi / 4 * min_error); 
	float d_LF_RB_ref = get_reference_d(micLF, micRB, Pi / 4 * min_error);
	float d_RF_LB_ref = get_reference_d(micRF, micLB, Pi / 4 * min_error); 
	float d_RF_RB_ref = get_reference_d(micRF, micRB, Pi / 4 * min_error); 
	float d_LB_RB_ref = get_reference_d(micLB, micRB, Pi / 4 * min_error);

	//Serial.print("d_LF_RF:\t");
    //Serial.print((d_LF_RF_ref/343*1000000));
    //Serial.print("\td_LB_RB:\t");
    //Serial.print((d_LB_RB_ref/343*1000000));
    //Serial.print("\td_LF_LB:\t");
    //Serial.print(d_LF_LB_ref/343*1000000);
    //Serial.print("\td_RF_RB:\t");
    //Serial.print(d_RF_RB_ref/343*1000000);
    //Serial.print("d_LF_RF:\t");
    //Serial.print((d_LF_RF_ref/343*1000000));
    //Serial.print("\td_LB_RB:\t");
    //Serial.println((d_LB_RB_ref/343*1000000));

	//Serial.print("region insider: ");
	//Serial.println(min_error);
	

	return min_error;
}