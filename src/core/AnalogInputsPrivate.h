/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ANALOGINPUTSPRIVATE_H_
#define ANALOGINPUTSPRIVATE_H_

#include "AnalogInputs.h"
#include "memory.h"


namespace AnalogInputs {

    extern PROGMEM const struct DefaultValues inputsP_[];//AnalogInputs::PHYSICAL_INPUTS];
    extern DATAMEM ValueType real_[ANALOG_INPUTS_ALL_INPUTS];
    extern DATAMEM ValueType avrAdc_[ANALOG_INPUTS_PHYSICAL_INPUTS];
    extern volatile DATAMEM ValueType i_adc_[ANALOG_INPUTS_PHYSICAL_INPUTS];
    extern volatile DATAMEM uint16_t  i_avrCount_;
    extern volatile DATAMEM uint32_t  i_avrSum_[ANALOG_INPUTS_PHYSICAL_INPUTS];

    extern volatile bool on_;
    extern volatile bool onTintern_;

    void intterruptFinalizeMeasurement();
    void resetStable();

    void doIdle();
    void doSlowInterrupt();

    //calibration
    void getCalibrationPoint(struct CalibrationPoint *p, enum Name name, uint8_t i);
    void setCalibrationPoint(enum Name name, uint8_t i, const struct CalibrationPoint *p);


}


#endif /* ANALOGINPUTSPRIVATE_H_ */
