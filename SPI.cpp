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

  // Set SS to high so a connected chip will be "deselected" by default

  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).

  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // By doing this AFTER enabling SPI, we avoid accidentally
  // clocking in a single bit since the lines go directly
  // from "input" to SPI control.
  // http://code.google.com/p/arduino/issues/detail?id=888
  #ifdef __ATTINY
    SPI_PORT |= _BV(SPI_PIN);                                   //digitalWrite(SPI_PIN, HIGH);
    SPI_DDR  |= _BV(SPI_PIN) | _BV(USCK_PIN) | _BV(DO_PIN);     //pinMode(USCK_PIN, SPI_PIN, DO_PIN, OUTPUT)
    SPI_DDR  &= ~_BV(DI_PIN);                                   //pinMode(DI_PIN, INPUT)
  #else
    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);

    digitalWrite(SPI_PIN, HIGH);
    pinMode(SPI_PIN, OUTPUT);

    pinMode(USCK_PIN, OUTPUT);
    pinMode(DI_PIN, INPUT);
    pinMode(DO_PIN, OUTPUT);
  #endif
}


void SPIClass::end() {
  #ifdef __ATTINY
    SPI_DDR &= ~(_BV(SPI_PIN) | _BV(USCK_PIN) | _BV(DO_PIN));   //pinMode(USCK_PIN, SPI_PIN, DO_PIN, INPUT)
  #else
    SPCR &= ~_BV(SPE);
  #endif
}


void SPIClass::setBitOrder(uint8_t bitOrder)
{
  #ifndef __ATTINY
    if(bitOrder == LSBFIRST) {
      SPCR |= _BV(DORD);
    } else {
      SPCR &= ~(_BV(DORD));
    }
  #else
    //TODO
  #endif
}

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
