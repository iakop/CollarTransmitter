#ifndef COLLARTRANSMITTER_H
#define COLLARTRANSMITTER_H

#include "Arduino.h"

#define PULSELENMICROS 256

typedef enum {
	ZAP_MODE = 0x1,
	RUMBLE_MODE = 0x2,
	BEEP_MODE = 0x4,
	BLINK_MODE = 0x8,
} collarTransmitterMode_t;

uint16_t reverseBits(uint16_t bits, uint8_t bitLen);

class CollarTransmitter {
private:
	uint8_t pin; //Pin to run from
	uint16_t id; //transmit id
	unsigned long long micros_timer;

	//Helpers
	void blockingSendDash();
	void blockingSendDot();
	void blockingTranslateSendBits(uint16_t bits, uint8_t bitLen);

	//Senders
	void blockingSendStartPattern();
	void blockingSendTarget(uint8_t target);
	void blockingSendMode(collarTransmitterMode_t mode);
	void blockingSendTransmitId(uint16_t transmit_id);
	void blockingSendPower(uint8_t power);
	void blockingSendInvMode(collarTransmitterMode_t mode);
	void blockingSendInvTarget(uint8_t target);
	void blockingSendEndPattern();

public:
	CollarTransmitter(){
		pin = 0xFF; //Uninitialized
	};
	CollarTransmitter(uint16_t transmitId /*known id: 0x6490*/){
		pin = 0xFF; //Uninitialized
		id = transmitId;
	};
	void attach(uint8_t attachPin){
		pin = attachPin;
		pinMode(pin, OUTPUT);
	};
	void setId(uint16_t transmitId){
		id = transmitId;
	};
	void blockingSend(uint8_t target, collarTransmitterMode_t mode, uint8_t power);

};

#endif /* COLLARTRANSMITTER_H */