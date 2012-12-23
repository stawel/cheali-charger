#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_MAX_VOLUME 100

class Buzzer {
public:
	enum SoundType { Keyboard, Info, Save, Select, StartProgram, ProgramComplete, Error, ReversedPolarity, Off };
	Buzzer():volume_(BUZZER_MAX_VOLUME), sound_(Off){}
	uint8_t volume_;
	uint16_t begin_time_;
	uint16_t last_value_;

	SoundType sound_;


	void soundKeyboard() 					{sound_ = Keyboard; 		begin(); }
	void soundInfo()						{if(sound_ != Info)			{sound_ = Info; 	 begin(); }}
	void soundSave()						{sound_ = Save; 			begin(); }
	void soundSelect()					{sound_ = Select; 			begin(); }
	void soundProgramComplete() 			{sound_ = ProgramComplete; 	begin(); }
	void soundStartProgram() 				{sound_ = StartProgram; 	begin(); }
	void soundReversedPolarity()			{if(sound_ != ReversedPolarity)		{sound_ = ReversedPolarity; 	begin(); }}
	void soundError() 						{if(sound_ != Error)		{sound_ = Error; 	begin(); }}
	void soundOff()						{sound_ = Off; }
	void begin();

	void doInterrupt();
private:
	uint16_t getKeyboard(uint16_t time);
	uint16_t getInfo(uint16_t time);
	uint16_t getSave(uint16_t time);
	uint16_t getSelect(uint16_t time);
	uint16_t getStartProgram(uint16_t time);
	uint16_t getProgramComplete(uint16_t time);
	uint16_t getReversedPolarity(uint16_t time);
	uint16_t getError(uint16_t time);
	uint16_t getOff(uint16_t time);

	uint16_t getSound(uint16_t time);
	uint16_t setOff();
};

extern Buzzer buzzer;

#endif /* BUZZER_H_ */
