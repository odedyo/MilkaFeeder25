/*
 * utils.c
 *
 *  Created on: Dec 18, 2024
 *      Author: Oded Yosef
 */
#include "utils.h"
#include "main.h"
#include <stdio.h>
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
extern volatile uint8_t buttonStableState[4];
extern AppState currentState;
extern AppState lastState;
extern int8_t selectedHour;
extern int8_t selectedMinute;
extern int8_t selectedSecond;
extern int8_t selectedYear;
extern int8_t selectedMonth;
extern int8_t selectedDay;
extern int8_t selectedFeedHour;
extern int8_t selectedFeedMinute;
extern int8_t selectedFeedSecond;
extern int16_t feedDuration;

/***********************************************/
int __io_putchar(int ch)
/***********************************************/

{
    // Send your character to the UART
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/***********************************************/
char* itoa(int i, char b[]) {
/***********************************************/

	char const digit[] = "0123456789";
	char *p = b;
	if (i < 0) {
		*p++ = '-';
		i *= -1;
	}
	int shifter = i;
	do { //Move to where representation ends
		++p;
		shifter = shifter / 10;
	} while (shifter);
	*p = '\0';
	do { //Move back, inserting digits as u go
		*--p = digit[i % 10];
		i = i / 10;
	} while (i);
	return b;
}

/***********************************************/
void showDateAndTime() {
/***********************************************/

	//  --------RTC CLOCK------  //
	RTC_TimeTypeDef TempsTime = { 0 };
	int8_t Thours;
	int8_t Tmin;
	int8_t Tsec;
	char tempHours[10];
	char tempMin[10];
	char tempSec[10];
	//  --------RTC DATE-------  //
	RTC_DateTypeDef TempDateToUpdate = { 0 };
	int8_t Tyear;
	int8_t Tmonth;
	int8_t Tdate;
	char tempYear[10];
	char tempMonth[10];
	char tempDate[10];


	HAL_RTC_GetTime(&hrtc, &TempsTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &TempDateToUpdate, RTC_FORMAT_BIN);

	// ---- Time ---- //
	// Hours
	// -------------- //
	Thours = TempsTime.Hours;
	itoa(Thours, tempHours);
	if (Thours < 10) {
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string("0");
		lcd_send_cmd(0x80 | 0x01);
		lcd_send_string(tempHours);
	} else {
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(tempHours);
	}
	lcd_send_cmd(0x80 | 0x02);
	lcd_send_string(":");
	// Minute
	// -------------- //
	Tmin = TempsTime.Minutes;
	itoa(Tmin, tempMin);
	if (Tmin < 10) {
		lcd_send_cmd(0x80 | 0x03);
		lcd_send_string("0");
		lcd_send_cmd(0x80 | 0x04);
		lcd_send_string(tempMin);
	} else {
		lcd_send_cmd(0x80 | 0x03);
		lcd_send_string(tempMin);
	}
	lcd_send_cmd(0x80 | 0x05);
	lcd_send_string(":");
	// Second
	// -------------- //
	Tsec = TempsTime.Seconds;
	itoa(Tsec, tempSec);
	if (Tsec < 10) {
		lcd_send_cmd(0x80 | 0x06);
		lcd_send_string("0");
		lcd_send_cmd(0x80 | 0x07);
		lcd_send_string(tempSec);
	} else {
		lcd_send_cmd(0x80 | 0x06);
		lcd_send_string(tempSec);
	}
	// -------------- //
	// ---- date ---- //
	// -------------- //
	Tdate = TempDateToUpdate.Date;
	itoa(Tdate, tempDate);
	if (Tdate < 10) {
		lcd_send_cmd(0x80 | 0x0C);
		lcd_send_string("0");
		lcd_send_cmd(0x80 | 0x0D);
		lcd_send_string(tempDate);
	} else {
		lcd_send_cmd(0x80 | 0x0C);
		lcd_send_string(tempDate);
	}
	lcd_send_cmd(0x80 | 0x0E);
	lcd_send_string("/");
	// month
	// -------------- //
	Tmonth = TempDateToUpdate.Month;
	itoa(Tmonth, tempMonth);
	if (Tmonth < 10) {
		lcd_send_cmd(0x80 | 0x0F);
		lcd_send_string("0");
		lcd_send_cmd(0x80 | 0x10);
		lcd_send_string(tempMonth);
	} else {
		lcd_send_cmd(0x80 | 0x0F);
		lcd_send_string(tempMonth);
	}
	lcd_send_cmd(0x80 | 0x11);
	lcd_send_string("/");
	// year
	// -------------- //
	Tyear = TempDateToUpdate.Year;
	itoa(Tyear, tempYear);
	lcd_send_cmd(0x80 | 0x12);
	lcd_send_string(tempYear);
}

/***********************************************/
void feedMilka(int Delay_ms) {
/***********************************************/
	HAL_GPIO_WritePin(MOTOR_Enable_GPIO_Port, MOTOR_Enable_Pin, GPIO_PIN_SET);
	HAL_Delay(Delay_ms);
	HAL_GPIO_WritePin(MOTOR_Enable_GPIO_Port, MOTOR_Enable_Pin, GPIO_PIN_RESET);
}

/***********************************************/
void checkButtonsPressed() {
/***********************************************/
	if(buttonStableState[SET_BUTTON]) {
		handle_event(SET_BUTTON_PRESSED_EVENT);
	}

	if(buttonStableState[UP_BUTTON]) {
		handle_event(UP_BUTTON_PRESSED_EVENT);
	}

	if(buttonStableState[DOWN_BUTTON]) {
		handle_event(DOWN_BUTTON_PRESSED_EVENT);
	}

	if(buttonStableState[ENTER_BUTTON]) {
		handle_event(ENTER_BUTTON_PRESSED_EVENT);
	}
}

/***********************************************/
void handleLCDMonitor() {
/***********************************************/
	if(lastState != currentState) {
		lcd_clear();
	}
	lastState = currentState;

	showDateAndTime();
	switch(currentState) {

		case IDLE_STATE:
			handleIdleLCDMonitor();
			break;

		case FEED_STATE:
			handleFeedLCDMonitor();
			break;

		case MAIN_MANU_STATE:
			handleMainManuLCDMonitor();
			break;

		case SET_HOUR_FEED_STATE:
			handleFeedHourLCDMonitor();
			break;

		case SET_MINUTE_FEED_STATE:
			handleFeedMinuteLCDMonitor();
			break;

		case SET_SECOND_FEED_STATE:
			handleFeedSecondLCDMonitor();
			break;

		case SET_HOUR_TIME_STATE:
			handleTimeHourLCDMonitor();
			break;

		case SET_MINUTE_TIME_STATE:
			handleTimeMinuteLCDMonitor();
			break;

		case SET_SECOND_TIME_STATE:
			handleTimeSecondLCDMonitor();
			break;

		case SET_DAY_STATE:
			handleDayLCDMonitor();
			break;

		case SET_MONTH_STATE:
			handleMonthLCDMonitor();
			break;

		case SET_YEAR_STATE:
			handleYearLCDMonitor();
			break;

		case SET_FEED_PERIOD_STATE:
			handleFeedPeriodLCDMonitor();
			break;

		default:

			break;
	}
}

/***********************************************/
void handleIdleLCDMonitor()
/***********************************************/
{
	static char temp[20] = "\0";
	lcd_send_cmd(0x80|0x41);
	lcd_send_string("Milka & Tinkerbell");
	lcd_send_cmd(0x80|0x1B);
	lcd_send_string("Feeder");

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Next Feed:");

	itoa(selectedFeedHour, temp);
	if(selectedFeedHour<10){
		lcd_send_cmd(0x80|0x5F);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x60);
		lcd_send_string(temp);
	}
	else{
		lcd_send_cmd(0x80|0x5F);
		lcd_send_string(temp);
	}
	lcd_send_cmd(0x80|0x61);
	lcd_send_string(":");

	itoa(selectedFeedMinute, temp);
	if(selectedFeedMinute<10){
		lcd_send_cmd(0x80|0x62);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x63);
		lcd_send_string(temp);
	}
	else{
		lcd_send_cmd(0x80|0x62);
		lcd_send_string(temp);
	}
	lcd_send_cmd(0x80|0x64);
	lcd_send_string(":");

	itoa(selectedFeedSecond, temp);
	if(selectedFeedSecond<10){
		lcd_send_cmd(0x80|0x65);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x66);
		lcd_send_string(temp);
	}
	else{
		lcd_send_cmd(0x80|0x65);
		lcd_send_string(temp);
	}
}

