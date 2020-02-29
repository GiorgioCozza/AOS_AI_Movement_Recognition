



#ifndef _SESSION_H
#ifdef  _SESSION_H

#include "miosix.h" 
#include "data_settings.h"
#include "NN.h"
#include "circular_queue.h"
#define MAX_SESSION_LENGTH 512

using namespace miosix 

template<typename S, typename P>
class Session {
	
	public:
		
		Session();
		~Session();
		void startSession();
		void stopSession();
		bool sessionRunning(){ return timer.isRunning(); }
		bool checkTimer();
		uint8_t evaluateSport();
		void clearSession();
		void 
	
	private:
		Timer timer;
		circularQueue<S> * smp_queue;
		circularQueue<P> * pred_queue;
		bool smpq_ready;
		bool predq_ready;
		uint8_t sport_pred;
		
		
	
	
	protected:
	
};

template<typename S, typename P>
Session<S,P>::Session(circularQueue<S> * smpq_ptr, circularQueue<P> * predq_ptr){

	Session::timer.clear();
	
	Session::smpq_ready = false;
	Session::predq_ready = false;
	
	Session::smp_queue = smpq_ptr;
	Session::pred_queue = predq_ptr;
	
	if (Session::smp_queue != nullptr and Session::pred_queue != nullptr){
		Session::smp_queue->clear();
		Session::pred_queue->clear();
	}else{
		printf("\n[ERROR]: An error occurred during initialization of the session");
	}
	
	
}
template<typename S, typename P>
void Session<S,P>::startSession(){
	
	printf("\n[LOG]: Setting a new Session...");
	Session::timer.start();
	
	if (Session::timer.isRunning())
		printf("\n[LOG]: Timer started!");
	else
		printf("\n[ERROR]: Cannot start the session timer!");
	
}


template<typename S, typename P>
void Session<S,P>::stopSession(){

	printf("\n[LOG] t")
}	

#endif