
/**
 * @param val: The value to be clamped
 * @param min: The minimum value allowed
 * @param max: The maximum value allowed
 * @brief Clamps a double between min and max
 *
 * Added benefit of being beautifully compiled in theory
 * (https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value)
 */
double clampDouble(double val, double min, double max)
{
	// if val < min, use min, otherwise use val
	const double temp = val < min ? min : val;

	// if temp > max, use max, otherwise use val.
	// value is clamped !
	return temp > max ? max : temp;
}

/**
 * @param val: The value to be clamped
 * @param min: The minimum value allowed
 * @param max: The maximum value allowed
 * @brief Clamps a long value between min and max
 */
long clampLong(long val, long min, long max)
{
	const long temp = val < min ? min : val;
	return temp > max ? max : temp;
}

/**
 * @param val: The value to be clamped
 * @param min: The minimum value allowed
 * @param max: The maximum value allowed
 * @brief Clamps an integer value between min and max
 */
int clampInt(int val, int min, int max)
{
	const long temp = val < min ? min : val;
	return temp > max ? max : temp;
}

/**
 * @brief Calculates score
 */
void calcScoreV1()
{
}