/***********************************************/
void handleFeedLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string("*   Feeding Now    *");
	lcd_send_cmd(0x80 | 0x14);
	lcd_send_string("*    (=^'.'^=)     *");
	lcd_send_cmd(0x80 | 0x54);
	lcd_send_string("********************");
}

/***********************************************/
void handleMainManuLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Down - Set Time");
	lcd_send_cmd(0x80|0x14);
	lcd_send_string("Up   - Set Feed Time");
	lcd_send_cmd(0x80|0x54);
	lcd_send_string("ENT  - Feed Duration");
}

/***********************************************/
void handleFeedHourLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Feed Hour");

	showSelectedFeedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleFeedMinuteLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Feed Minute");

	showSelectedFeedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleFeedSecondLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Feed Second");

	showSelectedFeedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleTimeHourLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Hour");

	showSelectedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleTimeMinuteLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Minute");

	showSelectedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleTimeSecondLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Second");

	showSelectedTime();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleFeedPeriodLCDMonitor()
/***********************************************/
{
	char tempc [10];

	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Feed Duration");

	itoa(feedDuration/1000, tempc);

	lcd_send_cmd(0x80|0x19);
	lcd_send_string(tempc);

	lcd_send_cmd(0x80|0x1A);
	lcd_send_string(".");

	itoa((feedDuration%1000) / 100, tempc);

	lcd_send_cmd(0x80|0x1B);
	lcd_send_string(tempc);

	lcd_send_cmd(0x80|0x1D);
	lcd_send_string("[ms]");

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleYearLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Year");

	showSelectedDate();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleMonthLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Month");

	showSelectedDate();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void handleDayLCDMonitor()
/***********************************************/
{
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Choose Day");

	showSelectedDate();

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("Use Up/Down Buttons");
}

/***********************************************/
void showSelectedTime() {
/***********************************************/
	char tempc [10];
	itoa(selectedHour, tempc);

	if(selectedHour < 10) {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1B);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1C);
	lcd_send_string(":");

	itoa(selectedMinute, tempc);
	if(selectedMinute < 10) {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1E);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1F);
	lcd_send_string(":");

	itoa(selectedSecond, tempc);
	if(selectedSecond < 10) {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x21);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string(tempc);
	}
}

