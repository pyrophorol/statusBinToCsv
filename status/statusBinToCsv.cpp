#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdint.h>

//not written by me
std::string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

//also not written by me
std::string padTo(std::string str, const size_t num, const char paddingChar = ' ')
{
    if(num > str.size())
	{
        str.insert(0, num - str.size(), paddingChar);
	}
	return str;
}

int main()
{
	uint16_t lastTime;
	
	uint16_t time;
	uint32_t current;
	uint16_t sum_of_cells;
	uint32_t power;
	uint16_t pec_result;
	
	FILE* fbinary;
	fbinary = fopen("status.bin", "rb");
	
	FILE* fcsv;
	fcsv = fopen("status.csv", "w+");
	
	fwrite("uptime[s],", sizeof(char), 10, fcsv);
	fwrite("current[A],", sizeof(char), 11, fcsv);
	fwrite("soc[V],", sizeof(char), 7, fcsv);
	fwrite("power[W],", sizeof(char), 9, fcsv);
	fwrite("PEC[uint16_t],\n", sizeof(char), 16, fcsv);
	
	while(true)
	{
		fread(&time, sizeof(time), 1, fbinary);
		fread(&current, sizeof(current), 1, fbinary);
		fread(&sum_of_cells, sizeof(sum_of_cells), 1, fbinary);
		fread(&power, sizeof(power), 1, fbinary);
		fread(&pec_result, sizeof(pec_result), 1, fbinary);
		
		fwrite(padTo(to_string(time), 5).c_str(), sizeof(char), 5, fcsv);
		fwrite(",", sizeof(char), 1, fcsv);
		fwrite(padTo(to_string(current), 11).c_str(), sizeof(char), 11, fcsv);
		fwrite(",", sizeof(char), 1, fcsv);
		fwrite(padTo(to_string(sum_of_cells), 5).c_str(), sizeof(char), 5, fcsv);
		fwrite(",", sizeof(char), 1, fcsv);
		fwrite(padTo(to_string(power), 11).c_str(), sizeof(char), 11, fcsv);
		fwrite(",", sizeof(char), 1, fcsv);
		fwrite(padTo(to_string(pec_result), 5).c_str(), sizeof(char), 5, fcsv);
		fwrite("\n", sizeof(char), 2, fcsv);
		
		printf("%d\t%d\t%d\t%d\t%d\n", time, current, sum_of_cells, power, pec_result);
		
		if(time==lastTime)
		{
			//for some reason values repeat after reading all values in file, so that i how I will detect end of file because I couldn't figure out how to use any other detection of end of file
			//but this means that if you save values for the same second twice in a row, it will not be converted to csv properly
			break;
		}
		else
		{
			lastTime=time;
		}
	}
	
	fclose(fbinary);
	fclose(fcsv);
	
	getch();
	return 0;
}