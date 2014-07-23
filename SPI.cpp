/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "pins_arduino.h"
#include "SPI.h"


SPIClass SPI;

void SPIClass::begin() {

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI
  // automatically switches to Slave, so the data direction of
  // the SS pin MUST be kept as OUTPUT.
  #ifndef __ATTINY
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
  #endif
    
  // Set SS to high so a connected chip will be "deselected" by default
  digitalWrite(SPI_DDR_PORT, HIGH);

  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).
  pinMode(SPI_DDR_PORT, OUTPUT);

  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // By doing this AFTER enabling SPI, we avoid accidentally
  // clocking in a single bit since the lines go directly
  // from "input" to SPI control.  
  // http://code.google.com/p/arduino/issues/detail?id=888
  pinMode(USCK_DD_PIN, OUTPUT);
  pinMode(DO_DD_PIN, OUTPUT);
  pinMode(DI_DD_PIN, INPUT);
}


void SPIClass::end() {
  #ifndef __ATTINY
    SPCR &= ~_BV(SPE);
  #else
    pinMode(USCK_DD_PIN, INPUT);
    pinMode(DO_DD_PIN, INPUT);
    pinMode(DI_DD_PIN,INPUT);
  #endif
}

#ifndef __ATTINY
  void SPIClass::setBitOrder(uint8_t bitOrder)
  {
    if(bitOrder == LSBFIRST) {
      SPCR |= _BV(DORD);
    } else {
      SPCR &= ~(_BV(DORD));
    }
  }
#endif

  void SPIClass::setDataMode(uint8_t mode)
  {
    #ifndef __ATTINY
      SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
    #endif
  }

  void SPIClass::setClockDivider(uint8_t rate)
  {
    #ifndef __ATTINY
      SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
      SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
    #endif
  }