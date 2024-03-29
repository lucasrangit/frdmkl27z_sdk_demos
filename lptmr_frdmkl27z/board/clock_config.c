/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Call CLOCK_SetMcgliteConfig to set MCG_Lite configuration.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!ClocksProfile
product: Clocks v1.0
processor: MKL27Z64xxx4
package_id: MKL27Z64VLH4
mcu_data: ksdk2_0
processor_version: 1.0.1
board: FRDM-KL27Z
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

#include "fsl_smc.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
#define SIM_OSC32KSEL_LPO_CLK                             3U  /*!< OSC32KSEL select: LPO clock */
#define SIM_CLKOUT_SEL_LPO_CLK                            3U  /*!< CLKOUT pin clock select: LPO clock */
#define SIM_CLKOUT_SEL_OSCERCLK_CLK                       6U  /*!< CLKOUT pin clock select: OSCERCLK clock */
#define SIM_CLKOUT_ERCLK32K_ENABLE                        1U  /*!< ERCLK32K is output on PTE0 *>

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockRUN
outputs:
- {id: Bus_clock.outFreq, value: 24 MHz}
- {id: Core_clock.outFreq, value: 48 MHz}
- {id: Flash_clock.outFreq, value: 24 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGIRCLK.outFreq, value: 8 MHz}
- {id: MCGPCLK.outFreq, value: 48 MHz}
- {id: System_clock.outFreq, value: 48 MHz}
settings:
- {id: MCGMode, value: HIRC}
- {id: MCG.CLKS.sel, value: MCG.HIRC}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_MC_HIRCEN_CFG, value: Enabled}
- {id: OSC0_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_SYS_OSC_CAP_LOAD_CFG, value: SC12PF}
- {id: SIM.CLKOUTSEL.sel, value: MCG.MCGPCLK}
- {id: SIM.COPCLKSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.FLEXIOSRCSEL.sel, value: MCG.MCGPCLK}
- {id: SIM.LPUART0SRCSEL.sel, value: MCG.MCGPCLK}
- {id: SIM.LPUART1SRCSEL.sel, value: MCG.MCGPCLK}
- {id: SIM.RTCCLKOUTSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.TPMSRCSEL.sel, value: MCG.MCGPCLK}
- {id: SIM.USBSRCSEL.sel, value: MCG.MCGPCLK}
sources:
- {id: MCG.HIRC.outFreq, value: 48 MHz}
- {id: OSC.OSC.outFreq, value: 32.768 kHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcglite_config_t mcgliteConfig_BOARD_BootClockRUN =
    {
        .outSrc = kMCGLITE_ClkSrcHirc,            /* MCGOUTCLK source is HIRC */
        .irclkEnableMode = kMCGLITE_IrclkEnable,  /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCGLITE_Lirc8M,                  /* Slow internal reference (LIRC) 8 MHz clock selected */
        .fcrdiv = kMCGLITE_LircDivBy1,            /* Low-frequency Internal Reference Clock Divider: divided by 1 */
        .lircDiv2 = kMCGLITE_LircDivBy1,          /* Second Low-frequency Internal Reference Clock Divider: divided by 1 */
        .hircEnableInNotHircMode = true,          /* HIRC source is enabled */
    };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
    {
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x10000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /2 */
    };
const osc_config_t oscConfig_BOARD_BootClockRUN =
    {
        .freq = 32768U,                           /* Oscillator frequency: 32768Hz */
        .capLoad = (kOSC_Cap4P | kOSC_Cap8P),     /* Oscillator capacity load: 12pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = kOSC_ErClkEnable,   /* Enable external reference clock, disable external reference clock in STOP mode */
            }
    };

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/

/*!
 * @brief Enable ERCLK32K clock output on PTE0.
 *
 * @param enable Boolean enable (1) or disable (0).
 */
