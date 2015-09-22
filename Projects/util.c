#include "util.h"
#include <stdlib.h>
#include <string.h>


uint32_t str_to_ip(const char* str)
{
    int i;
	uint32_t ip = 0;
	char* p = (char*)str;
	for(i = 0; i < 4; i++) {
		ip |= atoi(p);
		p = strchr(p, '.');
		if (p == NULL) {
			break;
		}
		ip <<= 8;
		p++;
	}
	return ip;
}

uint8_t* str_to_ip_array(const char* str)
{
    int i;
    uint8_t temp[4];
	char* p = (char*)str;
	for(i = 0; i < 4; i++) {
		temp[i] = atoi(p);
		p = strchr(p, '.');
		if (p == NULL) {
			break;
		}
		p++;
	}
	return temp;
}

