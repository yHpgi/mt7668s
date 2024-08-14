// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(C) 2016 MediaTek Inc.
 */

/*! \file   p2p_nic.c
 *    \brief  Wi-Fi Direct Functions that provide operation in NIC's (Network
 * Interface Card) point of view.
 *
 *    This file includes functions which unite multiple hal(Hardware) operations
 *    and also take the responsibility of Software Resource Management in order
 *    to keep the synchronization with Hardware Manipulation.
 */

/*******************************************************************************
 *                         C O M P I L E R   F L A G S
 *******************************************************************************
 */

/*******************************************************************************
 *                    E X T E R N A L   R E F E R E N C E S
 *******************************************************************************
 */

#include "precomp.h"

/*******************************************************************************
 *                              C O N S T A N T S
 *******************************************************************************
 */

/*******************************************************************************
 *                             D A T A   T Y P E S
 *******************************************************************************
 */

/*******************************************************************************
 *                            P U B L I C   D A T A
 *******************************************************************************
 */

/*******************************************************************************
 *                           P R I V A T E   D A T A
 *******************************************************************************
 */

/*******************************************************************************
 *                                 M A C R O S
 *******************************************************************************
 */

/*******************************************************************************
 *                   F U N C T I O N   D E C L A R A T I O N S
 *******************************************************************************
 */

/*******************************************************************************
 *                              F U N C T I O N S
 *******************************************************************************
 */

/*----------------------------------------------------------------------------*/
/*!
 * @brief When Probe Rsp & Beacon frame is received and decide a P2P device,
 *        this function will be invoked to buffer scan result
 *
 * @param prAdapter              Pointer to the Adapter structure.
 * @param prEventScanResult      Pointer of EVENT_SCAN_RESULT_T.
 *
 * @return (none)
 */
/*----------------------------------------------------------------------------*/
void nicRxAddP2pDevice(IN P_ADAPTER_T prAdapter,
		       IN P_EVENT_P2P_DEV_DISCOVER_RESULT_T prP2pResult,
		       IN u8 *pucRxIEBuf, IN u16 u2RxIELength)
{
	P_P2P_INFO_T prP2pInfo = (P_P2P_INFO_T)NULL;
	P_EVENT_P2P_DEV_DISCOVER_RESULT_T prTargetResult =
		(P_EVENT_P2P_DEV_DISCOVER_RESULT_T)NULL;
	u32 u4Idx = 0;
	u8 bUpdate = false;

	u8 *pucIeBuf = (u8 *)NULL;
	u16 u2IELength = 0;
	u8 zeroMac[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

	ASSERT(prAdapter);

	prP2pInfo = prAdapter->prP2pInfo;

	for (u4Idx = 0; u4Idx < prP2pInfo->u4DeviceNum; u4Idx++) {
		prTargetResult = &prP2pInfo->arP2pDiscoverResult[u4Idx];

		if (EQUAL_MAC_ADDR(prTargetResult->aucDeviceAddr,
				   prP2pResult->aucDeviceAddr)) {
			bUpdate = true;

			/* Backup OLD buffer result. */
			pucIeBuf = prTargetResult->pucIeBuf;
			u2IELength = prTargetResult->u2IELength;

			/* Update Device Info. */
			/* zero */
			kalMemZero(prTargetResult,
				   sizeof(EVENT_P2P_DEV_DISCOVER_RESULT_T));

			/* then buffer */
			kalMemCopy(prTargetResult, (void *)prP2pResult,
				   sizeof(EVENT_P2P_DEV_DISCOVER_RESULT_T));

			/* See if new IE length is longer or not. */
			if ((u2RxIELength > u2IELength) && (u2IELength != 0)) {
				/* Buffer is not enough. */
				u2RxIELength = u2IELength;
			} else if ((u2IELength == 0) && (u2RxIELength != 0)) {
				/* RX new IE buf. */
				ASSERT(pucIeBuf == NULL);
				pucIeBuf = prP2pInfo->pucCurrIePtr;

				if (((unsigned long)prP2pInfo->pucCurrIePtr +
				     (unsigned long)u2RxIELength) >
				    (unsigned long)&prP2pInfo->aucCommIePool
					    [CFG_MAX_COMMON_IE_BUF_LEN]) {
					/* Common Buffer is no enough. */
					u2RxIELength =
						(u16)((unsigned long)&prP2pInfo->aucCommIePool
							      [CFG_MAX_COMMON_IE_BUF_LEN] -
						      (unsigned long)prP2pInfo
							      ->pucCurrIePtr);
				}

				/* Step to next buffer address. */
				prP2pInfo->pucCurrIePtr =
					(u8 *)((unsigned long)
						       prP2pInfo->pucCurrIePtr +
					       (unsigned long)u2RxIELength);
			}

			/* Restore buffer pointer. */
			prTargetResult->pucIeBuf = pucIeBuf;

			if (pucRxIEBuf) {
				/* If new received IE is available.
				 * Replace the old one & update new IE length.
				 */
				kalMemCopy(pucIeBuf, pucRxIEBuf, u2RxIELength);
				prTargetResult->u2IELength = u2RxIELength;
			} else {
				/* There is no new IE information, keep the old
				 * one. */
				prTargetResult->u2IELength = u2IELength;
			}
		}
	}

	if (!bUpdate) {
		/* We would flush the whole scan result after each scan request
		 * is issued. If P2P device is too many, it may over the scan
		 * list.
		 */
		if ((u4Idx < CFG_MAX_NUM_BSS_LIST) &&
		    (UNEQUAL_MAC_ADDR(zeroMac, prP2pResult->aucDeviceAddr))) {
			prTargetResult = &prP2pInfo->arP2pDiscoverResult[u4Idx];

			/* zero */
			kalMemZero(prTargetResult,
				   sizeof(EVENT_P2P_DEV_DISCOVER_RESULT_T));

			/* then buffer */
			kalMemCopy(prTargetResult, (void *)prP2pResult,
				   sizeof(EVENT_P2P_DEV_DISCOVER_RESULT_T));

			 * prP2pInfo->u4DeviceNum, MAC2STR(prP2pResult->aucDeviceAddr),
			 * MAC2STR(prTargetResult->aucDeviceAddr));
			 * /

				 if (u2RxIELength)
			 {
				 prTargetResult->pucIeBuf =
					 prP2pInfo->pucCurrIePtr;

				 if (((unsigned long)prP2pInfo->pucCurrIePtr +
				      (unsigned long)u2RxIELength) >
				     (unsigned long)&prP2pInfo->aucCommIePool
					     [CFG_MAX_COMMON_IE_BUF_LEN]) {
					 /* Common Buffer is no enough. */
					 u2IELength =
						 (u16)((unsigned long)&prP2pInfo->aucCommIePool
							       [CFG_MAX_COMMON_IE_BUF_LEN] -
						       (unsigned long)prP2pInfo
							       ->pucCurrIePtr);
				 } else {
					 u2IELength = u2RxIELength;
				 }

				 prP2pInfo->pucCurrIePtr =
					 (u8 *)((unsigned long)prP2pInfo
							->pucCurrIePtr +
						(unsigned long)u2IELength);

				 kalMemCopy((void *)prTargetResult->pucIeBuf,
					    (void *)pucRxIEBuf,
					    (u32)u2IELength);
				 prTargetResult->u2IELength = u2IELength;
			 }
			 else
			 {
				 prTargetResult->pucIeBuf = NULL;
				 prTargetResult->u2IELength = 0;
			 }

			 prP2pInfo->u4DeviceNum++;
		} else {
			/* TODO: Fixme to replace an old one. (?) */
			ASSERT(false);
		}
	}
}
