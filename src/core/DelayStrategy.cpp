#include "DelayStrategy.h"
#include "Timer.h"
#include "Strategy.h"
#include "Buzzer.h"

namespace DelayStrategy 
{
    bool state_ = false;

    int16_t start_time_;
    int16_t delay_;
    
    

    
     const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy,
    };
  

}// namespace DelayStrategy

void DelayStrategy::powerOn() 
{
    start_time_ = Timer::getMiliseconds()/1000;
}

void DelayStrategy::powerOff() 
{

}

Strategy::statusType DelayStrategy::doStrategy() 
{
  Strategy::statusType status;
  
  if((Timer::getMiliseconds()/1000) <= start_time_ + delay_) 
  {
     state_ = true;
     return Strategy::RUNNING;
  }
  else 
  {
    state_ = false;
    return Strategy::COMPLETE;
  }  
}
 
 
 
 
bool DelayStrategy::isDelay() 
{
   return(state_);
}

void DelayStrategy::setDelay(int16_t minutes) 
{
   delay_ = minutes*60;
   DelayStrategy::powerOn();
}

