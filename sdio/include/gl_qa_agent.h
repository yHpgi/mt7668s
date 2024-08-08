/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(C) 2016 MediaTek Inc.
 */

/*! \file   gl_qa_agent.h
 *    \brief  This file includes private ioctl support.
 */

#ifndef _GL_QA_AGENT_H
#define _GL_QA_AGENT_H
#if CFG_SUPPORT_QA_TOOL
/*******************************************************************************
 *                         C O M P I L E R   F L A G S
 ********************************************************************************
 */

/*******************************************************************************
 *                    E X T E R N A L   R E F E R E N C E S
 ********************************************************************************
 */

/*******************************************************************************
 *                              C O N S T A N T S
 ********************************************************************************
 */

/*******************************************************************************
 *                                 M A C R O S
 ********************************************************************************
 */

/*******************************************************************************
 *                  F U N C T I O N   D E C L A R A T I O N S
 ********************************************************************************
 */

#define HQA_CMD_MAGIC_NO 0x18142880
#define HQA_CHIP_ID_6632 0x6632
#define HQA_CHIP_ID_7668 0x7668

#if CFG_SUPPORT_TX_BF
#define HQA_BF_STR_SIZE 512
#endif

#define HQA_RX_STATISTIC_NUM 66
#define BUFFER_BIN_MODE 0x0
#define EFUSE_MODE 0x2

extern UINT_8 uacEEPROMImage[MAX_EEPROM_BUFFER_SIZE];

typedef struct _PARAM_RX_STAT_T {
	UINT_32 MAC_FCS_Err; /* b0 */
	UINT_32 MAC_Mdrdy;	 /* b0 */
	UINT_32 FCSErr_CCK;
	UINT_32 FCSErr_OFDM;
	UINT_32 CCK_PD;
	UINT_32 OFDM_PD;
	UINT_32 CCK_SIG_Err;
	UINT_32 CCK_SFD_Err;
	UINT_32 OFDM_SIG_Err;
	UINT_32 OFDM_TAG_Err;
	UINT_32 WB_RSSI0;
	UINT_32 IB_RSSI0;
	UINT_32 WB_RSSI1;
	UINT_32 IB_RSSI1;
	UINT_32 PhyMdrdyCCK;
	UINT_32 PhyMdrdyOFDM;
	UINT_32 DriverRxCount;
	UINT_32 RCPI0;
	UINT_32 RCPI1;
	UINT_32 FreqOffsetFromRX;
	UINT_32 RSSI0;
	UINT_32 RSSI1;		   /* insert new member here */
	UINT_32 OutOfResource; /* MT7615 begin here */
	UINT_32 LengthMismatchCount_B0;
	UINT_32 MAC_FCS_Err1; /* b1 */
	UINT_32 MAC_Mdrdy1;	  /* b1 */
	UINT_32 FAGCRssiIBR0;
	UINT_32 FAGCRssiIBR1;
	UINT_32 FAGCRssiIBR2;
	UINT_32 FAGCRssiIBR3;
	UINT_32 FAGCRssiWBR0;
	UINT_32 FAGCRssiWBR1;
	UINT_32 FAGCRssiWBR2;
	UINT_32 FAGCRssiWBR3;

	UINT_32 InstRssiIBR0;
	UINT_32 InstRssiIBR1;
	UINT_32 InstRssiIBR2;
	UINT_32 InstRssiIBR3;
	UINT_32 InstRssiWBR0;
	UINT_32 InstRssiWBR1;
	UINT_32 InstRssiWBR2;
	UINT_32 InstRssiWBR3;
	UINT_32 ACIHitLower;
	UINT_32 ACIHitUpper;
	UINT_32 DriverRxCount1;
	UINT_32 RCPI2;
	UINT_32 RCPI3;
	UINT_32 RSSI2;
	UINT_32 RSSI3;
	UINT_32 SNR0;
	UINT_32 SNR1;
	UINT_32 SNR2;
	UINT_32 SNR3;
	UINT_32 OutOfResource1;
	UINT_32 LengthMismatchCount_B1;
	UINT_32 CCK_PD_Band1;
	UINT_32 OFDM_PD_Band1;
	UINT_32 CCK_SIG_Err_Band1;
	UINT_32 CCK_SFD_Err_Band1;
	UINT_32 OFDM_SIG_Err_Band1;
	UINT_32 OFDM_TAG_Err_Band1;
	UINT_32 PHY_CCK_MDRDY_Band1;
	UINT_32 PHY_OFDM_MDRDY_Band1;
	UINT_32 CCK_FCS_Err_Band1;
	UINT_32 OFDM_FCS_Err_Band1;
	UINT_32 MRURxCount;
	UINT_32 SIGMCS;
	UINT_32 SINR;
	UINT_32 RXVRSSI;
	UINT_32 Reserved[184];
	UINT_32 PHY_Mdrdy;
	UINT_32 Noise_Floor;
	UINT_32 AllLengthMismatchCount_B0;
	UINT_32 AllLengthMismatchCount_B1;
	UINT_32 AllMacMdrdy0;
	UINT_32 AllMacMdrdy1;
	UINT_32 AllFCSErr0;
	UINT_32 AllFCSErr1;
	UINT_32 RXOK0;
	UINT_32 RXOK1;
	UINT_32 PER0;
	UINT_32 PER1;
} PARAM_RX_STAT_T, *P_PARAM_RX_STAT_T;
extern PARAM_RX_STAT_T g_HqaRxStat;

typedef struct _HQA_CMD_FRAME {
	UINT_32 MagicNo;
	UINT_16 Type;
	UINT_16 Id;
	UINT_16 Length;
	UINT_16 Sequence;
	UCHAR	Data[2000];
} __packed HQA_CMD_FRAME;

typedef INT_32 (*HQA_CMD_HANDLER)(
		struct net_device *prNetDev, IN union iwreq_data *prIwReqData, HQA_CMD_FRAME *HqaCmdFrame);

typedef struct _HQA_CMD_TABLE {
	HQA_CMD_HANDLER *CmdSet;
	UINT_32			 CmdSetSize;
	UINT_32			 CmdOffset;
} HQA_CMD_TABLE;

int HQA_CMDHandler(struct net_device *prNetDev, IN union iwreq_data *prIwReqData, HQA_CMD_FRAME *HqaCmdFrame);

int priv_qa_agent(IN struct net_device *prNetDev, IN struct iw_request_info *prIwReqInfo,
		IN union iwreq_data *prIwReqData, IN char *pcExtra);

int priv_set_eeprom_mode(IN UINT_32 u4Mode);
#endif /*CFG_SUPPORT_QA_TOOL */
#endif /* _GL_QA_AGENT_H */
