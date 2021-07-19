/*
 * Copyright (c) 2021, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC: wma_eht.c
 *
 * WLAN Host Device Driver 802.11be - Extremely High Throughput Implementation
 */

#include "wma_eht.h"
#include "wmi_unified.h"
#include "service_ready_param.h"
#include "target_if.h"

/**
 * wma_convert_eht_cap() - convert EHT capabilities into dot11f structure
 * @eht_cap: pointer to dot11f structure
 * @mac_cap: Received EHT MAC capability
 * @phy_cap: Received EHT PHY capability
 *
 * This function converts various EHT capability received as part of extended
 * service ready event into dot11f structure.
 *
 * Return: None
 */
static void wma_convert_eht_cap(tDot11fIEeht_cap *eht_cap, uint32_t *mac_cap,
				uint32_t *phy_cap)
{
	eht_cap->present = true;

	/* EHT MAC capabilities */
	eht_cap->nsep_pri_access = WMI_EHTCAP_MAC_NSEPPRIACCESS_GET(mac_cap);
	eht_cap->eht_om_ctl = WMI_EHTCAP_MAC_EHTOMCTRL_GET(mac_cap);
	eht_cap->triggered_txop_sharing = WMI_EHTCAP_MAC_TRIGTXOP_GET(mac_cap);

	/* EHT PHY capabilities */
	eht_cap->support_320mhz_6ghz = WMI_EHTCAP_PHY_320MHZIN6GHZ_GET(phy_cap);
	eht_cap->ru_242tone_wt_20mhz = WMI_EHTCAP_PHY_242TONERUBWLT20MHZ_GET(
			phy_cap);
	eht_cap->ndp_4x_eht_ltf_3dot2_us_gi =
		WMI_EHTCAP_PHY_NDP4XEHTLTFAND320NSGI_GET(phy_cap);
	eht_cap->partial_bw_mu_mimo = WMI_EHTCAP_PHY_PARTIALBWULMU_GET(phy_cap);
	eht_cap->su_beamformer = WMI_EHTCAP_PHY_SUBFMR_GET(phy_cap);
	eht_cap->su_beamformee = WMI_EHTCAP_PHY_SUBFME_GET(phy_cap);
	eht_cap->bfee_ss_le_80mhz = WMI_EHTCAP_PHY_BFMESSLT80MHZ_GET(phy_cap);
	eht_cap->bfee_ss_160mhz = WMI_EHTCAP_PHY_BFMESS160MHZ_GET(phy_cap);
	eht_cap->bfee_ss_320mhz = WMI_EHTCAP_PHY_BFMESS320MHZ_GET(phy_cap);
	eht_cap->num_sounding_dim_le_80mhz = WMI_EHTCAP_PHY_NUMSOUNDLT80MHZ_GET(
			phy_cap);
	eht_cap->num_sounding_dim_160mhz = WMI_EHTCAP_PHY_NUMSOUND160MHZ_GET(
			phy_cap);
	eht_cap->num_sounding_dim_320mhz = WMI_EHTCAP_PHY_NUMSOUND320MHZ_GET(
			phy_cap);
	eht_cap->ng_16_su_feedback = WMI_EHTCAP_PHY_NG16SUFB_GET(phy_cap);
	eht_cap->ng_16_mu_feedback = WMI_EHTCAP_PHY_NG16MUFB_GET(phy_cap);
	eht_cap->cb_sz_4_2_su_feedback = WMI_EHTCAP_PHY_CODBK42SUFB_GET(
			phy_cap);
	eht_cap->cb_sz_7_5_su_feedback = WMI_EHTCAP_PHY_CODBK75MUFB_GET(
			phy_cap);
	eht_cap->trig_su_bforming_feedback = WMI_EHTCAP_PHY_TRIGSUBFFB_GET(
			phy_cap);
	eht_cap->trig_mu_bforming_partial_bw_feedback =
		WMI_EHTCAP_PHY_TRIGMUBFPARTBWFB_GET(phy_cap);
	eht_cap->triggered_cqi_feedback = WMI_EHTCAP_PHY_TRIGCQIFB_GET(phy_cap);
	eht_cap->partial_bw_dl_mu_mimo = WMI_EHTCAP_PHY_PARTBWDLMUMIMO_GET(
			phy_cap);
	eht_cap->psr_based_sr = WMI_EHTCAP_PHY_PSRSR_GET(phy_cap);
	eht_cap->power_boost_factor = WMI_EHTCAP_PHY_PWRBSTFACTOR_GET(phy_cap);
	eht_cap->eht_mu_ppdu_4x_ltf_0_8_us_gi =
		WMI_EHTCAP_PHY_4XEHTLTFAND800NSGI_GET(phy_cap);
	eht_cap->max_nc = WMI_EHTCAP_PHY_MAXNC_GET(phy_cap);
	eht_cap->non_trig_cqi_feedback = WMI_EHTCAP_PHY_NONTRIGCQIFB_GET(
			phy_cap);
	eht_cap->tx_1024_4096_qam_lt_242_tone_ru =
		WMI_EHTCAP_PHY_TX1024AND4096QAMLS242TONERU_GET(phy_cap);
	eht_cap->rx_1024_4096_qam_lt_242_tone_ru =
		WMI_EHTCAP_PHY_RX1024AND4096QAMLS242TONERU_GET(phy_cap);
	eht_cap->ppet_present = WMI_EHTCAP_PHY_PPETHRESPRESENT_GET(phy_cap);
	eht_cap->common_nominal_pkt_padding = WMI_EHTCAP_PHY_CMNNOMPKTPAD_GET(
			phy_cap);
	eht_cap->max_num_eht_ltf = WMI_EHTCAP_PHY_MAXNUMEHTLTF_GET(phy_cap);
	eht_cap->mcs_15 = WMI_EHTCAP_PHY_SUPMCS15_GET(phy_cap);
	eht_cap->eht_dup_6ghz = WMI_EHTCAP_PHY_EHTDUPIN6GHZ_GET(phy_cap);
	eht_cap->op_sta_rx_ndp_wider_bw_20mhz =
		WMI_EHTCAP_PHY_20MHZOPSTARXNDPWIDERBW_GET(phy_cap);
	eht_cap->non_ofdma_ul_mu_mimo_le_80mhz =
		WMI_EHTCAP_PHY_NONOFDMAULMUMIMOLT80MHZ_GET(phy_cap);
	eht_cap->non_ofdma_ul_mu_mimo_160mhz =
		WMI_EHTCAP_PHY_NONOFDMAULMUMIMO160MHZ_GET(phy_cap);
	eht_cap->non_ofdma_ul_mu_mimo_320mhz =
		WMI_EHTCAP_PHY_NONOFDMAULMUMIMO320MHZ_GET(phy_cap);
	eht_cap->mu_bformer_le_80mhz = WMI_EHTCAP_PHY_MUBFMRLT80MHZ_GET(
			phy_cap);
	eht_cap->mu_bformer_160mhz = WMI_EHTCAP_PHY_MUBFMR160MHZ_GET(phy_cap);
	eht_cap->mu_bformer_320mhz = WMI_EHTCAP_PHY_MUBFMR320MHZ_GET(phy_cap);

	/* TODO: MCS map and PPET */
}

