#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LED_PIN_LEFT (1<<7)
#define LED_PIN_RIGHT (1<<6)
#define SW_PIN_LEFT (1<<5)
#define SW_PIN_RIGHT (1<<4)
#define ENABLE_SENSOR_RIGHT 1
#define ENABLE_SENSOR_LEFT 0

static void gpio_init(void){
  DDRD |= LED_PIN_LEFT | LED_PIN_RIGHT | SW_PIN_LEFT | SW_PIN_RIGHT;
  PORTD &= ~LED_PIN_LEFT | ~LED_PIN_RIGHT | ~SW_PIN_LEFT | ~SW_PIN_RIGHT;
}

static void gpio_write(uint8_t p, uint8_t state){
  PORTD &= p;

  if(state)
    PORTD |= p;
}

static void uart_init(void){
  UBRR0 = 8;
  UCSR0B |= (1<<TXEN0);
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

static void uart_transmit(char c){
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = c;
}

static void uart_transmits(char* s){
  while(*s){
    uart_transmit(*s);
    s++;
  }
}

static void adc_init(uint8_t p){
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);

  ADMUX = 0x80;
  ADMUX = 0x80 | p;
}

static uint16_t adc_read(uint8_t p){
  uint16_t adc_avg = 0;
  adc_init(p);
  ADCSRA |= (1<<ADSC);
  for(uint8_t x=0;x<10;x++){
    while(ADCSRA & (1<<ADSC));
    adc_avg += ADCW;
  }
  return (adc_avg/10);
}

static uint8_t sensor_read(uint8_t s){
  uint8_t adc_array[10][8];
  uint8_t adc_array_sum[8];
  uint8_t adc_avg = 0;

  if(s){ // ENABLE_SENSOR_RIGHT
    gpio_write(LED_PIN_RIGHT | SW_PIN_RIGHT, 1);
    gpio_write((~LED_PIN_LEFT | ~SW_PIN_LEFT), 0);
  }else{
    gpio_write((~LED_PIN_RIGHT | ~SW_PIN_RIGHT), 0);
    gpio_write(LED_PIN_LEFT | SW_PIN_LEFT, 1);
  }

  for(uint8_t y=0;y<10;y++){
    for(uint8_t x=0;x<7;x++){
      adc_array[y][x] = adc_read(0);
    }
  }
  for(uint8_t x=0;x<7;x++){
    for(uint8_t y=0;y<10;y++){
      adc_avg += adc_array[y][x];
    }
    adc_array_sum[x] = (adc_avg/10);
    adc_avg = 0;
  }

  for(uint8_t x=0;x<7;x++)
    adc_avg += adc_array_sum[x];
  return adc_avg;
}

int main(void){
  char adc_data_left[8] = "";
  char adc_data_right[8] = "";
  uint8_t adc_data_raw[2];
  uint8_t counter = 0;

  gpio_init();
  uart_init();

  uint8_t sensor_cal_left = 0;
  uint8_t sensor_cal_right = 0;

  sensor_cal_left = sensor_read(ENABLE_SENSOR_LEFT);
  _delay_ms(1);
  sensor_cal_right = sensor_read(ENABLE_SENSOR_RIGHT);
  _delay_ms(1);

  /* uncomment for debug */
  // itoa(sensor_cal_left, adc_data_left, 10);
  // itoa(sensor_cal_right, adc_data_right, 10);

  // uart_transmits(adc_data_left);
  // uart_transmit('|');
  // uart_transmits(adc_data_right);
  // uart_transmit('\n');
  /* uncomment for debug */

  while(1){
    // break;
    if(counter > 5){
      adc_data_raw[0] = sensor_read(ENABLE_SENSOR_LEFT);
      adc_data_raw[1] = sensor_read(ENABLE_SENSOR_RIGHT);
      itoa((adc_data_raw[0]), adc_data_left, 10);
      itoa((adc_data_raw[1]), adc_data_right, 10);

      counter = 0;
    }

    /* Comment for debug and calibrate */
    if(adc_data_raw[0] < (sensor_cal_left-1))
      uart_transmits((char*)"L\n");
    else if(adc_data_raw[1] < (sensor_cal_right-1))
      uart_transmits((char*)"R\n");
    else
      uart_transmits((char*)"F\n");
    /* Comment for debug and calibrate */

    /* Uncomment for debug and calibrate */
    // uart_transmits(adc_data_left);
    // uart_transmit('|');
    // uart_transmits(adc_data_right);
    // uart_transmit('\n');
    /* Uncomment for debug and calibrate */

    counter++;
  }
}
