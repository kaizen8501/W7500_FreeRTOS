#ifndef	__SOCKET_H__
#define	__SOCKET_H__

#include "W7500x_wztoe.h"

/*******************************************************************/
#define	MAX_SOCK_NUM	4					// Concurrent maxmium number of socket


void far interrupt in4_isr_i2chip(void);

//void ISR_ESTABLISHED(SOCKET s);
//void ISR_CLOSED(SOCKET s);
//void ISR_RX(SOCKET s);

void initW3100A(void);
void sysinit(u_char sbufsize, u_char rbufsize);
void setsubmask(u_char * addr);
void setgateway(u_char * addr);
void setMACAddr(u_char * addr);
void setIP(u_char * addr);

char socket(SOCKET s, u_char protocol, u_int port, u_char flag);

void setIPprotocol(SOCKET s, u_char ipprotocol);

void setINTMask(u_char mask);
void settimeout(u_char * val);
void setTOS(SOCKET s, u_char tos);

void GetDestAddr(SOCKET s, u_char* addr);

//void setbroadcast(SOCKET s);

char connect(SOCKET s, u_char far * addr, u_int port);
char NBconnect(SOCKET s, u_char far * addr, u_int port);

//char listen(SOCKET s, u_char far * addr, u_int far * port);
char NBlisten(SOCKET s);

void initseqnum(SOCKET s);

int send(SOCKET s, u_char far * buf, u_int len);
int send_in(SOCKET s, u_char far * buf, u_int len);
int recv(SOCKET s, u_char far * buf, u_int len);

u_int sendto(SOCKET , u_char far * buf, u_int, u_char * addr, u_int);
u_int sendto_in(SOCKET , u_char far *, u_int);
u_int recvfrom(SOCKET , u_char far * buf, u_int, u_char * addr, u_int *);

u_int read_data(SOCKET s, u_int src_offset, u_char far * dst, u_int len);
u_int write_data(SOCKET s, u_char far * src, u_int dst_offset, u_int len);

void close(SOCKET s);
char reset_sock(SOCKET s);

u_int select(SOCKET s, u_char func);
void recv_clear(SOCKET s);
u_char tx_empty(SOCKET s);

#endif	// __SOCKET_H__
