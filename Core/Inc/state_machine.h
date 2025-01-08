/*
 * state_machine.h
 *
 *  Created on: Dec 24, 2024
 *      Author: User
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_
#include <stdint.h>
#include "i2c-lcd.h"
#include "DS1302.h"

typedef enum {
	IDLE_STATE = 0,
	FEED_STATE,
	MAIN_MANU_STATE,
	SET_HOUR_FEED_STATE,
	SET_MINUTE_FEED_STATE,
	SET_SECOND_FEED_STATE,
	SET_HOUR_TIME_STATE,
	SET_MINUTE_TIME_STATE,
	SET_SECOND_TIME_STATE,
	SET_YEAR_STATE,
	SET_MONTH_STATE,
	SET_DAY_STATE,
	SET_FEED_PERIOD_STATE,

	MAX_STATE
} AppState;

typedef enum {
	EVENT_NONE = 0,
	SET_BUTTON_PRESSED_EVENT,
	UP_BUTTON_PRESSED_EVENT,
	DOWN_BUTTON_PRESSED_EVENT,
	ENTER_BUTTON_PRESSED_EVENT,
	FEED_TIME_EVENT,
	END_FEED_TIME_EVENT,

	EVENT_MAX
} AppEvent;

void handle_event(AppEvent event);
void setTimeDate();
void setTimeDateFromExtRTC();

#endif /* INC_STATE_MACHINE_H_ */
