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
    int16_t errorLimit_;
    uint16_t setpoint_;

    int16_t lastError_;
    long output_minus_integral_;
    long output_;

    PID(){}
    void initialize(uint16_t output) {output_ = output<<PID_MV_PRECISION; lastError_ = 0; output_minus_integral_ = 0;}
    void setK(int16_t Kp, int16_t Ki, int16_t Kd, int16_t errorLimit) { Kp_ = Kp; Ki_= Ki; Kd_ = Kd; errorLimit_ = errorLimit;}
    void setSetpoint(uint16_t sp) {setpoint_ = sp;}
    int16_t subtract(uint16_t a, uint16_t b) {
        if(a > b) {
            if(a - b > errorLimit_) return errorLimit_;
        } else {
            if(b - a > errorLimit_) return -errorLimit_;
        }
        return a - b;
    }
    int16_t subtract2(int16_t a, int16_t b) {
        if(a > b) {
            if(a - b > errorLimit_) return errorLimit_;
        } else {
            if(b - a > errorLimit_) return -errorLimit_;
        }
        return a - b;
    }

    void calculateOutput(uint16_t measuredValue) {
        int16_t error = subtract(setpoint_, measuredValue);
        int16_t derivative = subtract2(error, lastError_);
        long v;
        {
            //calculate integral part
            output_ -= output_minus_integral_;

            v = Ki_; v *= error;
            output_ += v;
            //output_ contains only the integral part now.
        }

        {
            // add proportional part
            v = Kp_; v *= error;
            output_minus_integral_ = v;

            // add derivative part
            v = Kd_; v *= derivative;
            output_minus_integral_ += v;
        }

        output_ += output_minus_integral_;

        //store "last" error
        lastError_ = error;
    }
    uint16_t normalizeOutput(uint16_t maxOutput) {
        if(output_ < 0) {
            output_ = 0;
            output_minus_integral_ = 0;
            return 0;
        }
        if((output_ >> PID_MV_PRECISION) > maxOutput) {
//            return maxOutput;
            output_ = maxOutput;
            output_ <<= PID_MV_PRECISION;
            output_minus_integral_ = 0;
        }
        return output_ >> PID_MV_PRECISION;
    }
/*    uint16_t getOutput() {
        return output_ >> PID_MV_PRECISION;
    }*/
};

#endif //PID_H_