static inline void CLOCK_EnableEr32kClockOut(uint32_t enable)
{
    SIM->SOPT1 = ((SIM->SOPT1 & ~SIM_SOPT1_OSC32KOUT_MASK) | SIM_SOPT1_OSC32KOUT(enable));
}


void BOARD_BootClockRUN(void)
{
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Initializes OSC0 according to board configuration. */
    CLOCK_InitOsc0(&oscConfig_BOARD_BootClockRUN);
    CLOCK_SetXtal0Freq(oscConfig_BOARD_BootClockRUN.freq);
    /* Set MCG to HIRC mode. */
    CLOCK_SetMcgliteConfig(&mcgliteConfig_BOARD_BootClockRUN);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN);
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    /* Set CLKOUT source. */
    CLOCK_SetClkOutClock(SIM_CLKOUT_SEL_OSCERCLK_CLK);
    CLOCK_EnableEr32kClockOut(SIM_CLKOUT_ERCLK32K_ENABLE);
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockVLPR ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockVLPR
outputs:
- {id: Bus_clock.outFreq, value: 1 MHz}
- {id: Core_clock.outFreq, value: 2 MHz}
- {id: Flash_clock.outFreq, value: 1 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGIRCLK.outFreq, value: 2 MHz}
- {id: System_clock.outFreq, value: 2 MHz}
settings:
- {id: MCGMode, value: LIRC2M}
- {id: powerMode, value: VLPR}
- {id: MCG.LIRCDIV1.scale, value: '1', locked: true}
- {id: OSC0_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_SYS_OSC_CAP_LOAD_CFG, value: SC12PF}
- {id: RTCCLKOUTConfig, value: 'yes'}
- {id: SIM.OUTDIV1.scale, value: '1', locked: true}
- {id: SIM.OUTDIV4.scale, value: '2', locked: true}
- {id: SIM.RTCCLKOUTSEL.sel, value: OSC.OSCERCLK}
sources:
- {id: MCG.LIRC.outFreq, value: 2 MHz}
- {id: OSC.OSC.outFreq, value: 32.768 kHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockVLPR configuration
 ******************************************************************************/
const mcglite_config_t mcgliteConfig_BOARD_BootClockVLPR =
    {
        .outSrc = kMCGLITE_ClkSrcLirc,            /* MCGOUTCLK source is LIRC */
        .irclkEnableMode = kMCGLITE_IrclkEnable,  /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCGLITE_Lirc2M,                  /* Slow internal reference (LIRC) 2 MHz clock selected */
        .fcrdiv = kMCGLITE_LircDivBy1,            /* Low-frequency Internal Reference Clock Divider: divided by 1 */
        .lircDiv2 = kMCGLITE_LircDivBy1,          /* Second Low-frequency Internal Reference Clock Divider: divided by 1 */
        .hircEnableInNotHircMode = false,         /* HIRC source is not enabled */
    };
const sim_clock_config_t simConfig_BOARD_BootClockVLPR =
    {
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x10000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /2 */
    };
const osc_config_t oscConfig_BOARD_BootClockVLPR =
    {
        .freq = 0U,                               /* Oscillator frequency: 0Hz */
        .capLoad = (kOSC_Cap4P | kOSC_Cap8P),     /* Oscillator capacity load: 12pF */
        .workMode = kOSC_ModeExt,                 /* Use external clock */
        .oscerConfig =
            {
                .enableMode = kOSC_ErClkEnable,   /* Enable external reference clock, disable external reference clock in STOP mode */
            }
    };

/*******************************************************************************
 * Code for BOARD_BootClockVLPR configuration
 ******************************************************************************/
void BOARD_BootClockVLPR(void)
{
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Set MCG to LIRC2M mode. */
    CLOCK_SetMcgliteConfig(&mcgliteConfig_BOARD_BootClockVLPR);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockVLPR);
    /* Set VLPR power mode. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
    SMC_SetPowerModeVlpr(SMC, false);
#else
    SMC_SetPowerModeVlpr(SMC);
#endif
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKVLPR_CORE_CLOCK;
}

