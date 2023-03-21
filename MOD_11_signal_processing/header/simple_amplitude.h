#include "moving_average.h"

#pragma once

class simple_amplitude_4_shared {
public:
	simple_amplitude_4_shared(double Threshold, double Min_threshold, double Input_weight) {
		global_average = moving_average_simple(Input_weight);
		threshold = Threshold;
		min_threshold = Min_threshold;
	}
	int update(int micLF_value, int micRF_value, int micLB_value, int micRB_value) {
		double global_average_value = global_average.get_average();
		bool mic_threshold[4];
		mic_threshold[0] = (micLF_value - global_average_value) > threshold && micLF_value > min_threshold;
		mic_threshold[1] = (micRF_value - global_average_value) > threshold && micRF_value > min_threshold;
		mic_threshold[2] = (micRB_value - global_average_value) > threshold && micRB_value > min_threshold;
		mic_threshold[3] = (micLB_value - global_average_value) > threshold && micLB_value > min_threshold;
		//update global_average
		double average = double(micLF_value + micRF_value + micRB_value + micLB_value) / 4.0;
		global_average.insert(average);

		return bool_to_region(mic_threshold);
	}

	int bool_to_region(bool array[4]) {
		/*
		[0] -> micLF
		[1] -> micRF
		[2] -> micRB
		[3] -> micLB
		*/
		if ((array[0] + array[1] + array[2] + array[3]) > 1) {
			return 0;
		}
		else if (array[0]) {
			return 8;
		}
		else if (array[1]) {
			return 2;
		}
		else if (array[2]) {
			return 4;
		}
		else if (array[3]) {
			return 6;
		}
	}

private:
	moving_average_simple global_average;
	double threshold;
	double min_threshold;
};

class simple_amplitude_4_individual {
public:
	simple_amplitude_4_individual(double Threshold, double Min_threshold, double Input_weight) {
		micLF = moving_average(Threshold, Input_weight);
		micRF = moving_average(Threshold, Input_weight);
		micLB = moving_average(Threshold, Input_weight);
		micRB = moving_average(Threshold, Input_weight);
		threshold = Threshold;
		min_threshold = Min_threshold;
	}
	int update(int micLF_value, int micRF_value, int micLB_value, int micRB_value) {

		bool mic_threshold[4];
		mic_threshold[0] = micLF.insert(micLF_value) && micLF_value > min_threshold;
		mic_threshold[1] = micRF.insert(micRF_value) && micRF_value > min_threshold;
		mic_threshold[2] = micRB.insert(micRB_value) && micRB_value > min_threshold;
		mic_threshold[3] = micLB.insert(micLB_value) && micLB_value > min_threshold;

		return bool_to_region(mic_threshold);
	}

	int bool_to_region(bool array[4]) {
		/*
		[0] -> micLF
		[1] -> micRF
		[2] -> micRB
		[3] -> micLB
		*/
		if ((array[0] + array[1] + array[2] + array[3]) > 1) {
			return 0;
		}
		else if (array[0]) {
			return 8;
		}
		else if (array[1]) {
			return 2;
		}
		else if (array[2]) {
			return 4;
		}
		else if (array[3]) {
			return 6;
		}
	}

private:
	moving_average micLF, micRF, micLB, micRB;
	double threshold;
	double min_threshold;
};

class simple_amplitude_8_shared {
public:
	simple_amplitude_8_shared(double Threshold, double Min_threshold, double Input_weight) {
		global_average = moving_average_simple(Input_weight);
		threshold = Threshold;
		min_threshold = Min_threshold;
	}
	int update(int micLF_value, int micRF_value, int micLB_value, int micRB_value) {
		double global_average_value = global_average.get_average();
		bool mic_threshold[4];
		mic_threshold[0] = (micLF_value - global_average_value) > threshold && micLF_value > min_threshold;
		mic_threshold[1] = (micRF_value - global_average_value) > threshold && micRF_value > min_threshold;
		mic_threshold[2] = (micRB_value - global_average_value) > threshold && micRB_value > min_threshold;
		mic_threshold[3] = (micLB_value - global_average_value) > threshold && micLB_value > min_threshold;
		//update global_average
		double average = double(micLF_value + micRF_value + micRB_value + micLB_value) / 4.0;
		global_average.insert(average);

		return bool_to_region(mic_threshold);
	}

	int bool_to_region(bool array[4]) {
		/*
		[0] -> micLF
		[1] -> micRF
		[2] -> micRB
		[3] -> micLB
		*/
		if ((array[0] + array[1] + array[2] + array[3]) > 2) {
			return 0;
		}
		else if (array[0]) {
			return 8;
		}
		else if (array[0] && array[1]) {
			return 1;
		}
		else if (array[1]) {
			return 2;
		}
		else if (array[1] && array[2]) {
			return 3;
		}
		else if (array[2]) {
			return 4;
		}
		else if (array[2] && array[3]) {
			return 5;
		}
		else if (array[3]) {
			return 6;
		}
		else if (array[3] && array[0]) {
			return 7;
		}
	}

private:
	moving_average_simple global_average;
	double threshold;
	double min_threshold;
};

class simple_amplitude_8_individual {
public:
	simple_amplitude_8_individual(double Threshold, double Min_threshold, double Input_weight) {
		micLF = moving_average(Threshold, Input_weight);
		micRF = moving_average(Threshold, Input_weight);
		micLB = moving_average(Threshold, Input_weight);
		micRB = moving_average(Threshold, Input_weight);
		threshold = Threshold;
		min_threshold = Min_threshold;
	}
	int update(int micLF_value, int micRF_value, int micLB_value, int micRB_value) {

		bool mic_threshold[4];
		mic_threshold[0] = micLF.insert(micLF_value) && micLF_value > min_threshold;
		mic_threshold[1] = micRF.insert(micRF_value) && micRF_value > min_threshold;
		mic_threshold[2] = micRB.insert(micRB_value) && micRB_value > min_threshold;
		mic_threshold[3] = micLB.insert(micLB_value) && micLB_value > min_threshold;

		return bool_to_region(mic_threshold);
	}

	int bool_to_region(bool array[4]) {
		/*
		[0] -> micLF
		[1] -> micRF
		[2] -> micRB
		[3] -> micLB
		*/
		if ((array[0] + array[1] + array[2] + array[3]) > 2) {
			return 0;
		}
		else if (array[0]) {
			return 8;
		}
		else if (array[0] && array[1]) {
			return 1;
		}
		else if (array[1]) {
			return 2;
		}
		else if (array[1] && array[2]) {
			return 3;
		}
		else if (array[2]) {
			return 4;
		}
		else if (array[2] && array[3]) {
			return 5;
		}
		else if (array[3]) {
			return 6;
		}
		else if (array[3] && array[0]) {
			return 7;
		}
	}

private:
	moving_average micLF, micRF, micLB, micRB;
	double threshold;
	double min_threshold;
};