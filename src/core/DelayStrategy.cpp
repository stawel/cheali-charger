#include "DelayStrategy.h"
#include "Timer.h"
#include "Strategy.h"
#include "Buzzer.h"

namespace DelayStrategy {

    bool state = false;

    int16_t start_time_;
    int16_t delay_;
    
    
     
    
     const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy,
    };
  
  
    
    
    



    void powerOn() 
    {
        start_time_ = Timer::getMiliseconds()/1000;
    }

    Strategy::statusType doStrategy() 
    {
	    if((Timer::getMiliseconds()/1000) <= start_time_ + delay_) 
	    {
	       state = true;
	       return Strategy::RUNNING;
	    }
	    else 
	    {
	       state = false;
	       return Strategy::COMPLETE;
      }  
    }

    void powerOff() 
    {
      
    }
     
     bool isDelay() 
    {
      return(state);
    }
    
    void setDelay(int16_t minutes) 
    {
         delay_ = minutes*60;
         powerOn();
    }

}