#include "mod_adc.h"

void adc_init(ref_voltage ref, alignment alin, prescale presc, channel ch) {
	set_reference_voltage(ref);
	set_bit_alignment(alin);
	set_prescale(presc);
	set_analog_channel(ch);
	enable_adc(true);
}

void set_reference_voltage(ref_voltage v){
	switch (v) {
		case INTERNAL_VOLTAGE: ADMUX = (ADMUX & 0b00111111) | 0b11000000; break;
		case AVCC:             ADMUX = (ADMUX & 0b00111111) | 0b01000000; break;
		case AREF:             ADMUX = (ADMUX & 0b00111111);              break;
	}
}

void set_bit_alignment(alignment a) {
	if (a == LEFT) ADMUX |=  (1 << ADLAR);
	else           ADMUX &= ~(1 << ADLAR);
}

void set_analog_channel(channel canal) {
	ADMUX = (ADMUX & 0b11110000) | (canal & 0b00001111);
}

void enable_adc(bool enable) {
	if (enable) ADCSRA |=  (1 << ADEN);
	else        ADCSRA &= ~(1 << ADEN);
}

void start_conversion() {
	ADCSRA |= (1 << ADSC);
}

void enable_auto_trigger(bool enable) {
	if (enable) ADCSRA |=  (1 << ADATE);
	else        ADCSRA &= ~(1 << ADATE);
}

bool is_conversion_complete(void) {
	if (ADCSRA & (1 << ADIF)) {
		ADCSRA |= (1 << ADIF);  // limpa flag
		return true;
	}
	return false;
}

void enable_interrupt(bool enable) {
	if (enable) ADCSRA |=  (1 << ADIE);
	else        ADCSRA &= ~(1 << ADIE);
}

void set_prescale(prescale p) {
	ADCSRA = (ADCSRA & 0b11111000) | (p & 0b00000111);
}

uint16_t read_adc() {
	bool is_left_alignment = (ADMUX & (1 << ADLAR)) != 0;
	start_conversion();
	while (!is_conversion_complete());
	
	if (is_left_alignment)
		return ADCH; // Apenas 8 bits significativos
	else {
		uint8_t low  = ADCL;
		uint8_t high = ADCH;
		return (high << 8) | low;
	}
}

void set_auto_trigger_source(trigger_source s) {
	ADCSRB = (ADCSRB & 0b11111000) | (s & 0b00000111);
}

void disable_digital_input(channel canal, bool disable) {
	if (canal >= ADC0 && canal <= ADC5) {
		if (disable) DIDR0 |=  (1 << canal);   // Desliga entrada digital
		else         DIDR0 &= ~(1 << canal);  // Religa entrada digital
	}
}