/***********************************************/
void showSelectedFeedTime() {
/***********************************************/
	char tempc [10];
	itoa(selectedFeedHour, tempc);

	if(selectedFeedHour < 10) {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1B);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1C);
	lcd_send_string(":");

	itoa(selectedFeedMinute, tempc);
	if(selectedFeedMinute < 10) {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1E);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1F);
	lcd_send_string(":");

	itoa(selectedFeedSecond, tempc);
	if(selectedFeedSecond < 10) {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x21);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string(tempc);
	}
}

/***********************************************/
void showSelectedDate() {
/***********************************************/
	char tempc [10];
	itoa(selectedDay, tempc);

	if(selectedDay < 10) {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1B);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1A);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1C);
	lcd_send_string("/");

	itoa(selectedMonth, tempc);
	if(selectedMonth < 10) {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x1E);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x1D);
		lcd_send_string(tempc);
	}

	lcd_send_cmd(0x80|0x1F);
	lcd_send_string("/");

	itoa(selectedYear, tempc);
	if(selectedYear < 10) {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string("0");
		lcd_send_cmd(0x80|0x21);
		lcd_send_string(tempc);
	}
	else {
		lcd_send_cmd(0x80|0x20);
		lcd_send_string(tempc);
	}
}

/***********************************************/
void checkIfIsTimeToFeed() {
/***********************************************/
	RTC_TimeTypeDef TempsTime = { 0 };

	HAL_RTC_GetTime(&hrtc, &TempsTime, RTC_FORMAT_BIN);
	if(TempsTime.Hours == selectedFeedHour && TempsTime.Minutes == selectedFeedMinute && TempsTime.Seconds == selectedFeedSecond) {
		handle_event(FEED_TIME_EVENT);
		startFeeding();
	}
}

/***********************************************/
void startFeeding() {
/***********************************************/
	triggerOnce();
	HAL_GPIO_WritePin(MOTOR_Enable_GPIO_Port, MOTOR_Enable_Pin, GPIO_PIN_SET);
}

/***********************************************/
void stopFeeding() {
/***********************************************/
	HAL_GPIO_WritePin(MOTOR_Enable_GPIO_Port, MOTOR_Enable_Pin, GPIO_PIN_RESET);
	handle_event(END_FEED_TIME_EVENT);
}
