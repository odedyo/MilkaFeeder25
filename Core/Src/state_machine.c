/*
 * state_machine.c
 *
 *  Created on: Dec 24, 2024
 *      Author: Oded Yosef
 */
#include "state_machine.h"

extern AppState currentState;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;

int8_t selectedHour  	  = 12;
int8_t selectedMinute     = 30;
int8_t selectedSecond     = 15;
int8_t selectedYear  	  = 24;
int8_t selectedMonth      = 10;
int8_t selectedDay   	  = 11;
int8_t selectedFeedHour   = 15;
int8_t selectedFeedMinute = 31;
int8_t selectedFeedSecond = 10;
int16_t feedDuration 	  = 2000;

/***********************************************/
void handle_event(AppEvent event) {
/***********************************************/
	switch (currentState) {

		case IDLE_STATE:

			switch (event) {
				case SET_BUTTON_PRESSED_EVENT:
					currentState = MAIN_MANU_STATE;
					break;

				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = FEED_STATE;
					startFeeding();
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				default:

					break;
			}

			break;

		case FEED_STATE:

			switch (event) {
				case END_FEED_TIME_EVENT:
					currentState = IDLE_STATE;
					break;

				default:

					break;
			}
			break;

		case MAIN_MANU_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_FEED_PERIOD_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					currentState = SET_HOUR_FEED_STATE;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					currentState = SET_HOUR_TIME_STATE;

					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;

					break;

				default:

					break;
			}

			break;

		case SET_HOUR_FEED_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_MINUTE_FEED_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedFeedHour++;
					if(selectedFeedHour == 24) selectedFeedHour = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedFeedHour--;
					if(selectedFeedHour < 0) selectedFeedHour = 23;
					break;

				default:

					break;
			}

			break;

		case SET_MINUTE_FEED_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_SECOND_FEED_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedFeedMinute++;
					if(selectedFeedMinute == 60) selectedFeedMinute = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedFeedMinute--;
					if(selectedFeedMinute < 0) selectedFeedMinute = 59;
					break;

				default:

					break;
			}

			break;

		case SET_SECOND_FEED_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = IDLE_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedFeedSecond++;
					if(selectedFeedSecond == 60) selectedFeedSecond = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedFeedSecond--;
					if(selectedFeedSecond < 0) selectedFeedSecond = 59;
					break;

				default:

					break;
			}

			break;

		case SET_HOUR_TIME_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_MINUTE_TIME_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedHour++;
					if(selectedHour == 24) selectedHour = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedHour--;
					if(selectedHour < 0) selectedHour = 23;
					break;

				default:

					break;
			}

			break;

		case SET_MINUTE_TIME_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_SECOND_TIME_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedMinute++;
					if(selectedMinute == 60) selectedMinute = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedMinute--;
					if(selectedMinute < 0) selectedMinute = 59;
					break;

				default:

					break;
			}

			break;

		case SET_SECOND_TIME_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_DAY_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedSecond++;
					if(selectedSecond == 60) selectedSecond = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedSecond--;
					if(selectedSecond < 0) selectedSecond = 59;
					break;

				default:

					break;
			}

			break;

		case SET_DAY_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_MONTH_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedDay++;
					if(selectedDay == 32) selectedDay = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedDay--;
					if(selectedDay < 0) selectedDay = 31;
					break;

				default:

					break;
			}

			break;

		case SET_MONTH_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = SET_YEAR_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedMonth++;
					if(selectedMonth == 13) selectedMonth = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedMonth--;
					if(selectedMonth < 0) selectedMonth = 12;
					break;

				default:

					break;
				}

			break;

		case SET_YEAR_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					currentState = IDLE_STATE;
					setTimeDate();
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					selectedYear++;
					if(selectedYear == 100) selectedYear = 0;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					selectedYear--;
					if(selectedYear < 0) selectedYear = 99;
					break;

				default:

					break;
				}

			break;

		case SET_FEED_PERIOD_STATE:

			switch (event) {
				case ENTER_BUTTON_PRESSED_EVENT:
					setTimerInterval(feedDuration);
					currentState = IDLE_STATE;
					break;

				case FEED_TIME_EVENT:
					currentState = FEED_STATE;
					break;

				case UP_BUTTON_PRESSED_EVENT:
					feedDuration+=100;
					if(feedDuration == 10000) feedDuration = 1000;
					break;

				case DOWN_BUTTON_PRESSED_EVENT:
					feedDuration-=100;
					if(feedDuration < 1000) feedDuration = 9999;
					break;

				default:

					break;
			}

			break;

		default:

			break;
	}
}

/***********************************************/
void setTimeDate() {
/***********************************************/
	static RTC_TimeTypeDef setTime = {0};
	static RTC_DateTypeDef setDate = {0};

	setTime.Hours = selectedHour;
	setTime.Minutes = selectedMinute;
	setTime.Seconds = selectedSecond;
	setDate.Year = selectedYear;
	setDate.Month = selectedMonth;
	setDate.Date = selectedDay;

	if (HAL_RTC_SetTime(&hrtc, &setTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_RTC_SetDate(&hrtc, &setDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}
