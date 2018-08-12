/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

 {RIL_LOCAL_GSM_UNSOL_CARD_TYPE_NOTIFY, responseString, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_CT3G_DUALMODE_CARD, responseInts, WAKE_PARTIAL},
 /// M: [C2K] MD IRAT feature. @{
 {RIL_LOCAL_GSM_UNSOL_INTER_3GPP_IRAT_STATE_CHANGE, responseIratStateChange, WAKE_PARTIAL},
 /// @}
 {RIL_LOCAL_GSM_UNSOL_CDMA_SMS_SPECIFIC_FROM_GSM, responseString, WAKE_PARTIAL},
/// M:[Network] add for Wifi calling Service. @{
{RIL_LOCAL_GSM_UNSOL_MAL_PS_RGEGISTRATION_STATE, responseInts, WAKE_PARTIAL},
/// @}
 {RIL_LOCAL_GSM_UNSOL_EF_ECC, responseString, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_ENHANCED_MODEM_POWER, responseInts, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_PDN_DEACT_FAILURE_IND, responseInts, WAKE_PARTIAL},

/// M: eMBMS feature
 {RIL_LOCAL_GSM_UNSOL_EMBMS_START_SESSION_RESPONSE, responseLocalEmbmsSessionInfo, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_CELL_INFO_NOTIFICATION, responseInts, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_COVERAGE_STATE, responseInts, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_ACTIVE_SESSION, responseLocalEmbmsSessionNotify, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_AVAILABLE_SESSION, responseLocalEmbmsSessionNotify, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_SAI_LIST_NOTIFICATION, responseLocalEmbmsSaiNotify, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_EMBMS_OOS_NOTIFICATION, responseLocalEmbmsOosNotify, WAKE_PARTIAL},
/// M: eMBMS end

 /// M: Ims Data Framework feature. @{
 {RIL_LOCAL_GSM_UNSOL_DEDICATE_BEARER_ACTIVATED, responseSetupDedicateDataCall, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_DEDICATE_BEARER_MODIFIED, responseSetupDedicateDataCall, WAKE_PARTIAL},
 {RIL_LOCAL_GSM_UNSOL_DEDICATE_BEARER_DEACTIVATED, responseInts, WAKE_PARTIAL},
 /// @}

{RIL_LOCAL_GSM_UNSOL_IMS_CALL_EXIST, responseInts, WAKE_PARTIAL},
{RIL_LOCAL_GSM_UNSOL_ESIMIND_APPLIST, responseInts, WAKE_PARTIAL},