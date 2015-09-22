/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTIL_H
#define __UTIL_H


#include <stdio.h>
#include <stdint.h>

uint32_t str_to_ip(const char* str);
uint8_t* str_to_ip_array(const char* str);

#endif //__UTIL_H
