#include <boards/Board.h>

#include <stdlib.h>

#include <util/delay.h>
#include <avr/pgmspace.h>

/* Define in the main.c which board you would like to compile to. In the future
   it is possible that the software detects the version itself. */
#if defined(SMARTSENSOR_BOARD_1_2)
    #pragma message "SmartSensor v1.2 has been selected to be compiled."
    #include "boards/v1_2/Board.h"
    SmartSensorBoardV1_2 board;
#else
    #pragma message "SMARTSENSOR_BOARD_X_X has not been defined, using the newest SmartBoard version v1.2."
    #include "boards/BoardV1_2.h"
    SmartSensorBoardV1_2 board;
#endif

SmartSensorBoard* SmartSensorBoard::getBoard() {
    return &board;
}

void SmartSensorBoard::addTask(ITask *task, const char* taskName) {
    this->debug_P(PSTR("Adding task: "));
    this->debug_P(taskName);

    if ( this->totalDrivers < SMARTSENSOR_MAX_TASKS) {
        uint8_t result = task->setup();

        if ( result == 0 ) {
            this->drivers[this->totalDrivers] = task;
            this->totalDrivers++;
            this->debug_P(PSTR(": Succesfully!\n"));
        
        } else {
            this->debugf_P(PSTR(": Could not be loaded: %d\n"), result);
        }
    
    } else {
        this->debugf_P(PSTR(": Maximum drivers reached, not added to the system.\n"), taskName);
    }
}

void SmartSensorBoard::setup() {
    this->loopTiming = this->millis();
}

void SmartSensorBoard::loop() {
    for ( uint8_t i=0; i < this->totalDrivers; ++i ) { // Loop through all the drivers
        this->drivers[i]->loop(this->millis());
    }

    this->loopTime = ( this->millis() - this->loopTiming );
    this->debugf_P(PSTR("Loop time %d ms\n"), this->loopTime); // Show the actual loop timing in the serial
    this->loopTiming = this->millis();
}

void SmartSensorBoard::rtcReadTimestampEvent(RTCTime& time) { // Of toch niet hier doen? Als de timestamp er is, dan in de loop de buffer leeg maken.
    if ( this->buffer.getSize() > 0 ) { // TODO: handle the buffer! => IDEA to handle this buffer when a new timestemp arrived (callback RTC)
        // When an overflow has been detected, what to do?
    }
    // Check the measurement buffer and sent the messages to the broker. (BELOW some test code)
    //this->debugf("Buffer size: %d (overflow:%d)\n", this->buffer.getSize(), this->buffer.getBufferOverflow());
    char m[MESSAGE_TOTAL_CHARS];
    if ( this->buffer.popMeasurement(m) ) {
        this->debugf("Popped: %s\n", m);
    }
}


// http://www.cplusplus.com/reference/cstdio/vsprintf/
void SmartSensorBoard::debugf( const char* message, ...) { // TODO: Maybe move these functions to Serial0 (think of it)?
    char buffer[50];
    va_list args;
    va_start(args, message);
    vsprintf (buffer, message, args);
    va_end (args);

    this->debug(buffer);
}

void SmartSensorBoard::debugf_P( const char* message, ...) {
    char buffer[50];
    va_list args;
    va_start(args, message);
    vsprintf_P(buffer, message, args);
    va_end (args);

    this->debug(buffer);
}
