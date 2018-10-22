#ifndef __SimSPI_Conf_h__
#define __SimSPI_Conf_h__

#include "gpio.h"
#include "wfSys.h"

#define SimSPI_SPI0
// #define SimSPI_Delay_Ex
// #define SimSPI_Delay() wfDelay_ms(1);


#define SIMSPI_SCL_Low() HAL_GPIO_WritePin(RF_SCLK_GPIO_Port,RF_SCLK_Pin,GPIO_PIN_RESET)
#define SIMSPI_SCL_High() HAL_GPIO_WritePin(RF_SCLK_GPIO_Port,RF_SCLK_Pin,GPIO_PIN_SET)

#define SIMSPI_SDO_Low() HAL_GPIO_WritePin(RF_MOSI_GPIO_Port,RF_MOSI_Pin,GPIO_PIN_RESET)
#define SIMSPI_SDO_High() HAL_GPIO_WritePin(RF_MOSI_GPIO_Port,RF_MOSI_Pin,GPIO_PIN_SET)

#define SIMSPI_SDI_Read() HAL_GPIO_ReadPin(RF_MISO_GPIO_Port,RF_MISO_Pin)


#define SimSPI2
#define SimSPI_SPI0
// #define SimSPI_Delay_Ex
// #define SimSPI_Delay() wfDelay_ms(1);
 
 
#define SIMSPI2_SCL_Low() HAL_GPIO_WritePin(FM25V10_SCLK_GPIO_Port,FM25V10_SCLK_Pin,GPIO_PIN_RESET)
#define SIMSPI2_SCL_High() HAL_GPIO_WritePin(FM25V10_SCLK_GPIO_Port,FM25V10_SCLK_Pin,GPIO_PIN_SET)
 
#define SIMSPI2_SDO_Low() HAL_GPIO_WritePin(FM25V10_MOSI_GPIO_Port,FM25V10_MOSI_Pin,GPIO_PIN_RESET)
#define SIMSPI2_SDO_High() HAL_GPIO_WritePin(FM25V10_MOSI_GPIO_Port,FM25V10_MOSI_Pin,GPIO_PIN_SET)
 
#define SIMSPI2_SDI_Read() HAL_GPIO_ReadPin(FM25V10_MISO_GPIO_Port,FM25V10_MISO_Pin)

#endif


