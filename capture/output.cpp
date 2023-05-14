#include "capture.h"
#include <map>

/**
 * Requests and print out the channel's indexes,
 * to put down in the output file for each collumns
 *
 * @param	handle: Target device's handle
 * @param	out: Output stream to write on
 *
 * @return	True or False if was successful
 * */
bool defineCollums(t_handle* handle, std::ofstream& out)
{
	// List of all possible entries
	const char* all[] =
	{
		"EEG 1", "EEG 2", "EEG 3", "EEG 4", "EEG 5", "EEG 6", "EEG 7", "EEG 8",
		"Accelerometer X", "Accelerometer Y", "Accelerometer Z",
		"Gyroscope X", "Gyroscope Y", "Gyroscope Z",
		"Battery Level", "Validation Indicator", "Counter",
	};

	std::map<uint32_t, const char*>		map;
	std::pair<const char*, uint32_t>	pair;

	// Get each index of each entries
	for (uint32_t i = 0; i < 17; i++)
	{
		pair = std::make_pair(all[i], 0);
		if (UNICORN_GetChannelIndex(*handle, pair.first, &pair.second))
		{
			std::cout << "Failed fetch channels index : " << pair.first << std::endl;
			return false;
		}
		map.insert(std::make_pair(pair.second, pair.first));
	}

	// Finally put each entries in index order
	for (auto it = map.begin(); it != map.end(); it++)
	{
		if (it != map.begin())
			out << ",";
		out << it->second;
	}

	// Add extra entries collums
	out << ",Display,Direction,Timestamp\n";
	return true;
}

/**
 * Output recording buffers
 *
 * @param	out: Output stream to write on
 * @param	buffer: Buffer to put down in output file
 * @param	buffer_size: Size of the buffer
 * */
void writeValues
	(std::ofstream& out, float* buffer, uint32_t& buffer_size, Data* const data, const long long &dur)
{
	std::string buff;
	for (uint32_t y = 0; y < buffer_size; y++)
	{
		if (y) buff += ',';
		buff += std::to_string(buffer[y]);
	}
	writeDirectives(buff, data);
	buff += ',' + std::to_string(dur) + '\n';

	if (Config::method == 0)
		out << buff;
	else
		send_data(buff);
}

/**
 * Output data display and direction
 *
 * @param	buff: Output stream to write on
 * @param	data: Internal data about the software
 * */
void writeDirectives(std::string& buff, Data* const data)
{
	switch (data->display)
	{
		case Data::Arrow:
			buff += ",Arrow";
			break;
		case Data::Text:
			buff += ",Text";
			break;
		default:
			buff += ",None,None";
			return;
	}

	switch (data->direction)
	{
		case Data::Up:
			buff += ",Up";
			break;
		case Data::Down:
			buff += ",Down";
			break;
		case Data::Right:
			buff += ",Right";
			break;
		case Data::Left:
			buff += ",Left";
			break;
	}
}