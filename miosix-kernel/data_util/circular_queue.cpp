#include "circular_queue.h"

circularQueue::circularQueue() {
	f_idx = 0;
	r_idx = 0;
	smp_buf = (float*)malloc(VECTOR_SIZE * WINDOW_SIZE * sizeof(float));

	//data = new uint32_t[WINDOW_SIZE - 1][12];
	/*
	smp_buf = (float * *)malloc(WINDOW_SIZE * sizeof(float*));
	for (int i = 0; i < WINDOW_SIZE; i++)
		smp_buf[i] = (float*)malloc(VECTOR_SIZE * sizeof(float));
		*/
	currentLength = 0;
}


void circularQueue::insert(int32_t* result) {

	f_idx = r_idx;
	for (int i = 0; i < VECTOR_SIZE; i++) {
		smp_buf[f_idx] = (float)result[i];
		f_idx++;
	}

	r_idx += VECTOR_SIZE;
	currentLength++;

	if (f_idx >= ((WINDOW_SIZE * VECTOR_SIZE) - 1))
		circularQueue::clear();
}

/*
bool circularQueue::checkReady() {
	if (currentLength < WINDOW_SIZE)
		return 0;
	else
		return 1;
}
*/
float* circularQueue::getVectSmp(uint16_t pos) {
	return (circularQueue::smp_buf + (pos - 1) * VECTOR_SIZE);
}

float* circularQueue::getCircBuf() {
	return circularQueue::smp_buf;
}
/*
void circularQueue::printing() {
	int i, j, flag = 0;
	printf("\n--------------seg1--------------\n");
	for (i = 0; i < WINDOW_SIZE; i++) {
		for (j = 0; j < VECTOR_SIZE; j++) {
			if (i + f_idx > WINDOW_SIZE - 1)
				flag = i + f_idx - WINDOW_SIZE;
			else {
				flag = i + f_idx;
				printf("%6ld ", smp_buf[flag][j]);
				if (j == 11)
					printf("\n");
			}
		}
	}
	printf("--------------seg2--------------\n");
}
*/