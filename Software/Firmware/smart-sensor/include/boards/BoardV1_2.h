#pragma once
/**
 * @file       : boards/BoardV1_2.h
 * @description: This file contains the specific board v1.2.
 * @date       : 11-09-2021
 * @author     : Maurice Snoeren (MS)
 * @version    : 0.1
 * @todo       : -
 * @updates
 * 
 */
#define UART0_BAUD 9600
#define UART0_UBRR FOSC/16/BAUD-1

#define BOARDV1_2_ADAPTER_IN_USE_PIN PA6
#define BOARDV1_2_ADAPTER_IN_USE_DDR DDRA
#define BOARDV1_2_ADAPTER_IN_USE_PORT PORTA

#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <boards/Board.h>
#include <boards/SmartSensorMeasurement.h>
#include <util/MeasurementBuffer.h>
#include <drivers/LedDriver.h>
#include <drivers/SHTC3Driver.h>
#include <drivers/MCP7940NDriver.h>

class SmartSensorBoardV1_2: public SmartSensorBoard {
private:
    MeasurementBuffer buffer;

    LedDriver ledDriver;
    SHTC3Driver shtc3Driver;
    MCP7940NDriver mcp7940nDriver;

    // Should be implemented by the counter. At this moment, we do a delay_ms(1) in the main loop and count one at each time the loop starts.
    uint32_t _millis;
        
public:
    SmartSensorBoardV1_2(): _millis(0) {}
    SmartSensorBoardV1_2(SmartSensorMeasurement* cbMeasurement): shtc3Driver(cbMeasurement), _millis(0) {}

    void setup();

    // Can be removed when millis is correctly implemented!
    void loop();

    uint32_t millis();
    bool adapterInUse();

    void debug( const char* message);

    void addMeasurement(const char* measurement, ...);
};

