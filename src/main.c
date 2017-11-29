#include <adc.h>
#include <gpio.h>
#include "main.h"
#include "config.h"
#include "hd44780.h"

/* One global to rule them all */
struct {
  bool paused;
  struct {
    uint32_t volts;
    uint32_t millivolts;
  } adc;
} flextimus_prime;

void assert_failed(uint8_t* file, uint32_t line) {
  while (1) {}
}

void delay(int dly) {
  while (dly--);
}

/* Called when the ADC finishes a conversion */
adc_status_t adc_convert_async_callback(adc_status_t adc_status) {
  flextimus_prime.adc.volts = ADC_VOLTS(adc_status.data);
  flextimus_prime.adc.millivolts = ADC_MILLIVOLTS(adc_status.data);
  gpio_off(PAUSE_LED);
  return ADC_OK;
}

int main(void) {
  adc_status_t adc_status;

  gpio_up(PAUSE_LED);

  adc_status = adc_up(FLEX_SENSOR);
  if (ADC_ERROR(adc_status)) {
    assert_failed(__FILE__, __LINE__);
  }

  // gpio_on(PAUSE_LED);

  //LCD Test code. NOTE: WILL HANG FOREVER IF YOU DON'T HAVE AN LCD ATTACHED
  //TODO_MAX: Update documentation with physical set up. Replicate final schematic set up.

  /*HD44780_Setup();
  HD44780_PowerOn();
  HD44780_Puts((uint8_t *)"Hello");
  HD44780_GotoXY(2,1);
  HD44780_Puts((uint8_t *)"World!");
  delay(5000000);
  HD44780_Clear();*/

  __enable_irq();

  adc_status = adc_convert_async(FLEX_SENSOR, adc_convert_async_callback);
  if (ADC_ERROR(adc_status)) {
    assert_failed(__FILE__, __LINE__);
  }

	while (1) {
    // PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
	}

  gpio_down(PAUSE_LED);

  adc_status = adc_down();
  if (ADC_ERROR(adc_status)) {
    assert_failed(__FILE__, __LINE__);
  }

	return 0;
}

// Function to pause the alert system
void flextimus_prime_pause_pressed() {
  if (flextimus_prime.paused == 0) {
    flextimus_prime.paused = true;
    gpio_on(PAUSE_LED);
  } else {
    flextimus_prime.paused = false;
    gpio_off(PAUSE_LED);
  }
}

void flextimus_prime_config_pressed() {
  if (flextimus_prime.paused == 0) {
    flextimus_prime.paused = true;
    gpio_on(PAUSE_LED);
  } else {
    flextimus_prime.paused = false;
    gpio_off(PAUSE_LED);
  }
}

// IRQ handler for both button interrupts
void button_irq_handler() {
  if (gpio_asserted(PAUSE_BUTTON)) {
    flextimus_prime_pause_pressed();
  } else if (gpio_asserted(CONFIG_BUTTON)) {
    flextimus_prime_config_pressed();
  }
}