void wma_eht_update_tgt_services(struct wmi_unified *wmi_handle,
				 struct wma_tgt_services *cfg)
{
	if (wmi_service_enabled(wmi_handle, wmi_service_11be)) {
		cfg->en_11be = true;
		wma_set_fw_wlan_feat_caps(DOT11BE);
		wma_debug("11be is enabled");
	} else {
		wma_debug("11be is not enabled");
	}
}

void wma_update_target_ext_eht_cap(struct target_psoc_info *tgt_hdl,
				   struct wma_tgt_cfg *tgt_cfg)
{
	tDot11fIEeht_cap *eht_cap = &tgt_cfg->eht_cap;
	tDot11fIEeht_cap *eht_cap_2g = &tgt_cfg->eht_cap_2g;
	tDot11fIEeht_cap *eht_cap_5g = &tgt_cfg->eht_cap_5g;
	int i, num_hw_modes, total_mac_phy_cnt;
	tDot11fIEeht_cap eht_cap_mac;
	struct wlan_psoc_host_mac_phy_caps_ext2 *mac_cap, *mac_phy_cap;
	struct wlan_psoc_host_mac_phy_caps *host_cap;
	uint32_t supported_bands;

	qdf_mem_zero(eht_cap_2g, sizeof(tDot11fIEeht_cap));
	qdf_mem_zero(eht_cap_5g, sizeof(tDot11fIEeht_cap));
	num_hw_modes = target_psoc_get_num_hw_modes(tgt_hdl);
	mac_phy_cap = target_psoc_get_mac_phy_cap_ext2(tgt_hdl);
	host_cap = target_psoc_get_mac_phy_cap(tgt_hdl);
	total_mac_phy_cnt = target_psoc_get_total_mac_phy_cnt(tgt_hdl);
	if (!mac_phy_cap || !host_cap) {
		wma_err("Invalid MAC PHY capabilities handle");
		eht_cap->present = false;
		return;
	}

	if (!num_hw_modes) {
		wma_err("No extended EHT cap for current SOC");
		eht_cap->present = false;
		return;
	}

	if (!tgt_cfg->services.en_11be) {
		wma_info("Target does not support 11BE");
		eht_cap->present = false;
		return;
	}

