#include <led.h>
#include <adc.h>
#include "main.h"
#include "config.h"

void delay(int dly) {
  while (dly--);
}

int main(void)
{
  uint32_t volts, millivolts;
  adc_status_t adc_status;

  led_up(BLINK_LED);

  adc_status = adc_up();
  if (ADC_ERROR(adc_status)) {
    assert_failed(__FILE__, __LINE__);
  }

  __enable_irq();

	while (1) {
    led_on(BLINK_LED);
		delay(200000);

    adc_status = adc_convert(ADC_CONVERT_PA0);
    if (ADC_ERROR(adc_status)) {
      assert_failed(__FILE__, __LINE__);
    }
    volts = ADC_VOLTS(adc_status.data);
    millivolts = ADC_MILLIVOLTS(adc_status.data);

    led_off(BLINK_LED);
		delay(200000);
	}

  led_down(BLINK_LED);

  adc_status = adc_down();
  if (ADC_ERROR(adc_status)) {
    assert_failed(__FILE__, __LINE__);
  }

  /* TODO go to sleep
   *
   * Calls the ARM `WFI` instruction.
   *
   * WFI (Wait For Interrupt) makes the processor suspend execution (Clock is
   * stopped) until one of the following events take place:
   * - An IRQ interrupt
   * - An FIQ interrupt
   * - A Debug Entry request made to the processor.
   */
  // cpu_sleep();

	return 0;
}

void assert_failed(uint8_t* file, uint32_t line) {
  while (1) {}
}
