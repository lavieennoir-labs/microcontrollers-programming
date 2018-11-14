
#define CS_OFF() GPIO_SetBits(GPIOE, GPIO_Pin_3);
#define CS_ON() GPIO_ResetBits(GPIOE, GPIO_Pin_3);

#define LIS3DSH_DATARATE_100						((uint8_t)0x60)

#define LIS3DSH_FULLSCALE_2							((uint8_t)0x00)  /* 2 g  */
#define LIS3DSH_FULLSCALE_4							((uint8_t)0x08)  /* 4 g  */
#define LIS3DSH_FULLSCALE_6							((uint8_t)0x10)  /* 6 g  */
#define LIS3DSH_FULLSCALE_8							((uint8_t)0x18)  /* 8 g  */
#define LIS3DSH_FULLSCALE_16						((uint8_t)0x20)  /* 16 g */
#define LIS3DSH__FULLSCALE_SELECTION		((uint8_t)0x38)

#define LIS3DSH_FILTER_BW_800						((uint8_t)0x00)  /* 800 Hz */
#define LIS3DSH_FILTER_BW_400						((uint8_t)0x40)	 //((uint8_t)0x08) /* 400 Hz  */
#define LIS3DSH_FILTER_BW_200						((uint8_t)0x80)  //((uint8_t)0x10)  /* 200 Hz */
#define LIS3DSH_FILTER_BW_50						((uint8_t)(0x80 | 0x40))//((uint8_t)0x18)  /* 50 Hz  */
#define LIS3DSH_SELFTEST_NORMAL					((uint8_t)0x00)
#define LIS3DSH_XYZ_ENABLE							((uint8_t)0x07)
#define LIS3DSH_SERIALINTERFACE_4WIRE		((uint8_t)0x00)
#define LIS3DSH_SM_ENABLE								((uint8_t)0x01)
#define LIS3DSH_SM_DISABLE							((uint8_t)0x00)

#define LIS3DSH_SENSITIVITY_0_06G        0.06  /* 0.06 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_12G        0.12  /* 0.12 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_18G        0.18  /* 0.18 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_24G        0.24  /* 0.24 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_73G        0.73  /* 0.73 mg/digit*/

#define LIS3DSH_WHO_AM_I_ADDR						 0x0F
#define LIS3DSH_CTRL_REG4_ADDR					 0x20
#define LIS3DSH_CTRL_REG1_ADDR					 0x21
#define LIS3DSH_CTRL_REG2_ADDR					 0x22
#define LIS3DSH_CTRL_REG3_ADDR					 0x23
#define LIS3DSH_CTRL_REG5_ADDR					 0x24
#define LIS3DSH_CTRL_REG6_ADDR					 0x25
#define LIS3DSH_OUT_X_L_ADDR						 0x28
#define LIS3DSH_OUT_X_H_ADDR						 0x29
#define LIS3DSH_OUT_Y_L_ADDR						 0x2A
#define LIS3DSH_OUT_Y_H_ADDR						 0x2B
#define LIS3DSH_OUT_Z_L_ADDR						 0x2C
#define LIS3DSH_OUT_Z_H_ADDR						 0x2D

typedef enum {
	TM_LIS3DSH_Sensitivity_2G,
	TM_LIS3DSH_Sensitivity_4G,
	TM_LIS3DSH_Sensitivity_6G,
	TM_LIS3DSH_Sensitivity_8G,
	TM_LIS3DSH_Sensitivity_16G,
} LIS3DSH_Sensitivity_t;

typedef enum {

	TM_LIS3DSH_Filter_800Hz,
	TM_LIS3DSH_Filter_400Hz,
	TM_LIS3DSH_Filter_200Hz,
	TM_LIS3DSH_Filter_50Hz,
} LIS3DSH_Filter_t;

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} LIS3DSH_t;


void spi_init_pin (void);
uint8_t Write_Data(uint8_t data);
void Write_Val_Reg(uint8_t address, uint8_t value);
uint8_t Read_Val_Reg(uint8_t address);
void ReadAxes(LIS3DSH_t* Axes_Data);
void InitLIS3DSH(LIS3DSH_Sensitivity_t Sensitivity, LIS3DSH_Filter_t Filter);


