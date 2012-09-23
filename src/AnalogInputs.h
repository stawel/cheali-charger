#ifndef ANALOGINPUTS_H_
#define ANALOGINPUTS_H_

#include <Arduino.h>

#define MAX_CALIBRATION_POINTS 2

#define ANALOG_INPUTS_RESOLUTION 		16  // bits
#define STD_ANALOG_INPUTS_RESOLUTION 	10  // bits
#define MAKE_ANALOG_INPUTS_RESOLUTION(val, bits) ((val) << (ANALOG_INPUTS_RESOLUTION - (bits)))

#define FOR_ALL_PHY_INPUTS(iterator) for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::PHYSICAL_INPUTS; iterator = AnalogInputs::Name(iterator + 1) )
#define FOR_ALL_INPUTS(iterator)     for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::ALL_INPUTS;      iterator = AnalogInputs::Name(iterator + 1) )

#define ANALOG_CELCIUS(x) ((AnalogInputs::ValueType)((x)*100))
#define ANALOG_VOLTS(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_AMPS(x) ((AnalogInputs::ValueType)((x)*1000))

class AnalogInputs {
public:
	typedef uint16_t ValueType;
	typedef uint16_t (*MeasureFunction)();

	struct CalibrationPoint {
		ValueType x;
		ValueType y;
	};
	struct DefaultValues {
		MeasureFunction f;
		CalibrationPoint p0;
		CalibrationPoint p1;
	};
	struct Calibration {
		CalibrationPoint p[MAX_CALIBRATION_POINTS];
	};

	enum Type {
		Current,
		Voltage,
		Temperature,
		Charge,
		Resistance,
		Unknown
	};
	enum Name {
		Vout,
		Ismps,
		Idischarge,

		VoutMux,
		Tintern,
		Vin,
		Textern,

		Vb0,
		Vb1,
		Vb2,
		Vb3,
		Vb4,
		Vb5,

		IsmpsValue,
		IdischargeValue,

		VirtualInputs,
		Vbalacer,
		VoutBalancer,
		VobInfo,
		VbalanceInfo,
		LastInput
	};
	static const uint8_t PHYSICAL_INPUTS = VirtualInputs - Vout;
	static const uint8_t ALL_INPUTS = LastInput - Vout;
	static const uint16_t AVR_MAX_COUNT = 100;
	static const ValueType STABLE_VALUE_ERROR = 2;
	static const uint16_t STABLE_MIN_VALUE = 3;


	AnalogInputs(const DefaultValues * inputs_P);

	ValueType getValue(Name name) const;
	ValueType getRealValue(Name name) const;
	ValueType getMeasuredValue(Name name) const;

	ValueType calibrateValue(Name name, ValueType x) const;
	ValueType reverseCalibrateValue(Name name, ValueType y) const;

	void measureValue(Name name);
	void doMeasurement(uint32_t count = PHYSICAL_INPUTS);
	void doFullMeasurement();
	void doVirtualCalculations();
	void doCalculations();
	void clearAvr();

	void restoreDefault();
	static void getCalibrationPoint(CalibrationPoint &p, Name name, uint8_t i);
	static void setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &p);

	int getConnectedBalancePorts() const;
	bool isConnected(Name name) const;

	template<uint8_t analogInput>
	static uint16_t analogValue() {
		// TODO: think it over
		::analogRead(analogInput);
		delayMicroseconds(30);
		return MAKE_ANALOG_INPUTS_RESOLUTION(::analogRead(analogInput), STD_ANALOG_INPUTS_RESOLUTION);
	}

	uint16_t getCalculationCount() { return calculationCount_; }
	void printRealValue(Name name, uint8_t dig) const;
	void printMeasuredValue(Name name, uint8_t dig) const;
	static Type getType(Name name);
	uint16_t getStableCount(Name name) const { return stableCount_[name]; };
	bool isStable(Name name) const { return stableCount_[name] >= STABLE_MIN_VALUE; };
	void resetStable();

protected:
	void setReal(Name name, ValueType real);
	const DefaultValues * inputsP_;
	uint16_t avrCount_;
	uint32_t avrSum_[PHYSICAL_INPUTS];
	Name currentInput_;
	ValueType x_[PHYSICAL_INPUTS];
	ValueType measured_[PHYSICAL_INPUTS];
	ValueType real_[ALL_INPUTS];
	uint16_t stableCount_[ALL_INPUTS];

	uint16_t calculationCount_;
};

inline AnalogInputs::ValueType absDiff(AnalogInputs::ValueType x, AnalogInputs::ValueType y)
{
	if(x > y) return x - y;
	return y - x;
}


#endif /* ANALOGINPUTS_H_ */
