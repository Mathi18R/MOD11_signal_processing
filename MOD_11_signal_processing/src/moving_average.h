#pragma once
class moving_average_factor_threshold
{
public:
	moving_average_factor_threshold(double Threshold_factor, double Input_weight) {
		average = 0;
		threshold_factor = Threshold_factor;
		input_weight = Input_weight;
	}

	bool insert(double input) {
		double averageNew = (1 - input_weight) * average + input_weight * input;
		bool threshold_reached = false;
		if (averageNew > threshold_factor*average) {
			threshold_reached = true;
			Serial.println("------------------------------");
		}
		average = averageNew;
		Serial.println(average/1000000000000.0f);
		return threshold_reached;
	}


private:
	double threshold_factor;
	double input_weight;
	double average;
};

class moving_average_constant_threshold
{
public:
	moving_average_constant_threshold(double Threshold, double Input_weight) {
		average = 0;
		threshold = Threshold;
		input_weight = Input_weight;
	}

	bool insert(double input) {
		double averageNew = (1 - input_weight) * average + input_weight * input;
		bool threshold_reached = false;
		if (averageNew > threshold) {
			threshold_reached = true;
		}
		average = averageNew;
		return threshold_reached;
	}


private:
	double threshold;
	double input_weight;
	double average;
};

class moving_average_difference_threshold
{
public:
	moving_average_difference_threshold(double Threshold, double Input_weight) {
		average = 0;
		threshold = Threshold;
		input_weight = Input_weight;
	}

	bool insert(double input) {
		double averageNew = (1 - input_weight) * average + input_weight * input;
		bool threshold_reached = false;
		if (averageNew - average > threshold) {
			threshold_reached = true;
		}
		average = averageNew;
		return threshold_reached;
	}


private:
	double threshold;
	double input_weight;
	double average;
};

class moving_average_simple
{
public:
	moving_average_simple(double Input_weight) {
		average = 0;
		input_weight = Input_weight;
	}

	void insert(double input) {
		average = (1 - input_weight) * average + input_weight * input;
	}

	double get_average() {
		return average;
	}

private:
	double input_weight;
	double average;
};