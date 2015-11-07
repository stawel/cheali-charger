#ifndef PID_H_
#define PID_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>

//MV - manipulated variable in PID
#define MAX_PID_MV_FACTOR 1.5
#define MAX_PID_MV ((uint16_t) (OUTPUT_PWM_PRECISION_PERIOD * MAX_PID_MV_FACTOR))
#define PID_MV_PRECISION 8

#define PID_KVALUE(x) ((int16_t)((x)*(1<<PID_MV_PRECISION)))

struct PID
{
    int16_t Kp_, Ki_, Kd_;
    uint16_t setpoint_;

    int16_t lastError_;
    int16_t lastDerivative_;
    long output_;

    PID(){}
    void initialize(uint16_t output) {output_ = output<<PID_MV_PRECISION; lastError_ = lastDerivative_ = 0;}
    void setK(int16_t Kp, int16_t Ki, int16_t Kd) { Kp_ = Kp; Ki_= Ki; Kd_ = Kd;}
    void setSetpoint(uint16_t sp) {setpoint_ = sp;}
    int16_t subtract(uint16_t a, uint16_t b) {
        if (a - b > INT16_MAX) return INT16_MAX;
        return a - b;
    }

    void calculateOutput(uint16_t measuredValue) {
        int16_t error = subtract(setpoint_, measuredValue);

        //we do some magic here because we don't want to store the integral part
        //this is necessary because we "normalize" the PID's output

        long v;
        {
            //restore integral part
            v = Kp_; v *= lastError_;
            output_ -= v;
            v = Kd_; v *= lastDerivative_;
            output_ -= v;
        }

        //now output_ contains only the last time calculated integral part (Ki_ * integral_)
        {
            //add to the integral part the current error
            v = error; v*= Ki_;
            output_ += v;
        }


        int16_t derivative = subtract(error, lastError_);
        {
            //calculate output

            // add proportional part
            v = Kp_; v *= error;
            output_ += v;

            // add derivative part
            v = Kd_; v *= derivative;
            output_ += v;
        }

        //store "last" values
        lastError_ = error;
        lastDerivative_ = derivative;
    }
    void normalizeOutput(uint16_t maxOutput) {
        if(output_ < 0) output_ = 0;
        if((output_ >> PID_MV_PRECISION) > maxOutput) {
            output_ = maxOutput;
            output_ <<= PID_MV_PRECISION;
        }
    }
    uint16_t getOutput() {
        return output_ >> PID_MV_PRECISION;
    }
};

#endif //PID_H_
