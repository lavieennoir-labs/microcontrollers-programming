/*Include ST file*/
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
/*Include User file*/
#include "main.h"

float TM_LIS3DSH_INT_Sensitivity;

//initiating the pins
//selecting - output push-pull
//selecting - MOSI, MISO and CLOCK LINE - alternative function
void spi_init_pin (void)
{
	//settings for some port (will be applied later to the port)
	GPIO_InitTypeDef 	GPIO_Init_LED;
	
	//initiating user for SPI
	SPI_InitTypeDef		SPI_Init_user;
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_LED);
	
	//configuring alternative function
  //assigning pins to module SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	//turning on clock pulses for the port E
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_Init_LED);
	
	CS_OFF();
	
	//turning on clock pulses for interface SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//configuring SPI user (monitor sensor)
	SPI_Init_user.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init_user.SPI_Mode = SPI_Mode_Master;
	SPI_Init_user.SPI_DataSize = SPI_DataSize_8b;
	
	//TODO check documentation motion sensor
	SPI_Init_user.SPI_CPOL = SPI_CPOL_High;
	SPI_Init_user.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Init_user.SPI_NSS = SPI_NSS_Soft;
	SPI_Init_user.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_Init_user.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init_user.SPI_CRCPolynomial = 7;
	
	//applying settings to SPI user
	SPI_Init(SPI1, &SPI_Init_user);
	
	//enabling SPI1
	SPI_Cmd(SPI1, ENABLE);
}

//waiting until the bus is free (by checking its business flag)
//sending data 
//waiting until data transmission is completed
//obtaining data from the bus
uint8_t Write_Data(uint8_t data) 
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}
  SPI_I2S_SendData(SPI1, data);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}
  return SPI_I2S_ReceiveData(SPI1);
}

//writing data in the motion sensor’s internal registers
void Write_Val_Reg(uint8_t address, uint8_t value) 
{
	CS_ON();
	Write_Data(address);
	Write_Data(value);
	CS_OFF();
}

//setting the most significant bit of the internal register address to 1
uint8_t Read_Val_Reg(uint8_t address) 
{
	uint8_t data=0;
	address = address|0x80;
	CS_ON();
	Write_Data(address);
	data = Write_Data(0x00);
  CS_OFF();
	
	return data;
}


void ReadAxes(LIS3DSH_t* Axes_Data) 
{
	int8_t buffer[6];

	buffer[0] = Read_Val_Reg(LIS3DSH_OUT_X_L_ADDR);
	buffer[1] = Read_Val_Reg(LIS3DSH_OUT_X_H_ADDR);
	buffer[2] = Read_Val_Reg(LIS3DSH_OUT_Y_L_ADDR);
	buffer[3] = Read_Val_Reg(LIS3DSH_OUT_Y_H_ADDR);
	buffer[4] = Read_Val_Reg(LIS3DSH_OUT_Z_L_ADDR);
	buffer[5] = Read_Val_Reg(LIS3DSH_OUT_Z_H_ADDR);
	
	/* Set axes */
	Axes_Data->X = (int16_t)((buffer[1] << 8) + buffer[0]) * 0.06;
	Axes_Data->Y = (int16_t)((buffer[3] << 8) + buffer[2]) * 0.06;
	Axes_Data->Z = (int16_t)((buffer[5] << 8) + buffer[4]) * 0.06;
}


void InitLIS3DSH(LIS3DSH_Sensitivity_t Sensitivity, LIS3DSH_Filter_t Filter) 
{
	uint8_t tmpreg;
	uint16_t temp;

	/* Set data */
	temp = (uint16_t) (LIS3DSH_DATARATE_100 | LIS3DSH_XYZ_ENABLE);
	temp |= (uint16_t) (LIS3DSH_SERIALINTERFACE_4WIRE | LIS3DSH_SELFTEST_NORMAL);
	
	/* Set sensitivity */
	if (Sensitivity == TM_LIS3DSH_Sensitivity_2G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_2);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_06G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_4G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_4);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_12G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_6G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_6);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_18G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_8G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_8);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_24G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_16G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_16);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_73G;
	} else {
		return;
	}
	
	/* Set filter */
	if (Filter == TM_LIS3DSH_Filter_800Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_800 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_400Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_400 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_200Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_200 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_50Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_50 << 8);
	} else {
		return;
	}
	
	/* Configure MEMS: power mode(ODR) and axes enable */
	tmpreg = (uint8_t) (temp);

	/* Write value to MEMS CTRL_REG4 register */
	Write_Val_Reg(LIS3DSH_CTRL_REG4_ADDR, tmpreg);
	/* Configure MEMS: full scale and self test */
	tmpreg = (uint8_t) (temp >> 8);

	/* Write value to MEMS CTRL_REG5 register */
	Write_Val_Reg(LIS3DSH_CTRL_REG5_ADDR, tmpreg);
}
