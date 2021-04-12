#include "CollarTransmitter.h"

uint16_t reverseBits(uint16_t bits, uint8_t bitLen){
	uint16_t reverseBits = 0;
	for(uint8_t i = 0; i < bitLen; i++){
		if(bits & (1 << i)){
			reverseBits |= (1 << (3-i));
		}
	}
	return reverseBits;
}

void CollarTransmitter::blockingSendDash(){
	micros_timer = micros();
	digitalWrite(pin, LOW);
	while(micros() < micros_timer + PULSELENMICROS);
	micros_timer = micros();
	digitalWrite(pin, HIGH);
	while(micros() < micros_timer + PULSELENMICROS * 3);
}

void CollarTransmitter::blockingSendDot(){
	micros_timer = micros();
	digitalWrite(pin, LOW);
	while(micros() < micros_timer + PULSELENMICROS);
	micros_timer = micros();
	digitalWrite(pin, HIGH);
	while(micros() < micros_timer + PULSELENMICROS);
	micros_timer = micros();
	digitalWrite(pin, LOW);
	while(micros() < micros_timer + PULSELENMICROS * 2);
}

void CollarTransmitter::blockingTranslateSendBits(uint16_t bits, uint8_t bitLen){
	for(uint16_t i = bitLen; i > 0; i--){
		if((uint16_t)bits & (uint16_t)(1 << (i-1))){
			blockingSendDash();
		}
		else{
			blockingSendDot();
		}
	}
}

void CollarTransmitter::blockingSendStartPattern(){
	micros_timer = micros();
	digitalWrite(pin, HIGH);
	while(micros() < micros_timer + PULSELENMICROS * 6);
	micros_timer = micros();
	digitalWrite(pin, LOW);
	while(micros() < micros_timer + PULSELENMICROS * 3);
	micros_timer = micros();
	digitalWrite(pin, HIGH);
	while(micros() < micros_timer + PULSELENMICROS * 3);
}

void CollarTransmitter::blockingSendTarget(uint8_t target){
	switch(target){
		case 1:
		blockingTranslateSendBits(0b000, 3);
		break;
		case 2:
		blockingTranslateSendBits(0b111, 3);
		break;
		default:
		blockingTranslateSendBits(0b000, 3);
		break;
	}
}

void CollarTransmitter::blockingSendMode(collarTransmitterMode_t mode){
	//Serial.println("translating mode");
	blockingTranslateSendBits(mode, 4);
}

void CollarTransmitter::blockingSendTransmitId(uint16_t transmit_id){
	blockingTranslateSendBits(transmit_id, 16);
}

void CollarTransmitter::blockingSendPower(uint8_t power){
	blockingTranslateSendBits(power, 8);
}

void CollarTransmitter::blockingSendInvMode(collarTransmitterMode_t mode){
	blockingTranslateSendBits(reverseBits(~mode, 4), 4);
}

void CollarTransmitter::blockingSendInvTarget(uint8_t target){
	switch(target){
		case 1:
		blockingTranslateSendBits(0b111, 3);
		break;
		case 2:
		blockingTranslateSendBits(0b000, 3);
		break;
	}
}

void CollarTransmitter::blockingSendEndPattern(){
	blockingSendDot();
	blockingSendDot();
}

void CollarTransmitter::blockingSend(uint8_t target, collarTransmitterMode_t mode, uint8_t power){
	blockingSendStartPattern();
	blockingSendTarget(target);
	blockingSendMode(mode);
	blockingSendTransmitId(id);
	blockingSendPower(power);
	blockingSendInvMode(mode);
	blockingSendInvTarget(target);
	blockingSendEndPattern();
}