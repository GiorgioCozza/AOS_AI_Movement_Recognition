#ifndef _CIRCULAR_QUEUE_H
#define _CIRCULAR_QUEUE_H
//-----------------include----------------
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>

//-----------------define-----------------
#define VECTOR_SIZE					12
#define WINDOW_SIZE					50
#define CIRC_BUFF_SIZE				256

//-----------------class------------------
class circularQueue{
private:
	uint16_t f_idx ;
	uint16_t r_idx  ;
	float *smp_buf;

	//uint8_t static maxSize = 256;

public:
    //for output directly
    int currentLength;
    circularQueue();
	~circularQueue(){ delete [] smp_buf;}
	void clear() { f_idx = 0; r_idx = 0; currentLength = 0; }
	void insert(int32_t *result);  //insert value
    //bool checkReady ();              //check if it already have 50 number
	float* getCircBuf();
	float* getVectSmp(uint16_t pos);
	//void printing();                 //printing the current 50 data
};


#endif