#include "capture.h"

#include <vector>
#include <cmath>

typedef std::vector<std::vector<float>>		storage_type;

/**
 * Prepares the storage and modifies accordingly
 * to its current status (either empty or full)
 * 
 * @param	in: Storage for inputs
 * @param	filter: Storage for filtered inputs
 * @param	buffer: New series of inputs to store
 * */
void prepareStorage(storage_type &in, storage_type &filter, float* buffer)
{
	// Pre-allocate memory on first call
	if (in.empty())
	{
		in.reserve(Config::max_storage);
		filter.reserve(Config::max_storage);
	}

	// Erases oldest stored value in case the vector is at full capacity
	if (in.size() == in.capacity())
	{
		in.erase(in.begin());
		filter.erase(filter.begin());
	}

	// Saves the new buffer
	std::vector<float>	new_input(Config::channels);
	for (uint8_t y = 0; y < Config::channels; y++)
		new_input[y] = buffer[y];
	in.push_back(new_input);

	// Also prepares new filter storage
	filter.push_back(std::vector<float>(Config::channels));
}

/**
 * Calculates the ratio using the timestamp
 * 
 * @param	time_diff: Difference between last and current inputs
 * 
 * @return	Newly calculated ratio
 * */
float calculate_ratio(uint64_t time_diff)
{
	float target_period = 1.0f / static_cast<float>(Config::filter_sample);
	return (target_period / time_diff);
}

/**
 * Apply a band-pass filtering on the input and stores
 * within the filtered storage using a simplified
 * Butterworth algorithm to reduce aliasing
 * 
 * @param	input: Storage for inputs
 * @param	filtered: Storage for filtered inputs
 * */
void applyFiltering(storage_type& input, storage_type& filtered)
{
	// Defines filters coefficients and constants
	const float		b[3] = { 0.051f,  0.102f, 0.051f };
	const float		a[3] = { 1.000f, -0.836f, 0.483f };
	const size_t	N = input.size();
	int32_t			m;

	// Performs filtering on each EEGs channels
	for (uint32_t i = 0; i < Config::channels; i++)
	{
		// Apply the Butterworth filtering
		for (uint32_t y = 0; y < N; y++)
		{
			filtered[y][i] = b[0] * input[y][i];
			for (uint32_t k = 0; k < 3; k++)
			{
				m = y - k;
				if (m >= 0)
					filtered[y][i] += b[k] * input[m][i];
				if (m - 1 >= 0)
					filtered[y][i] -= a[k] * filtered[m - 1][i];
			}
		}
	}
}

/**
 * Interpolate the filtered values to calibrate
 * to recalibrate to the desired new sample rate
 * using the Sinc Function
 * 
 * @param	filtered: Storage for filtered inputs
 * @param	output: Storage for outputs
 * */
void interpolateValues(storage_type& filtered, storage_type& output, float& ratio)
{
	// Defines constants
	const float		PI = 3.1415f;
	const size_t	N = static_cast<size_t>(ceil(filtered.size() * ratio));
	double			K = Config::interpolation_factor;

	// Prepares output
	output.resize(N);
	for (uint32_t n = 0; n < N; n++)
		output[n] = std::vector<float>(Config::channels);
	
	// Apply interpolation on each EEGs channels
	for (uint32_t i = 0; i < Config::channels; i++)
	{
		// Apply interpolation
		for (uint32_t n = 0; n < N; n++)
		{
			float	sum = 0.0f;
			float	t = (n + 0.5f) / ratio;
			int32_t	m = (int)t;
			for (int32_t k = static_cast<int32_t>(-K); k <= K; k++)
			{
				if (m + k >= 0 && m + k < N)
					sum += filtered[m + k][i] * sin(PI * (t - (m + k))) / (PI * (t - (m + k)));
			}
			output[n][i] = sum;
		}
	}
}

/**
 * Filters out the 250 Hz sample rate into
 * a 1-30 Hz bandpass using moving average
 *
 * @param	buffer: Buffer to filter out
 * @param	time_diff: Difference between last and current inputs
 * */
void filterBuffer(float* buffer, uint64_t time_diff)
{
	// Storage of inputs and output history
	static storage_type		input;
	static storage_type		filtered;
	static storage_type		output;

	// Steps by steps
	prepareStorage(input, filtered, buffer);
	float ratio = calculate_ratio(time_diff);
	applyFiltering(input, filtered);
	interpolateValues(filtered, output, ratio);

	// Swaps the buffer with the new output
	const size_t	N = output.size() - 1;
	for (uint32_t i = 0; i < Config::channels; i++)
		buffer[i] = output[N][i];
}