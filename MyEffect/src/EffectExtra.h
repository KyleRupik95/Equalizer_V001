//
//  EffectExtra.h
//  Additional Plugin Code
//
//  This file is a workspace for developing new DSP objects or functions to use in your plugin.
// 

// I attempted making my own filters but I couldn't figure out how to get them to work.
// None of these are used in EffectPlugin.ccp

class LowHighPassFilter {
public:
	void setHighPass(bool highpass) {
		this->highpass = highpass;
	}
	void setCutoffFrequency(float cutoffFrequency) {
		this->cutoffFrequency = cutoffFrequency;
	}
	void setSampleRate(float samplerate) {
		this->samplerate = samplerate;
	}
	float process(float inputSample) {
		const auto sign = highpass ? -1.f : 1.f;
		const auto tan = std::tan(M_PI * cutoffFrequency / samplerate);
		const auto a1 = (tan - 1.f) / (tan + 1.f);

		const auto allpassFilteredSample = a1 * inputSample;

		const auto filterOutput =
			0.5f * (inputSample + sign * allpassFilteredSample);
	}
private:
	bool highpass = false;
	float cutoffFrequency;
	float samplerate;
};

class LPFilter
{
public:
	void set(float coeff) {
		//initialise your filter variables here
        a = coeff;
	    b = 1 - a;
	}
	float process(float input) {
		//filter individual samples here
		// y = ax + by-1
		float y;
		previous = (input * a) + (b * previous);
		return (input * a) + (b * previous);
	}

	float getCutoff() 
	{
		float Cutoff = (acos(1 - (square(a) / (2 * b)) * (getSampleRate() / (2 * M_PI))));
		return Cutoff;
		printf("%p", Cutoff);
	}

	float square(float x)
	{
		float y = x * x;
		return y;
	}

private:
	//declare your internal filter variables here
	float a, b, previous = 0;
};


class BiQuadBSF
{
public:

	float square(float x){
		float y = x * x;
		return y;
	}

	void set(float frequency, float bandwidth) {
		f = frequency;
		BW = bandwidth;
		R = 1 - 3 * BW;
		K = 1 - 2 * R * cos(2 * M_PI * f) + square(R) / 2 - 2 * cos(2 * M_PI * f);

		a_0 = K;
		a_1 = -2 * K * cos(2 * M_PI * f);
		a_2 = K;
		b_1 = 2 * R * cos(2 * M_PI * f);
		b_2 = -square(R);;
	
	}
	float process(float x_0){

		y_0 =(a_0 * x_0) + (a_1 * x_1) + (a_2 * x_2)  // IIR difference equation
				+ (b_1 * y_1) + (b_2 * y_2);

		x_2 = x_1;             	    //x_0 = impulse input , y = output                  
		x_1 = x_0;
		y_2 = y_1;
		y_1 = y_0;

		return y_0;
	}
private:
	float f, BW, R, K, a_0, a_1, a_2, b_1, b_2, 
		  x_1 = 0.0f, x_2 = 0.0f, 
		  y_0 = 0.0f, y_1 = 0.0f, y_2 = 0.0f;

};

/*
class BandFilter BIQUAD FILTER
{
public:
	void filter(const int* x, int* y, int n)
	{
		static float x_2 = 0.0f;                    // delayed x, y samples
		static float x_1 = 0.0f;
		static float y_2 = 0.0f;
		static float y_1 = 0.0f;


		// ONLY NEED THIS PART BELOW. CAN DELETE THE REST. 
		// FEED it X (impulse/input). 
		// Frequency and BW are user inputs. Everything else is derived from those 3 inputted values

		// f = frequency, BW = bandwidth, 
		float BW;
		float f;
		float R = 1 - 3 * BW;
		float K = 1 - 2 * R * cos(2 * M_PI * f) + square(R) / 2 - 2 * cos(2 * M_PI * f);

		float a_0 = K;
		float a_1 = -2 * K * cos(2 * M_PI * f);
		float a_2 = K;
		float b_1 = 2 * R * cos(2 * M_PI * f);
		float b_2 = - square(R);;

		for (int i = 0; i < n; ++i)
		{
			y[i] = a_0 * x[i] + a_1 * x_1 + a_2 * x_2  // IIR difference equation
				+ b_1 * y_1 + b_2 * y_2;
			x_2 = x_1;                              // shift delayed x, y samples
			x_1 = x[i];
			y_2 = y_1;
			y_1 = y[i];
		}
	}
	void set(float coeff){}

	void process(float input){}

private:

	float square(float x)
	{
		float y = x * x;
		return y;
	}
};
*/
