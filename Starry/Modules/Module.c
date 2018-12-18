/* ---------------------------------------------------------------------
 * 
 * Module库函数:
 * 传感器操作函数.
 *
 *   修订操作      版本号       日期         修订人
 * ------------    ------    ----------    ------------
 *     创建        1.0       2018.01.19      Universea
 *
 * ---------------------------------------------------------------------*/
#include "ModulesConfig.h"
#include "module.h"
#include "faultDetect.h"
#include "Icm20689.h"
#include "Ak8975.h"
#include "ublox.h"
#include "Spl06.h"
#include "board.h"
#include "Rotation.h"
#include "Vl53l0X.h"

/**********************************************************************************************************
*函 数 名: GyroSensorRotate
*功能说明: 陀螺仪传感器方向变换，以适应传感器的实际安装方向
*形    参: 陀螺仪数据指针
*返 回 值: 无
**********************************************************************************************************/
static void GyroSensorRotate(Vector3ForFloat* gyro)
{
    RotateVector3f(GYRO_ROTATION, gyro);
}

/**********************************************************************************************************
*函 数 名: AccSensorRotate
*功能说明: 加速度数据方向变换，以适应传感器的实际安装方向
*形    参: 加速度数据指针
*返 回 值: 无
**********************************************************************************************************/
static void AccSensorRotate(Vector3ForFloat *acc)
{
    RotateVector3f(ACC_ROTATION, acc);
}

/**********************************************************************************************************
*函 数 名: MagSensorRotate
*功能说明: 磁力计数据方向变换，以适应传感器的实际安装方向
*形    参: 磁力计数据指针
*返 回 值: 无
**********************************************************************************************************/
static void MagSensorRotate(Vector3ForFloat *mag)
{
    RotateVector3f(MAG_ROTATION, mag);
}

/**********************************************************************************************************
*函 数 名: GyroSensorInit
*功能说明: 陀螺仪传感器初始化
*形    参: 无
*返 回 值: 陀螺仪存在状态
**********************************************************************************************************/
void GyroSensorInit(void)
{
    uint8_t detectFlag = 0;

		if (GYRO_TYPE == ICM20689)
    {
        if(invSensorDetect())
        {
            invSensorInit();
            detectFlag = 1;
        }
    }

    //未检测到陀螺仪
    if(!detectFlag)
        FaultDetectSetError(GYRO_UNDETECTED);
}

/**********************************************************************************************************
*函 数 名: MagSensorInit
*功能说明: 磁力传感器初始化
*形    参: 无
*返 回 值: 磁力计存在状态
**********************************************************************************************************/
void MagSensorInit(void)
{
    uint8_t detectFlag = 0;

    if(MAG_TYPE == AK8975)
    {
        if(ak8975Detect())
        {
            ak8975Init();
            detectFlag = 1;
        }
    }
    //未检测到磁力计
    if(!detectFlag)
        FaultDetectSetError(MAG_UNDETECTED);
}

/**********************************************************************************************************
*函 数 名: BaroSensorInit
*功能说明: 气压传感器初始化
*形    参: 无
*返 回 值: 气压计存在状态
**********************************************************************************************************/
void BaroSensorInit(void)
{
    uint8_t detectFlag = 0;

    if(BARO_TYPE == SPL06)
    {
        if(spl0601Detect())
        {
            spl0601Init();
            detectFlag = 1;
        }
    }

    //未检测到气压计
    if(!detectFlag)
        FaultDetectSetError(BARO_UNDETECTED);
}

/**********************************************************************************************************
*函 数 名: TofSensorInit
*功能说明: TOF传感器初始化
*形    参: 无
*返 回 值: 气压计存在状态
**********************************************************************************************************/
void TofSensorInit(void)
{
   vl53l0xInit();
}

/**********************************************************************************************************
*函 数 名: GPSModuleInit
*功能说明: GPS模块初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void GPSModuleInit(void)
{
    ubloxInit();
}


/**********************************************************************************************************
*函 数 名: GyroSensorRead
*功能说明: 角速度数据读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void GyroSensorRead(Vector3ForFloat* gyro)
{
    if(GYRO_TYPE == ICM20689)
        invSensorReadGyro(gyro);

    //传感器方向转换
    GyroSensorRotate(gyro);
}

/**********************************************************************************************************
*函 数 名: AccSensorRead
*功能说明: 加速度数据读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void AccSensorRead(Vector3ForFloat* acc)
{
    if(GYRO_TYPE == ICM20689)
		{
				invSensorRead();
				invSensorReadAccel(acc);
		}
    //传感器方向转换
    AccSensorRotate(acc);
}

/**********************************************************************************************************
*函 数 名: TempSensorRead
*功能说明: 陀螺仪温度数据读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void TempSensorRead(float* temp)
{
    if(GYRO_TYPE == ICM20689)
        invSensorReadTemperature(temp);
}

/**********************************************************************************************************
*函 数 名: MagSensorUpdate
*功能说明: 罗盘数据更新
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void MagSensorUpdate(void)
{
    if(MAG_TYPE == AK8975)
        ak8975Update();
}

/**********************************************************************************************************
*函 数 名: MagSensorRead
*功能说明: 罗盘数据读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void MagSensorRead(Vector3ForFloat* mag)
{
    if(MAG_TYPE == AK8975)
       ak8975Read(mag);

    //传感器方向转换
    MagSensorRotate(mag);
}

/**********************************************************************************************************
*函 数 名: BaroSensorUpdate
*功能说明: 气压高度数据更新
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void BaroSensorUpdate(void)
{
    if(BARO_TYPE == SPL06)
        spl0601Update();
}

/**********************************************************************************************************
*函 数 名: BaroSensorRead
*功能说明: 气压高度数据读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void BaroSensorRead(int32_t* baroAlt)
{
    if(BARO_TYPE == SPL06)
        spl0601Read(baroAlt);
}

/**********************************************************************************************************
*函 数 名: BaroTemperatureRead
*功能说明: 气压计温度读取
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void BaroTemperatureRead(float* temp)
{
    if(BARO_TYPE == SPL06)
        spl0601ReadTemperature(temp);
}

/**********************************************************************************************************
*函 数 名: TempControlSet
*功能说明: 陀螺仪传感器加热数值设置（0-1000）
*形    参: 读出数据指针
*返 回 值: 无
**********************************************************************************************************/
void TempControlSet(int16_t value)
{
#if(configUSE_SENSORHEAT == 1)
    //TempControlPWMSet(value);
#else
    //TempControlPWMSet(0);
#endif
}

