#include "main.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "square.pio.h"
#include <tusb.h>
/* --- pin mappings --- */
#ifdef USE_PDND
static const uint dtrigger_input_pin = 18;
static const uint trigger_output_pin = 19;
#else
static const uint trigger_input_pin = 9;
static const uint trigger_output_pin = 6;
#endif
static const uint reset_pin = 2;

int main() {
  init_gpios(reset_pin);
  stdio_init_all();
  while (!tud_cdc_connected()) {
    /* If the Pico PCB is not conntected to USB it
    will IDLE here and toggle the board LED*/
    sleep_ms(100);
    toggle_led();
  }
  printf("USB connected.\n");
  // Uncomment for overclocking to 400MHz
  // oc(2);
  init_pios();
  DEBUG_PRINT("Starting program now...\n");
  char c = 0;
  /*This is the main programm:
  You can select the modes by sending the corresponding
  chars over UART. */
  while (true) {
    c = getchar();
    if (c == 0)
      continue;
    DEBUG_PRINT("Got char: %c \n", c);
    switch (c) {
    case 'l':
      print_current_glitch_timings(); // check if commands are as expected
      break;
    case '@':
    case 'r':
      reset_glitcher();
      break;
    case 'A':
    case 't':
      toggle_led();
      break;
    case 'C':
    case 'p':
      set_glitch_pulse_width();
      break;
    case 'D':
    case 'd':
      set_delay_from_trigger();
      break;
    case 'F':
    case 'g':
      glitch(trigger_input_pin, trigger_output_pin);
      break;
    case 'I':
    case 's':
      putchar(get_status());
      break;
    default:
      printf("Unknown command: '%c'. Expected:\n"
             "@/r (reset glitch status),\n"
             "A/t (toggle LED),\n"
             "C/p (set pulse width),\n"
             "D/d (set delay from trigger pulse),\n"
             "F/g (trigger a glitch),\n"
             "I/s (status of the glitch),\n"
             "l (print current glitch timings)\n",
             c);
      break;
    }
  }
}
