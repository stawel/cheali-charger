#ifndef POLARITYCHECK_H_
#define POLARITYCHECK_H_

#include <inttypes.h>

class PolarityCheck {
public:
	static const uint8_t REVERSED_POLARITY = 127;

	static bool isReversedPolarity_;
	static bool runReversedPolarityInfo();
};

#endif /* POLARITYCHECK_H_ */
