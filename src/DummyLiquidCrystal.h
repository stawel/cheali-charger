#ifdef LiquidCrystal_h
#define LiquidCrystal_h

#include <inttypes.h>

class LiquidCrystal {
public:
	LiquidCrystal(int rs, int enable,
			int d0, int d1, int d2, int d3) {};
	void begin(int,int) {};
	void clear(){};
	void setCursor(int x, int y){};
	void print(const char *){};
	void print(int){};
	void print(unsigned int){};
	void print(long int, int x = 1){};
	void print(unsigned long int, int x = 1){};
	void print(double, int x = 1){};

};

#define DummyLiquidCrystal_h


#endif

