/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdio.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

/*
 * Pin definitions for ATtiny44, ATtiny84, ATtiny45, ATtiny85 and Arduino.
 */
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define SPI_PORT   PORTA
  #define SPI_DDR     DDRA
  #define SPI_PIN        7
  #define USCK_PIN       4
  #define DO_PIN         5
  #define DI_PIN         6
  #define __ATTINY
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define SPI_PORT   PORTB
  #define SPI_DDR     DDRB
  #define SPI_PIN        4
  #define USCK_PIN       2
  #define DO_PIN         0
  #define DI_PIN         1
  #define __ATTINY
#else
  #define SPI_PIN       SS
  #define USCK_PIN     SCK
  #define DO_PIN      MOSI
  #define DI_PIN      MISO
#endif


#define SPI_CLOCK_DIV4   0x00
#define SPI_CLOCK_DIV16  0x01
#define SPI_CLOCK_DIV64  0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2   0x04
#define SPI_CLOCK_DIV8   0x05
#define SPI_CLOCK_DIV32  0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK    0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK   0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

class SPIClass {
public:
  inline static byte transfer(byte _data);
  
  // SPI Configuration methods

  #ifndef __ATTINY							// Defined only if not ATtiny 
    inline static void attachInterrupt();
    inline static void detachInterrupt(); 	// Default
  #endif
    static void setBitOrder(uint8_t);

  static void begin(); // Default
  static void end();
    
  static void setDataMode(uint8_t);
  static void setClockDivider(uint8_t);
};

extern SPIClass SPI;

uint8_t SPIClass::transfer(uint8_t _data) {

  #ifdef __ATTINY
    USIDR = _data;
    USISR = _BV(USIOIF);
  
    while((USISR & _BV(USIOIF)) == 0) {
      USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    }
    return USIDR;
  #else
    SPDR = _data;
    while (!(SPSR & _BV(SPIF)))
      ;
    return SPDR;
  #endif
}

#ifndef __ATTINY
  void SPIClass::attachInterrupt() {
    SPCR |= _BV(SPIE);
  }

  void SPIClass::detachInterrupt() {
    SPCR &= ~_BV(SPIE);
  }
#endif

#endif
