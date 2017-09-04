#include <avr/io.h>
#include <stdlib.h>

void rand_seed_init()
{
	/* use ADC7 */
	ADMUX |= (1 << REFS0) | (7 << MUX0);

	/* prescaler at 128 */
	ADCSRA |= (7 << ADPS0);

	/* enable ADC */
	ADCSRA |= (1 << ADEN);

	/* start conversion */
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));

	uint8_t byte1 = ADCL;

	/* start conversion */
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	uint8_t byte2 = ADCL;

	uint16_t seed = byte1 << 8 | byte2;

	srand(seed);

	ADCSRA &= ~(1 << ADSC);
}

void timer_init()
{
	/* non-inverting mode, CTC with TOP at OCR1A */
	/* prescaler 1024 */
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (5 << CS10);

	/* activate interrupts */
	TIMSK1 |= (1 << OCIE1A);

	/* so that we get a 1Hz timer */
	OCR1A = 0x3D09;
}