/*
 * utils.h
 *
 *  Created on: Dec 18, 2024
 *      Author: Oded Yosef
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "i2c-lcd.h"
#include "stm32f1xx_hal.h"

char* itoa(int i, char b[]);

void showDateAndTime();

void feedMilka(int Delay_ms);

void checkButtonsPressed();

void handleLCDMonitor();

void handleIdleLCDMonitor();

void handleFeedLCDMonitor();

void handleMainManuLCDMonitor();

void handleFeedHourLCDMonitor();

void handleFeedMinuteLCDMonitor();

void handleFeedSecondLCDMonitor();

void handleTimeHourLCDMonitor();

void handleTimeMinuteLCDMonitor();

void handleTimeSecondLCDMonitor();

void handleFeedPeriodLCDMonitor();

void handleYearLCDMonitor();

void handleMonthLCDMonitor();

void handleDayLCDMonitor();

void showSelectedTime();

void showSelectedFeedTime();

void showSelectedDate();

void checkIfIsTimeToFeed();

void startFeeding();

void stopFeeding();

enum BUTTONS {
	SET_BUTTON = 0,
	UP_BUTTON,
	DOWN_BUTTON,
	ENTER_BUTTON
};

#endif /* INC_UTILS_H_ */
