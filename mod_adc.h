/***************************************************************
 * @file    mod_adc.h
 * @author  Davi A. M. Gonzaga
 * @brief   Biblioteca para configuração e uso do ADC em microcontroladores AVR.
 * @version 1.0
 * @date    2025-11-11
 *
 * @details
 * Este módulo fornece funções de configuração e controle do conversor
 * analógico-digital (ADC) para microcontroladores da família AVR,
 * permitindo selecionar canal, referência, alinhamento, prescaler,
 * modo de trigger e leitura de dados.
 *
 ***************************************************************/

#ifndef MOD_ADC_H
#define MOD_ADC_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

/*--------------------------------------------------------------
 * ENUMERAÇÕES
 *--------------------------------------------------------------*/

/** @brief Tipos de tensão de referência possíveis. */
typedef enum { INTERNAL_VOLTAGE, AREF, AVCC } ref_voltage;

/** @brief Alinhamento do resultado da conversão no registrador ADC. */
typedef enum { LEFT, RIGHT } alignment;

/** @brief Seleção dos canais de entrada analógica. */
typedef enum {
	ADC0, 
	ADC1, 
	ADC2, 
	ADC3, 
	ADC4, 
	ADC5, 
	ADC6, 
	ADC7, 
	TEMPERATURE, 
	FIXED_VOLTAGE, 
	GND
} channel;

/** @brief Fatores de divisão de clock (prescaler) para o ADC. */
typedef enum {
	P_2 = 1,
	P_4,
	P_8, 
    P_16,
	P_32, 
	P_64, 
	P_128
} prescale;

/** @brief Fontes possíveis de disparo automático do ADC. */
typedef enum { 
	FREE_RUNNING,
	ANALOG_COMPARATOR,
	EXTERNAL_INTERRUPT0,
	TIMER0_COMP_A,
	TIMER0_OVERFLOW,
	TIMER1_COMP_B,
	TIMER1_OVERFLOW,
	TIMER1_CAPTURE
} trigger_source;


/*--------------------------------------------------------------
 * FUNÇÕES PRINCIPAIS
 *--------------------------------------------------------------*/

/**
 * @brief Inicializa o ADC com parâmetros básicos.
 * @param ref  Tipo de tensão de referência.
 * @param alin Alinhamento (LEFT ou RIGHT).
 * @param presc Fator de divisão do clock.
 * @param ch Canal analógico a ser selecionado.
 */
void adc_init(ref_voltage ref, alignment alin, prescale presc, channel ch);


/*--------------------------------------------------------------
 * FUNÇÕES DE CONFIGURAÇÃO DO REGISTRADOR ADMUX
 *--------------------------------------------------------------*/

/**
 * @brief Define a tensão de referência do ADC (bits REFS1:0 do ADMUX).
 */
void set_reference_voltage(ref_voltage v);

/**
 * @brief Define o alinhamento do resultado no registrador ADC (bit ADLAR).
 */
void set_bit_alignment(alignment a);

/**
 * @brief Seleciona o canal de entrada analógica (bits MUX3:0 do ADMUX).
 */
void set_analog_channel(channel canal);


/*--------------------------------------------------------------
 * FUNÇÕES DE CONTROLE DO REGISTRADOR ADCSRA
 *--------------------------------------------------------------*/

/**
 * @brief Habilita ou desabilita o conversor ADC (bit ADEN).
 */
void enable_adc(bool enable);

/**
 * @brief Inicia manualmente uma conversão (bit ADSC).
 */
void start_conversion();

/**
 * @brief Habilita ou desabilita o modo de disparo automático (bit ADATE).
 */
void enable_auto_trigger(bool enable);

/**
 * @brief Verifica se a conversão terminou (bit ADIF).
 * @return true se a conversão terminou, false caso contrário.
 * @note A leitura limpa a flag automaticamente.
 */
bool is_conversion_complete(void);

/**
 * @brief Habilita ou desabilita a interrupção do ADC (bit ADIE).
 */
void enable_interrupt(bool enable);

/**
 * @brief Define o fator de divisão de clock (prescaler) do ADC (bits ADPS2:0).
 */
void set_prescale(prescale p);

/*--------------------------------------------------------------
 * FUNÇÕES DE LEITURA
 *--------------------------------------------------------------*/

/**
 * @brief Realiza uma leitura do ADC e retorna o resultado.
 * @return Resultado da conversão (10 bits).
 */
uint16_t read_adc();

/*--------------------------------------------------------------
 * FUNÇÕES DE CONFIGURAÇÃO DO REGISTRADOR ADCSRB E DIDR0
 *--------------------------------------------------------------*/

/**
 * @brief Define a fonte de disparo automático do ADC (bits ADTS2:0 do ADCSRB).
 */
void set_auto_trigger_source(trigger_source s);

/**
 * @brief Habilita ou desabilita a entrada digital de um canal (bits ADC5D:0D do DIDR0).
 * @param canal Canal a ser modificado.
 * @param disable true para desativar a entrada digital, false para ativar.
 */
void disable_digital_input(channel canal, bool disable);
#endif