	supported_bands = host_cap->supported_bands;
	for (i = 0; i < total_mac_phy_cnt; i++) {
		qdf_mem_zero(&eht_cap_mac, sizeof(tDot11fIEeht_cap));
		mac_cap = &mac_phy_cap[i];
		if (supported_bands & WLAN_2G_CAPABILITY) {
			wma_convert_eht_cap(&eht_cap_mac,
					    mac_cap->eht_cap_info_2G,
					    mac_cap->eht_cap_phy_info_2G);
			wma_convert_eht_cap(eht_cap_2g,
					    mac_cap->eht_cap_info_2G,
					    mac_cap->eht_cap_phy_info_2G);
		}

		if (supported_bands & WLAN_5G_CAPABILITY) {
			qdf_mem_zero(&eht_cap_mac, sizeof(tDot11fIEeht_cap));
			wma_convert_eht_cap(&eht_cap_mac,
					    mac_cap->eht_cap_info_5G,
					    mac_cap->eht_cap_phy_info_5G);
			wma_convert_eht_cap(eht_cap_5g,
					    mac_cap->eht_cap_info_5G,
					    mac_cap->eht_cap_phy_info_5G);
		}
	}
	qdf_mem_copy(eht_cap, &eht_cap_mac, sizeof(tDot11fIEeht_cap));
	wma_print_eht_cap(eht_cap);
}

void wma_update_vdev_eht_ops(uint32_t *eht_ops, tDot11fIEeht_op *eht_op)
{
}

void wma_print_eht_cap(tDot11fIEeht_cap *eht_cap)
{
	if (!eht_cap->present)
		return;

	wma_debug("EHT Capabilities:");
}

void wma_print_eht_phy_cap(uint32_t *phy_cap)
{
	wma_debug("EHT PHY Capabilities:");
}

void wma_print_eht_mac_cap_w1(uint32_t mac_cap)
{
	wma_debug("EHT MAC Capabilities:");
}

void wma_print_eht_mac_cap_w2(uint32_t mac_cap)
{
}

void wma_print_eht_op(tDot11fIEeht_op *eht_ops)
{
}

void wma_populate_peer_eht_cap(struct peer_assoc_params *peer,
			       tpAddStaParams params)
{
	tDot11fIEeht_cap *eht_cap = &params->eht_config;
	uint32_t *phy_cap = peer->peer_eht_cap_phyinfo;
	uint32_t mac_cap[PSOC_HOST_MAX_MAC_SIZE] = {0};

	if (params->eht_capable)
		peer->eht_flag = 1;
	else
		return;

	qdf_mem_copy(peer->peer_eht_cap_macinfo, peer->peer_he_cap_macinfo,
		     sizeof(mac_cap));
	qdf_mem_copy(peer->peer_he_cap_phyinfo, peer->peer_he_cap_phyinfo,
		     sizeof(peer->peer_he_cap_phyinfo));

	qdf_mem_copy(peer->peer_eht_rx_mcs_set, peer->peer_he_rx_mcs_set,
		     sizeof(peer->peer_he_rx_mcs_set));
	qdf_mem_copy(peer->peer_eht_tx_mcs_set, peer->peer_he_tx_mcs_set,
		     sizeof(peer->peer_he_tx_mcs_set));

	peer->peer_eht_mcs_count = peer->peer_he_mcs_count;

	wma_print_eht_cap(eht_cap);
	wma_debug("Peer EHT Capabilities:");
	wma_print_eht_phy_cap(phy_cap);
	wma_print_eht_mac_cap_w1(mac_cap[0]);
	wma_print_eht_mac_cap_w2(mac_cap[1]);
}

void wma_vdev_set_eht_bss_params(tp_wma_handle wma, uint8_t vdev_id,
				 struct vdev_mlme_eht_ops_info *eht_info)
{
	if (!eht_info->eht_ops)
		return;
}

QDF_STATUS wma_get_eht_capabilities(struct eht_capability *eht_cap)
{
	tp_wma_handle wma_handle;

	wma_handle = cds_get_context(QDF_MODULE_ID_WMA);
	if (!wma_handle)
		return QDF_STATUS_E_FAILURE;

	qdf_mem_copy(eht_cap->phy_cap,
		     &wma_handle->eht_cap.phy_cap,
		     WMI_MAX_EHTCAP_PHY_SIZE);
	eht_cap->mac_cap = wma_handle->eht_cap.mac_cap;
	return QDF_STATUS_SUCCESS;
}

void wma_set_peer_assoc_params_bw_320(struct peer_assoc_params *params,
				      enum phy_ch_width ch_width)
{
	if (ch_width == CH_WIDTH_320MHZ)
		params->bw_320 = 1;
}
