#ifndef _CIRCULAR_QUEUE_H
#define _CIRCULAR_QUEUE_H
//-----------------include----------------
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>
#include "main_config.h"


//-----------------class------------------
template <typename T>
class circularQueue{
private:
	uint16_t f_idx ;
	uint16_t r_idx  ;
	T *smp_buf;
	uint16_t size;
	bool ready;
	//uint8_t static maxSize = 256;

public:
    //for output directly
    int currentLength;
    circularQueue(uint16_t size);
	~circularQueue(){ delete [] smp_buf;}
	void clear() { f_idx = 0; r_idx = 0; currentLength = 0; }
	void insert(const T *result, const uint8_t vec_sz);  //insert value
    bool isReady (){ return ready; };              //check if it already have 50 number
	T* getCircBuf();

};


template<typename T>
circularQueue<T>::circularQueue(uint16_t size) {
	
	f_idx = 0;
	r_idx = 0;
	
	this->size = size;
	smp_buf = (T *)malloc(this->size * sizeof(T));
	
	currentLength = 0;
}


template <typename T>
void circularQueue<T>::insert(const T* result, const uint8_t vec_sz) {

	f_idx = r_idx;
	for (int i = 0; i < vec_sz; i++) {
		smp_buf[f_idx] = (T)result[i];
		f_idx++;
	}

	r_idx += vec_sz;
	currentLength++;

	if(f_idx >= size)
		circularQueue::clear();
}


template<typename T>
T* circularQueue<T>::getCircBuf() {
	return circularQueue::smp_buf;
}

#endif