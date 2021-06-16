/*
 * pasco2.h
 *
 *  Created on: 2 Nov 2020
 *      Author: aamark
 */

#ifndef PASCO2_H_
#define PASCO2_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CO2_I2C_ADDRESS 0x28

#define PROD_ID 		0x00
#define SENS_STS		0x01
#define MEAS_RATE_H		0x02
#define MEAS_RATE_L		0x03
#define MEAS_CFG		0x04
#define CO2PPM_H		0x05
#define CO2PPM_L		0x06
#define MEAS_STS		0x07
#define INT_CFG			0x08
#define ALARM_TH_H		0x09
#define ALARM_TH_L		0x0A
#define PRESS_REF_H		0x0B
#define PRESS_REF_L		0x0C
#define CALIB_REF_H		0x0D
#define CALIB_REF_L		0x0E
#define SCRATCH_PAD		0x0F
#define SENS_RST		0x10

#define PROD_ID_PROD_MSK		0x70
#define PROD_ID_PROD_POS		5
#define PROD_ID_REV_MSK			0x1F
#define PROD_ID_REV_POS			0

#define SENS_STS_SEN_RDY_MSK		0x80
#define SENS_STS_SEN_RDY_POS		7
#define SENS_STS_PWM_DIS_ST_MSK		0x40
#define SENS_STS_PWM_DIS_ST_POS 	6
#define SENS_STS_ORTMP_MSK			0x20
#define SENS_STS_ORTMP_POS			5
#define SENS_STS_ORVS_MSK			0x10
#define SENS_STS_ORVS_POS			4
#define SENS_STS_ICCER_MSK			0x08
#define SENS_STS_ICCER_POS			3
#define SENS_STS_ORTMP_CLR_MSK		0x04
#define SENS_STS_ORTMP_CLR_POS		2
#define SENS_STS_ORVS_CLR_MSK		0x02
#define SENS_STS_ORVS_CLR_POS		1
#define SENS_STS_ICCER_CLR_MSK		0x01
#define SENS_STS_ICCER_CLR_POS		0

#define MEAS_CFG_PWM_OUTEN_MSK		0x20
#define MEAS_CFG_PWM_OUTEN_POS		5
#define MEAS_CFG_PWM_MODE_MSK		0x10
#define MEAS_CFG_PWM_MODE_POS		4
#define MEAS_CFG_BOC_CFG_MSK		0x0C
#define MEAS_CFG_BOC_CFG_POS		2
#define MEAS_CFG_OP_MODE_MSK		0x03
#define MEAS_CFG_OP_MODE_POS		0

#define MEAS_STS_RES_MSK			0x20
#define MEAS_STS_RES_POS			5
#define MEAS_STS_DRDY_MSK			0x10
#define MEAS_STS_DRDY_POS			4
#define MEAS_STS_INT_STS_MSK		0x08
#define MEAS_STS_INT_STS_POS		3
#define MEAS_STS_ALARM_MSK			0x04
#define MEAS_STS_ALARM_POS			2
#define MEAS_STS_INT_STS_CLR_MSK	0x02
#define MEAS_STS_INT_STS_CLR_POS	1
#define MEAS_STS_ALARM_CLR_MSK		0x01
#define MEAS_STS_ALARM_CLR_POS		0

#define INT_CFG_INT_TYP_MSK			0x10
#define INT_CFG_INT_TYP_POS			4
#define INT_CFG_INT_FUNC_MSK		0x0E
#define INT_CFG_INT_FUNC_POS		1
#define INT_CFG_ALARM_TYP_MSK		0x01
#define INT_CFG_ALARM_TYP_POS		0

typedef enum
{
	PASCO2_PWM_MODE_SINGLE_PULSE,
	PASCO2_PWM_MODE_TRAIN,
}pasco2_pwm_mode_t;

typedef enum
{
	PASCO2_BOC_CFG_DISABLED,
	PASCO2_BOC_CFG_ENABLED,
	PASCO2_BOC_CFG_FORCED,
}pasco2_boc_cfg_t;

typedef enum
{
	PASCO2_OP_MODE_IDLE,
	PASCO2_OP_MODE_SINGLE,
	PASCO2_OP_MODE_CONTINUOUS,
}pasco2_op_mode_t;

typedef enum
{
	PASCO2_INT_TYP_ACTIVELOW,
	PASCO2_INT_TYP_ACTIVEHIGH,
}pasco2_int_typ_t;

