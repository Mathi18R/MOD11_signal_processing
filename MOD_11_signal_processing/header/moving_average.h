#pragma once
class moving_average
{
	moving_average(double Threshold, double Input_weight) {
		average = 0;
		threshold = Threshold;
		input_weight = Input_weight;
	}

	bool insert(double input) {
		double averageNew = (1 - input_weight) * average + input_weight * input;
		if (averageNew - average > threshold) {
			average = averageNew;
			return true;
		}
		average = averageNew;
		return false;
	}


private:
	double threshold;
	double input_weight;
	double average;
};

