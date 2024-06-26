/******************************************************************************
*Copyright(C)2017, Xiaohua Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Xiaohua Semiconductor Co.,Ltd("XHSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with XHSC
* components. This software is licensed by XHSC to be adapted only
* for use in systems utilizing XHSC components. XHSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. XHSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* XHSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* XHSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/** \file bt.c
 **
 ** Common API of base timer.
 ** @link btGroup Some description @endlink
 **
 **   - 2017-05-16
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "bt.h"
/**
 *******************************************************************************
 ** \addtogroup BtGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_TIM(x)         (TIM0 == (x) ||\
                                 TIM1 == (x) ||\
                                 TIM2 == (x))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *****************************************************************************
 ** \brief Base Timer 中断标志获取
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval TRUE or FALSE
 *****************************************************************************/
boolean_t Bt_GetIntFlag(en_bt_unit_t enUnit)
{
    boolean_t bRetVal = FALSE;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            bRetVal = M0P_TIM0->IFR_f.UIF ? TRUE : FALSE;
            break;
        case TIM1:
            bRetVal = M0P_TIM1->IFR_f.UIF ? TRUE : FALSE;
            break;
        case TIM2:
            bRetVal = M0P_TIM2->IFR_f.UIF ? TRUE : FALSE;
            break;
        default:
            bRetVal = FALSE;
            break;
    }

    return bRetVal;
}

/**
 *****************************************************************************
 ** \brief Base Timer 中断标志清除
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_ClearIntFlag(en_bt_unit_t enUnit)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->ICLR_f.UIF = FALSE;
            break;
        case TIM1:
            M0P_TIM1->ICLR_f.UIF = FALSE;;
            break;
        case TIM2:
            M0P_TIM2->ICLR_f.UIF = FALSE;;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 中断使能
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_EnableIrq (en_bt_unit_t enUnit)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CR_f.UIE = TRUE;
            break;
        case TIM1:
            M0P_TIM1->CR_f.UIE = TRUE;
            break;
        case TIM2:
            M0P_TIM2->CR_f.UIE = TRUE;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 中断禁止
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_DisableIrq(en_bt_unit_t enUnit)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CR_f.UIE = FALSE;
            break;
        case TIM1:
            M0P_TIM1->CR_f.UIE = FALSE;
            break;
        case TIM2:
            M0P_TIM2->CR_f.UIE = FALSE;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 初始化配置
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 ** \param [in]  pstcCfg       初始化配置结构体指针
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_Init(en_bt_unit_t enUnit, stc_bt_cfg_t* pstcCfg)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            {
                M0P_TIM0->CR_f.GATEP = pstcCfg->enGateP;
                M0P_TIM0->CR_f.GATE   = pstcCfg->enGate;
                M0P_TIM0->CR_f.PRS    = pstcCfg->enPRS;
                M0P_TIM0->CR_f.TOGEN = pstcCfg->enTog;
                M0P_TIM0->CR_f.CT     = pstcCfg->enCT;
                M0P_TIM0->CR_f.MD     = pstcCfg->enMD;

            }
            break;
        case TIM1:
            {
                M0P_TIM1->CR_f.GATEP = pstcCfg->enGateP;
                M0P_TIM1->CR_f.GATE   = pstcCfg->enGate;
                M0P_TIM1->CR_f.PRS    = pstcCfg->enPRS;
                M0P_TIM1->CR_f.TOGEN = pstcCfg->enTog;
                M0P_TIM1->CR_f.CT     = pstcCfg->enCT;
                M0P_TIM1->CR_f.MD     = pstcCfg->enMD;

            }
            break;
        case TIM2:
            {
                M0P_TIM2->CR_f.GATEP = pstcCfg->enGateP;
                M0P_TIM2->CR_f.GATE   = pstcCfg->enGate;
                M0P_TIM2->CR_f.PRS    = pstcCfg->enPRS;
                M0P_TIM2->CR_f.TOGEN = pstcCfg->enTog;
                M0P_TIM2->CR_f.CT     = pstcCfg->enCT;
                M0P_TIM2->CR_f.MD     = pstcCfg->enMD;

            }
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 启动运行
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_Run(en_bt_unit_t enUnit)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CR_f.CTEN = TRUE;
            break;
        case TIM1:
            M0P_TIM1->CR_f.CTEN = TRUE;
            break;
        case TIM2:
            M0P_TIM2->CR_f.CTEN = TRUE;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 停止运行
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_Stop(en_bt_unit_t enUnit)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CR_f.CTEN = FALSE;
            break;
        case TIM1:
            M0P_TIM1->CR_f.CTEN = FALSE;
            break;
        case TIM2:
            M0P_TIM2->CR_f.CTEN = FALSE;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 重载值设置
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 ** \param [in]  u16Data          16bits重载值
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_ARRSet(en_bt_unit_t enUnit, uint16_t u16Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->ARR_f.ARR = u16Data;
            break;
        case TIM1:
            M0P_TIM1->ARR_f.ARR = u16Data;
            break;
        case TIM2:
            M0P_TIM2->ARR_f.ARR = u16Data;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 16位计数器初值设置
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 ** \param [in]  u16Data          16位初值
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_Cnt16Set(en_bt_unit_t enUnit, uint16_t u16Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CNT_f.CNT = u16Data;
            break;
        case TIM1:
            M0P_TIM1->CNT_f.CNT = u16Data;
            break;
        case TIM2:
            M0P_TIM2->CNT_f.CNT = u16Data;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 16位计数值获取
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval 16bits计数值
 *****************************************************************************/
uint16_t Bt_Cnt16Get(en_bt_unit_t enUnit)
{
    uint16_t    u16CntData = 0;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            u16CntData = M0P_TIM0->CNT_f.CNT;
            break;
        case TIM1:
            u16CntData = M0P_TIM1->CNT_f.CNT;
            break;
        case TIM2:
            u16CntData = M0P_TIM2->CNT_f.CNT;
            break;
        default:
            u16CntData = 0;
            break;
    }

    return u16CntData;
}

/**
 *****************************************************************************
 ** \brief Base Timer 32位计数器初值设置
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 ** \param [in]  u32Data          32位初值
 **
 ** \retval Ok or Error
 *****************************************************************************/
en_result_t Bt_Cnt32Set(en_bt_unit_t enUnit, uint32_t u32Data)
{
    en_result_t enResult = Ok;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            M0P_TIM0->CNT32_f.CNT32 = u32Data;
            break;
        case TIM1:
            M0P_TIM1->CNT32_f.CNT32 = u32Data;
            break;
        case TIM2:
            M0P_TIM2->CNT32_f.CNT32 = u32Data;
            break;
        default:
            enResult = Error;
            break;
    }

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Base Timer 32位计数值获取
 **
 **
 ** \param [in]  enUnit           Timer通道选择（TIM0、TIM1、TIM2）
 **
 ** \retval 32bits计数值
 *****************************************************************************/
uint32_t Bt_Cnt32Get(en_bt_unit_t enUnit)
{
    uint32_t    u32CntData = 0;

    ASSERT(IS_VALID_TIM(enUnit));

    switch (enUnit)
    {
        case TIM0:
            u32CntData = M0P_TIM0->CNT32_f.CNT32;
            break;
        case TIM1:
            u32CntData = M0P_TIM1->CNT32_f.CNT32;
            break;
        case TIM2:
            u32CntData = M0P_TIM2->CNT32_f.CNT32;
            break;
        default:
            u32CntData = 0;
            break;
    }

    return u32CntData;
}

//@} // BtGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