typedef enum
{
	PASCO2_INT_FUNC_INACTIVE,
	PASCO2_INT_FUNC_ALARM,
	PASCO2_INT_FUNC_DATAREADY,
	PASCO2_INT_FUNC_BUSY,
	PASCO2_INT_FUNC_EARLY,
}pasco2_int_func_t;

typedef enum
{
	PASCO2_ALARM_TYP_TRESHOLD_LOWER,
	PASCO2_ALARM_TYP_TRESHOLD_HIGHER
}pasco2_alarm_typ_t;

typedef enum
{
	PASCO2_SRTRG_SOFT_RESET = 0xA3,
	PASCO2_SRTRG_ABOC_CORRECTION_RESET = 0xBC,
	PASCO2_SRTRG_FORCED_CALIB_RESET = 0xFC,
}pasco2_srtrg_t;

typedef struct
{
	uint8_t			address;
}pasco2_t;

void pasco2_init(pasco2_t* co2, uint8_t i2cAddress);

uint8_t pasco2_getProductId(pasco2_t* co2);
uint8_t pasco2_getRevisionId(pasco2_t* co2);

uint8_t pasco2_isSensorReady(pasco2_t* co2);
uint8_t pasco2_getPwmDisPinStatus(pasco2_t* co2);
uint8_t pasco2_isTemperatureOutOfRange(pasco2_t* co2);
uint8_t pasco2_isVdd12OutOfRange(pasco2_t* co2);
uint8_t pasco2_isCommunicationError(pasco2_t* co2);
void pasco2_clearTemperatureOutOfRange(pasco2_t* co2);
void pasco2_clearVdd12OutOfRange(pasco2_t* co2);
void pasco2_clearCommunicationError(pasco2_t* co2);

int16_t pasco2_getMeasurementPeriod(pasco2_t* co2);
void pasco2_setMeasurementPeriod(pasco2_t* co2, int16_t period);

uint8_t pasco2_getPwmEnable(pasco2_t* co2);
void pasco2_setPwmEnable(pasco2_t* co2, uint8_t enable);
pasco2_pwm_mode_t pasco2_getPwmMode(pasco2_t* co2);
void pasco2_setPwmMode(pasco2_t* co2, pasco2_pwm_mode_t mode);
pasco2_boc_cfg_t pasco2_getBaselineOffsetCompensation(pasco2_t* co2);
void pasco2_setBaselineOffsetCompensation(pasco2_t* co2, pasco2_boc_cfg_t boc);
pasco2_op_mode_t pasco2_getOperationMode(pasco2_t* co2);
void pasco2_setOperationMode(pasco2_t* co2, pasco2_op_mode_t mode);

int16_t pasco2_getCo2Concentration(pasco2_t* co2);

uint8_t pasco2_isDataReady(pasco2_t* co2);
uint8_t pasco2_getIntPinStatus(pasco2_t* co2);
uint8_t pasco2_getAlarmNotification(pasco2_t* co2);
void pasco2_clearIntPinStatus(pasco2_t* co2);
void pasco2_clearAlarmNotification(pasco2_t* co2);

pasco2_int_typ_t pasco2_getIntTyp(pasco2_t* co2);
void pasco2_setIntTyp(pasco2_t* co2, pasco2_int_typ_t typ);
pasco2_int_func_t pasco2_getIntFunction(pasco2_t* co2);
void pasco2_setIntFunction(pasco2_t* co2, pasco2_int_func_t func);
pasco2_alarm_typ_t pasco2_getAlarm(pasco2_t* co2);
void pasco2_setAlarm(pasco2_t* co2, pasco2_alarm_typ_t alarm);

int16_t pasco2_getAlarmThreshold(pasco2_t* co2);
void pasco2_setAlarmThreshold(pasco2_t* co2, int16_t threshold);

uint16_t pasco2_getPressureCompensation(pasco2_t* co2);
void pasco2_setPressureCompensation(pasco2_t* co2, uint16_t pressure);

int16_t pasco2_getAutomaticBaselineCompensation(pasco2_t* co2);
void pasco2_setAutomaticBaselineCompensation(pasco2_t* co2, int16_t ref);

uint8_t pasco2_getScratchPad(pasco2_t* co2);
void pasco2_setScratchPad(pasco2_t* co2, uint8_t scratch);

void pasco2_reset(pasco2_t* co2, pasco2_srtrg_t rst);

#endif /* PASCO2_H_ */
