// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 * Copyright (c) 2024, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/device.h>
#include <linux/interconnect.h>
#include <linux/interconnect-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <dt-bindings/interconnect/qcom,sm7150-rpmh.h>

#include "bcm-voter.h"
#include "icc-rpmh.h"
#include "sm7150.h"

static struct qcom_icc_node qhm_a1noc_cfg = {
	.name = "qhm-a1noc-cfg",
	.id = SM7150_MASTER_A1NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_SERVICE_A1NOC },
};

static struct qcom_icc_node qhm_qup_center = {
	.name = "qhm_qup_center",
	.id = SM7150_MASTER_QUP_0,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node qhm_tsif = {
	.name = "qhm_tsif",
	.id = SM7150_MASTER_TSIF,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_emmc = {
	.name = "xm_emmc",
	.id = SM7150_MASTER_EMMC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_sdc2 = {
	.name = "xm_sdc2",
	.id = SM7150_MASTER_SDCC_2,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_sdc4 = {
	.name = "xm_sdc4",
	.id = SM7150_MASTER_SDCC_4,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_ufs_mem = {
	.name = "xm_ufs_mem",
	.id = SM7150_MASTER_UFS_MEM,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_SLV },
};

static struct qcom_icc_node qhm_a2noc_cfg = {
	.name = "qhm_a2noc_cfg",
	.id = SM7150_MASTER_A2NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_SERVICE_A2NOC },
};

static struct qcom_icc_node qhm_qdss_bam = {
	.name = "qhm_qdss_bam",
	.id = SM7150_MASTER_QDSS_BAM,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node qhm_qup_north = {
	.name = "qhm_qup_north",
	.id = SM7150_MASTER_QUP_1,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node qnm_cnoc = {
	.name = "qnm_cnoc",
	.id = SM7150_MASTER_CNOC_A2NOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node qxm_crypto = {
	.name = "qxm_crypto",
	.id = SM7150_MASTER_CRYPTO_CORE_0,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node qxm_ipa = {
	.name = "qxm_ipa",
	.id = SM7150_MASTER_IPA,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_pcie3_0 = {
	.name = "xm_pcie3_0",
	.id = SM7150_MASTER_PCIE,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_SLAVE_ANOC_PCIE_GEM_NOC },
};

static struct qcom_icc_node xm_qdss_etr = {
	.name = "xm_qdss_etr",
	.id = SM7150_MASTER_QDSS_ETR,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node xm_usb3_0 = {
	.name = "xm_usb3_0",
	.id = SM7150_MASTER_USB3,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_SLV },
};

static struct qcom_icc_node qxm_camnoc_hf0_uncomp = {
	.name = "qxm_camnoc_hf0_uncomp",
	.id = SM7150_MASTER_CAMNOC_HF0_UNCOMP,
	.channels = 2,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_CAMNOC_UNCOMP },
};

static struct qcom_icc_node qxm_camnoc_rt_uncomp = {
	.name = "qxm_camnoc_rt_uncomp",
	.id = SM7150_MASTER_CAMNOC_RT_UNCOMP,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_CAMNOC_UNCOMP },
};

static struct qcom_icc_node qxm_camnoc_sf_uncomp = {
	.name = "qxm_camnoc_sf_uncomp",
	.id = SM7150_MASTER_CAMNOC_SF_UNCOMP,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_CAMNOC_UNCOMP },
};

static struct qcom_icc_node qxm_camnoc_nrt_uncomp = {
	.name = "qxm_camnoc_nrt_uncomp",
	.id = SM7150_MASTER_CAMNOC_NRT_UNCOMP,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_CAMNOC_UNCOMP },
};

static struct qcom_icc_node qnm_npu = {
	.name = "qnm_npu",
	.id = SM7150_MASTER_NPU,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_CDSP_GEM_NOC },
};

static struct qcom_icc_node qhm_spdm = {
	.name = "qhm_spdm",
	.id = SM7150_MASTER_SPDM,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_CNOC_A2NOC },
};

static struct qcom_icc_node qnm_snoc = {
	.name = "qnm_snoc",
	.id = SM7150_SNOC_CNOC_MAS,
	.channels = 1,
	.buswidth = 8,
	.num_links = 47,
	.links = { SM7150_SLAVE_TLMM_SOUTH,
		   SM7150_SLAVE_CAMERA_CFG,
		   SM7150_SLAVE_SDCC_4,
		   SM7150_SLAVE_SDCC_2,
		   SM7150_SLAVE_CNOC_MNOC_CFG,
		   SM7150_SLAVE_UFS_MEM_CFG,
		   SM7150_SLAVE_QUP_0,
		   SM7150_SLAVE_GLM,
		   SM7150_SLAVE_PDM,
		   SM7150_SLAVE_CAMERA_NRT_THROTTLE_CFG,
		   SM7150_SLAVE_A2NOC_CFG,
		   SM7150_SLAVE_QDSS_CFG,
		   SM7150_SLAVE_CAMERA_RT_THROTTLE_CFG,
		   SM7150_SLAVE_DISPLAY_CFG,
		   SM7150_SLAVE_PCIE_CFG,
		   SM7150_SLAVE_DISPLAY_THROTTLE_CFG,
		   SM7150_SLAVE_TCSR,
		   SM7150_SLAVE_VENUS_CVP_THROTTLE_CFG,
		   SM7150_SLAVE_CNOC_DDRSS,
		   SM7150_SLAVE_AHB2PHY_NORTH,
		   SM7150_SLAVE_SNOC_CFG,
		   SM7150_SLAVE_GRAPHICS_3D_CFG,
		   SM7150_SLAVE_VENUS_CFG,
		   SM7150_SLAVE_TSIF,
		   SM7150_SLAVE_CDSP_CFG,
		   SM7150_SLAVE_CLK_CTL,
		   SM7150_SLAVE_AOP,
		   SM7150_SLAVE_QUP_1,
		   SM7150_SLAVE_AHB2PHY_SOUTH,
		   SM7150_SLAVE_SERVICE_CNOC,
		   SM7150_SLAVE_AHB2PHY_WEST,
		   SM7150_SLAVE_USB3,
		   SM7150_SLAVE_VENUS_THROTTLE_CFG,
		   SM7150_SLAVE_IPA_CFG,
		   SM7150_SLAVE_RBCPR_CX_CFG,
		   SM7150_SLAVE_TLMM_WEST,
		   SM7150_SLAVE_A1NOC_CFG,
		   SM7150_SLAVE_AOSS,
		   SM7150_SLAVE_PRNG,
		   SM7150_SLAVE_VSENSE_CTRL_CFG,
		   SM7150_SLAVE_EMMC_CFG,
		   SM7150_SLAVE_SPDM_WRAPPER,
		   SM7150_SLAVE_CRYPTO_0_CFG,
		   SM7150_SLAVE_PIMEM_CFG,
		   SM7150_SLAVE_TLMM_NORTH,
		   SM7150_SLAVE_RBCPR_MX_CFG,
		   SM7150_SLAVE_IMEM_CFG
	},
};

static struct qcom_icc_node xm_qdss_dap = {
	.name = "xm_qdss_dap",
	.id = SM7150_MASTER_QDSS_DAP,
	.channels = 1,
	.buswidth = 8,
	.num_links = 48,
	.links = { SM7150_SLAVE_TLMM_SOUTH,
		   SM7150_SLAVE_CAMERA_CFG,
		   SM7150_SLAVE_SDCC_4,
		   SM7150_SLAVE_SDCC_2,
		   SM7150_SLAVE_CNOC_MNOC_CFG,
		   SM7150_SLAVE_UFS_MEM_CFG,
		   SM7150_SLAVE_QUP_0,
		   SM7150_SLAVE_GLM,
		   SM7150_SLAVE_PDM,
		   SM7150_SLAVE_CAMERA_NRT_THROTTLE_CFG,
		   SM7150_SLAVE_A2NOC_CFG,
		   SM7150_SLAVE_QDSS_CFG,
		   SM7150_SLAVE_CAMERA_RT_THROTTLE_CFG,
		   SM7150_SLAVE_DISPLAY_CFG,
		   SM7150_SLAVE_PCIE_CFG,
		   SM7150_SLAVE_DISPLAY_THROTTLE_CFG,
		   SM7150_SLAVE_TCSR,
		   SM7150_SLAVE_VENUS_CVP_THROTTLE_CFG,
		   SM7150_SLAVE_CNOC_DDRSS,
		   SM7150_SLAVE_CNOC_A2NOC,
		   SM7150_SLAVE_AHB2PHY_NORTH,
		   SM7150_SLAVE_SNOC_CFG,
		   SM7150_SLAVE_GRAPHICS_3D_CFG,
		   SM7150_SLAVE_VENUS_CFG,
		   SM7150_SLAVE_TSIF,
		   SM7150_SLAVE_CDSP_CFG,
		   SM7150_SLAVE_CLK_CTL,
		   SM7150_SLAVE_AOP,
		   SM7150_SLAVE_QUP_1,
		   SM7150_SLAVE_AHB2PHY_SOUTH,
		   SM7150_SLAVE_SERVICE_CNOC,
		   SM7150_SLAVE_AHB2PHY_WEST,
		   SM7150_SLAVE_USB3,
		   SM7150_SLAVE_VENUS_THROTTLE_CFG,
		   SM7150_SLAVE_IPA_CFG,
		   SM7150_SLAVE_RBCPR_CX_CFG,
		   SM7150_SLAVE_TLMM_WEST,
		   SM7150_SLAVE_A1NOC_CFG,
		   SM7150_SLAVE_AOSS,
		   SM7150_SLAVE_PRNG,
		   SM7150_SLAVE_VSENSE_CTRL_CFG,
		   SM7150_SLAVE_EMMC_CFG,
		   SM7150_SLAVE_SPDM_WRAPPER,
		   SM7150_SLAVE_CRYPTO_0_CFG,
		   SM7150_SLAVE_PIMEM_CFG,
		   SM7150_SLAVE_TLMM_NORTH,
		   SM7150_SLAVE_RBCPR_MX_CFG,
		   SM7150_SLAVE_IMEM_CFG
	},
};

static struct qcom_icc_node qhm_cnoc_dc_noc = {
	.name = "qhm_cnoc_dc_noc",
	.id = SM7150_MASTER_CNOC_DC_NOC,
	.channels = 1,
	.buswidth = 4,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC_CFG,
		   SM7150_SLAVE_GEM_NOC_CFG
	},
};

static struct qcom_icc_node acm_apps = {
	.name = "acm_apps",
	.id = SM7150_MASTER_AMPSS_M0,
	.channels = 1,
	.buswidth = 16,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node acm_sys_tcu = {
	.name = "acm_sys_tcu",
	.id = SM7150_MASTER_SYS_TCU,
	.channels = 1,
	.buswidth = 8,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node qhm_gemnoc_cfg = {
	.name = "qhm_gemnoc_cfg",
	.id = SM7150_MASTER_GEM_NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 2,
	.links = { SM7150_SLAVE_SERVICE_GEM_NOC,
		   SM7150_SLAVE_MSS_PROC_MS_MPU_CFG
	},
};

static struct qcom_icc_node qnm_cmpnoc = {
	.name = "qnm_cmpnoc",
	.id = SM7150_MASTER_COMPUTE_NOC,
	.channels = 1,
	.buswidth = 32,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node qnm_mnoc_hf = {
	.name = "qnm_mnoc_hf",
	.id = SM7150_MASTER_MNOC_HF_MEM_NOC,
	.channels = 2,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_LLCC },
};

static struct qcom_icc_node qnm_mnoc_sf = {
	.name = "qnm_mnoc_sf",
	.id = SM7150_MASTER_MNOC_SF_MEM_NOC,
	.channels = 1,
	.buswidth = 32,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node qnm_pcie = {
	.name = "qnm_pcie",
	.id = SM7150_MASTER_GEM_NOC_PCIE_SNOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node qnm_snoc_gc = {
	.name = "qnm_snoc_gc",
	.id = SM7150_MASTER_SNOC_GC_MEM_NOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_SLAVE_LLCC },
};

static struct qcom_icc_node qnm_snoc_sf = {
	.name = "qnm_snoc_sf",
	.id = SM7150_MASTER_SNOC_SF_MEM_NOC,
	.channels = 1,
	.buswidth = 16,
	.num_links = 1,
	.links = { SM7150_SLAVE_LLCC },
};

static struct qcom_icc_node qxm_gpu = {
	.name = "qxm_gpu",
	.id = SM7150_MASTER_GRAPHICS_3D,
	.channels = 2,
	.buswidth = 32,
	.num_links = 2,
	.links = { SM7150_SLAVE_LLCC,
		   SM7150_SLAVE_GEM_NOC_SNOC
	},
};

static struct qcom_icc_node llcc_mc = {
	.name = "llcc_mc",
	.id = SM7150_MASTER_LLCC,
	.channels = 2,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_EBI_CH0 },
};

static struct qcom_icc_node qhm_mnoc_cfg = {
	.name = "qhm_mnoc_cfg",
	.id = SM7150_MASTER_CNOC_MNOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_SERVICE_MNOC },
};

static struct qcom_icc_node qxm_camnoc_hf = {
	.name = "qxm_camnoc_hf",
	.id = SM7150_MASTER_CAMNOC_HF0,
	.channels = 2,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_HF_MEM_NOC },
};

static struct qcom_icc_node qxm_camnoc_nrt = {
	.name = "qxm_camnoc_nrt",
	.id = SM7150_MASTER_CAMNOC_NRT,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxm_camnoc_rt = {
	.name = "qxm_camnoc_rt",
	.id = SM7150_MASTER_CAMNOC_RT,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_HF_MEM_NOC },
};

static struct qcom_icc_node qxm_camnoc_sf = {
	.name = "qxm_camnoc_sf",
	.id = SM7150_MASTER_CAMNOC_SF,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxm_mdp0 = {
	.name = "qxm_mdp0",
	.id = SM7150_MASTER_MDP_PORT0,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_HF_MEM_NOC },
};

static struct qcom_icc_node qxm_mdp1 = {
	.name = "qxm_mdp1",
	.id = SM7150_MASTER_MDP_PORT1,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_HF_MEM_NOC },
};

static struct qcom_icc_node qxm_rot = {
	.name = "qxm_rot",
	.id = SM7150_MASTER_ROTATOR,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxm_venus0 = {
	.name = "qxm_venus0",
	.id = SM7150_MASTER_VIDEO_P0,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxm_venus1 = {
	.name = "qxm_venus1",
	.id = SM7150_MASTER_VIDEO_P1,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxm_venus_arm9 = {
	.name = "qxm_venus_arm9",
	.id = SM7150_MASTER_VIDEO_PROC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_SLAVE_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qhm_snoc_cfg = {
	.name = "qhm_snoc_cfg",
	.id = SM7150_MASTER_SNOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_SLAVE_SERVICE_SNOC },
};

static struct qcom_icc_node qnm_aggre1_noc = {
	.name = "qnm_aggre1_noc",
	.id = SM7150_A1NOC_SNOC_MAS,
	.channels = 1,
	.buswidth = 16,
	.num_links = 6,
	.links = { SM7150_SLAVE_SNOC_GEM_NOC_SF,
		   SM7150_SLAVE_PIMEM,
		   SM7150_SLAVE_OCIMEM,
		   SM7150_SLAVE_APPSS,
		   SM7150_SNOC_CNOC_SLV,
		   SM7150_SLAVE_QDSS_STM
	},
};

static struct qcom_icc_node qnm_aggre2_noc = {
	.name = "qnm_aggre2_noc",
	.id = SM7150_A2NOC_SNOC_MAS,
	.channels = 1,
	.buswidth = 16,
	.num_links = 7,
	.links = { SM7150_SLAVE_SNOC_GEM_NOC_SF,
		   SM7150_SLAVE_PIMEM,
		   SM7150_SLAVE_OCIMEM,
		   SM7150_SLAVE_APPSS,
		   SM7150_SNOC_CNOC_SLV,
		   SM7150_SLAVE_TCU,
		   SM7150_SLAVE_QDSS_STM
	},
};

static struct qcom_icc_node qnm_gemnoc = {
	.name = "qnm_gemnoc",
	.id = SM7150_MASTER_GEM_NOC_SNOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 6,
	.links = { SM7150_SLAVE_PIMEM,
		   SM7150_SLAVE_OCIMEM,
		   SM7150_SLAVE_APPSS,
		   SM7150_SNOC_CNOC_SLV,
		   SM7150_SLAVE_TCU,
		   SM7150_SLAVE_QDSS_STM
	},
};

static struct qcom_icc_node qxm_pimem = {
	.name = "qxm_pimem",
	.id = SM7150_MASTER_PIMEM,
	.channels = 1,
	.buswidth = 8,
	.num_links = 2,
	.links = { SM7150_SLAVE_SNOC_GEM_NOC_GC,
		   SM7150_SLAVE_OCIMEM
	},
};

static struct qcom_icc_node xm_gic = {
	.name = "xm_gic",
	.id = SM7150_MASTER_GIC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 2,
	.links = { SM7150_SLAVE_SNOC_GEM_NOC_GC,
		   SM7150_SLAVE_OCIMEM
	},
};

static struct qcom_icc_node qns_a1noc_snoc = {
	.name = "qns_a1noc_snoc",
	.id = SM7150_A1NOC_SNOC_SLV,
	.channels = 1,
	.buswidth = 16,
	.num_links = 1,
	.links = { SM7150_A1NOC_SNOC_MAS },
};

static struct qcom_icc_node srvc_aggre1_noc = {
	.name = "srvc_aggre1_noc",
	.id = SM7150_SLAVE_SERVICE_A1NOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qns_a2noc_snoc = {
	.name = "qns_a2noc_snoc",
	.id = SM7150_A2NOC_SNOC_SLV,
	.channels = 1,
	.buswidth = 16,
	.num_links = 1,
	.links = { SM7150_A2NOC_SNOC_MAS },
};

static struct qcom_icc_node qns_pcie_gemnoc = {
	.name = "qns_pcie_gemnoc",
	.id = SM7150_SLAVE_ANOC_PCIE_GEM_NOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_MASTER_GEM_NOC_PCIE_SNOC },
};

static struct qcom_icc_node srvc_aggre2_noc = {
	.name = "srvc_aggre2_noc",
	.id = SM7150_SLAVE_SERVICE_A2NOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qns_camnoc_uncomp = {
	.name = "qns_camnoc_uncomp",
	.id = SM7150_SLAVE_CAMNOC_UNCOMP,
	.channels = 1,
	.buswidth = 32,
};

static struct qcom_icc_node qns_cdsp_gemnoc = {
	.name = "qns_cdsp_gemnoc",
	.id = SM7150_SLAVE_CDSP_GEM_NOC,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_MASTER_COMPUTE_NOC },
};

static struct qcom_icc_node qhs_a1_noc_cfg = {
	.name = "qhs_a1_noc_cfg",
	.id = SM7150_SLAVE_A1NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_A1NOC_CFG },
};

static struct qcom_icc_node qhs_a2_noc_cfg = {
	.name = "qhs_a2_noc_cfg",
	.id = SM7150_SLAVE_A2NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_A2NOC_CFG },
};

static struct qcom_icc_node qhs_ahb2phy_north = {
	.name = "qhs_ahb2phy_north",
	.id = SM7150_SLAVE_AHB2PHY_NORTH,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_ahb2phy_south = {
	.name = "qhs_ahb2phy_south",
	.id = SM7150_SLAVE_AHB2PHY_SOUTH,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_ahb2phy_west = {
	.name = "qhs_ahb2phy_west",
	.id = SM7150_SLAVE_AHB2PHY_WEST,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_aop = {
	.name = "qhs_aop",
	.id = SM7150_SLAVE_AOP,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_aoss = {
	.name = "qhs_aoss",
	.id = SM7150_SLAVE_AOSS,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_camera_cfg = {
	.name = "qhs_camera_cfg",
	.id = SM7150_SLAVE_CAMERA_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_camera_nrt_thrott_cfg = {
	.name = "qhs_camera_nrt_thrott_cfg",
	.id = SM7150_SLAVE_CAMERA_NRT_THROTTLE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_camera_rt_throttle_cfg = {
	.name = "qhs_camera_rt_throttle_cfg",
	.id = SM7150_SLAVE_CAMERA_RT_THROTTLE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_clk_ctl = {
	.name = "qhs_clk_ctl",
	.id = SM7150_SLAVE_CLK_CTL,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_compute_dsp_cfg = {
	.name = "qhs_compute_dsp_cfg",
	.id = SM7150_SLAVE_CDSP_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_cpr_cx = {
	.name = "qhs_cpr_cx",
	.id = SM7150_SLAVE_RBCPR_CX_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_cpr_mx = {
	.name = "qhs_cpr_mx",
	.id = SM7150_SLAVE_RBCPR_MX_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_crypto0_cfg = {
	.name = "qhs_crypto0_cfg",
	.id = SM7150_SLAVE_CRYPTO_0_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_ddrss_cfg = {
	.name = "qhs_ddrss_cfg",
	.id = SM7150_SLAVE_CNOC_DDRSS,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_CNOC_DC_NOC },
};

static struct qcom_icc_node qhs_display_cfg = {
	.name = "qhs_display_cfg",
	.id = SM7150_SLAVE_DISPLAY_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_display_throttle_cfg = {
	.name = "qhs_display_throttle_cfg",
	.id = SM7150_SLAVE_DISPLAY_THROTTLE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_emmc_cfg = {
	.name = "qhs_emmc_cfg",
	.id = SM7150_SLAVE_EMMC_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_glm = {
	.name = "qhs_glm",
	.id = SM7150_SLAVE_GLM,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_gpuss_cfg = {
	.name = "qhs_gpuss_cfg",
	.id = SM7150_SLAVE_GRAPHICS_3D_CFG,
	.channels = 1,
	.buswidth = 8,
};

static struct qcom_icc_node qhs_imem_cfg = {
	.name = "qhs_imem_cfg",
	.id = SM7150_SLAVE_IMEM_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_ipa = {
	.name = "qhs_ipa",
	.id = SM7150_SLAVE_IPA_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_mnoc_cfg = {
	.name = "qhs_mnoc_cfg",
	.id = SM7150_SLAVE_CNOC_MNOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_CNOC_MNOC_CFG },
};

static struct qcom_icc_node qhs_pcie_cfg = {
	.name = "qhs_pcie_cfg",
	.id = SM7150_SLAVE_PCIE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_pdm = {
	.name = "qhs_pdm",
	.id = SM7150_SLAVE_PDM,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_pimem_cfg = {
	.name = "qhs_pimem_cfg",
	.id = SM7150_SLAVE_PIMEM_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_prng = {
	.name = "qhs_prng",
	.id = SM7150_SLAVE_PRNG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_qdss_cfg = {
	.name = "qhs_qdss_cfg",
	.id = SM7150_SLAVE_QDSS_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_qupv3_center = {
	.name = "qhs_qupv3_center",
	.id = SM7150_SLAVE_QUP_0,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_qupv3_north = {
	.name = "qhs_qupv3_north",
	.id = SM7150_SLAVE_QUP_1,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_sdc2 = {
	.name = "qhs_sdc2",
	.id = SM7150_SLAVE_SDCC_2,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_sdc4 = {
	.name = "qhs_sdc4",
	.id = SM7150_SLAVE_SDCC_4,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_snoc_cfg = {
	.name = "qhs_snoc_cfg",
	.id = SM7150_SLAVE_SNOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_SNOC_CFG },
};

static struct qcom_icc_node qhs_spdm = {
	.name = "qhs_spdm",
	.id = SM7150_SLAVE_SPDM_WRAPPER,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_tcsr = {
	.name = "qhs_tcsr",
	.id = SM7150_SLAVE_TCSR,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_tlmm_north = {
	.name = "qhs_tlmm_north",
	.id = SM7150_SLAVE_TLMM_NORTH,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_tlmm_south = {
	.name = "qhs_tlmm_south",
	.id = SM7150_SLAVE_TLMM_SOUTH,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_tlmm_west = {
	.name = "qhs_tlmm_west",
	.id = SM7150_SLAVE_TLMM_WEST,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_tsif = {
	.name = "qhs_tsif",
	.id = SM7150_SLAVE_TSIF,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_ufs_mem_cfg = {
	.name = "qhs_ufs_mem_cfg",
	.id = SM7150_SLAVE_UFS_MEM_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_usb3_0 = {
	.name = "qhs_usb3_0",
	.id = SM7150_SLAVE_USB3,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_venus_cfg = {
	.name = "qhs_venus_cfg",
	.id = SM7150_SLAVE_VENUS_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_venus_cvp_throttle_cfg = {
	.name = "qhs_venus_cvp_throttle_cfg",
	.id = SM7150_SLAVE_VENUS_CVP_THROTTLE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_venus_throttle_cfg = {
	.name = "qhs_venus_throttle_cfg",
	.id = SM7150_SLAVE_VENUS_THROTTLE_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_vsense_ctrl_cfg = {
	.name = "qhs_vsense_ctrl_cfg",
	.id = SM7150_SLAVE_VSENSE_CTRL_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qns_cnoc_a2noc = {
	.name = "qns_cnoc_a2noc",
	.id = SM7150_SLAVE_CNOC_A2NOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_MASTER_CNOC_A2NOC },
};

static struct qcom_icc_node srvc_cnoc = {
	.name = "srvc_cnoc",
	.id = SM7150_SLAVE_SERVICE_CNOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_gemnoc = {
	.name = "qhs_gemnoc",
	.id = SM7150_SLAVE_GEM_NOC_CFG,
	.channels = 1,
	.buswidth = 4,
	.num_links = 1,
	.links = { SM7150_MASTER_GEM_NOC_CFG },
};

static struct qcom_icc_node qhs_llcc = {
	.name = "qhs_llcc",
	.id = SM7150_SLAVE_LLCC_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_mdsp_ms_mpu_cfg = {
	.name = "qhs_mdsp_ms_mpu_cfg",
	.id = SM7150_SLAVE_MSS_PROC_MS_MPU_CFG,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qns_gem_noc_snoc = {
	.name = "qns_gem_noc_snoc",
	.id = SM7150_SLAVE_GEM_NOC_SNOC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_MASTER_GEM_NOC_SNOC },
};

static struct qcom_icc_node qns_llcc = {
	.name = "qns_llcc",
	.id = SM7150_SLAVE_LLCC,
	.channels = 2,
	.buswidth = 16,
	.num_links = 1,
	.links = { SM7150_MASTER_LLCC },
};

static struct qcom_icc_node srvc_gemnoc = {
	.name = "srvc_gemnoc",
	.id = SM7150_SLAVE_SERVICE_GEM_NOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node ebi = {
	.name = "ebi",
	.id = SM7150_SLAVE_EBI_CH0,
	.channels = 2,
	.buswidth = 4,
};

static struct qcom_icc_node qns2_mem_noc = {
	.name = "qns2_mem_noc",
	.id = SM7150_SLAVE_MNOC_SF_MEM_NOC,
	.channels = 1,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_MASTER_MNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qns_mem_noc_hf = {
	.name = "qns_mem_noc_hf",
	.id = SM7150_SLAVE_MNOC_HF_MEM_NOC,
	.channels = 2,
	.buswidth = 32,
	.num_links = 1,
	.links = { SM7150_MASTER_MNOC_HF_MEM_NOC },
};

static struct qcom_icc_node srvc_mnoc = {
	.name = "srvc_mnoc",
	.id = SM7150_SLAVE_SERVICE_MNOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node qhs_apss = {
	.name = "qhs_apss",
	.id = SM7150_SLAVE_APPSS,
	.channels = 1,
	.buswidth = 8,
};

static struct qcom_icc_node qns_cnoc = {
	.name = "qns_cnoc",
	.id = SM7150_SNOC_CNOC_SLV,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_SNOC_CNOC_MAS },
};

static struct qcom_icc_node qns_gemnoc_gc = {
	.name = "qns_gemnoc_gc",
	.id = SM7150_SLAVE_SNOC_GEM_NOC_GC,
	.channels = 1,
	.buswidth = 8,
	.num_links = 1,
	.links = { SM7150_MASTER_SNOC_GC_MEM_NOC },
};

static struct qcom_icc_node qns_gemnoc_sf = {
	.name = "qns_gemnoc_sf",
	.id = SM7150_SLAVE_SNOC_GEM_NOC_SF,
	.channels = 1,
	.buswidth = 16,
	.num_links = 1,
	.links = { SM7150_MASTER_SNOC_SF_MEM_NOC },
};

static struct qcom_icc_node qxs_imem = {
	.name = "qxs_imem",
	.id = SM7150_SLAVE_OCIMEM,
	.channels = 1,
	.buswidth = 8,
};

static struct qcom_icc_node qxs_pimem = {
	.name = "qxs_pimem",
	.id = SM7150_SLAVE_PIMEM,
	.channels = 1,
	.buswidth = 8,
};

static struct qcom_icc_node srvc_snoc = {
	.name = "srvc_snoc",
	.id = SM7150_SLAVE_SERVICE_SNOC,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node xs_qdss_stm = {
	.name = "xs_qdss_stm",
	.id = SM7150_SLAVE_QDSS_STM,
	.channels = 1,
	.buswidth = 4,
};

static struct qcom_icc_node xs_sys_tcu_cfg = {
	.name = "xs_sys_tcu_cfg",
	.id = SM7150_SLAVE_TCU,
	.channels = 1,
	.buswidth = 8,
};

static struct qcom_icc_bcm bcm_acv = {
	.name = "ACV",
	.enable_mask = BIT(3),
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &ebi },
};

static struct qcom_icc_bcm bcm_mc0 = {
	.name = "MC0",
	.keepalive = true,
	.num_nodes = 1,
	.nodes = { &ebi },
};

static struct qcom_icc_bcm bcm_sh0 = {
	.name = "SH0",
	.keepalive = true,
	.num_nodes = 1,
	.nodes = { &qns_llcc },
};

static struct qcom_icc_bcm bcm_mm0 = {
	.name = "MM0",
	.keepalive = true,
	.num_nodes = 1,
	.nodes = { &qns_mem_noc_hf },
};

static struct qcom_icc_bcm bcm_mm1 = {
	.name = "MM1",
	.keepalive = true,
	.num_nodes = 8,
	.nodes = { &qxm_camnoc_hf0_uncomp,
		   &qxm_camnoc_rt_uncomp,
		   &qxm_camnoc_sf_uncomp,
		   &qxm_camnoc_nrt_uncomp,
		   &qxm_camnoc_hf,
		   &qxm_camnoc_rt,
		   &qxm_mdp0,
		   &qxm_mdp1
	},
};

static struct qcom_icc_bcm bcm_sh2 = {
	.name = "SH2",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qns_gem_noc_snoc },
};

static struct qcom_icc_bcm bcm_sh3 = {
	.name = "SH3",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &acm_sys_tcu },
};

static struct qcom_icc_bcm bcm_mm2 = {
	.name = "MM2",
	.keepalive = false,
	.num_nodes = 2,
	.nodes = { &qxm_camnoc_nrt,
		   &qns2_mem_noc
	},
};

static struct qcom_icc_bcm bcm_mm3 = {
	.name = "MM3",
	.keepalive = false,
	.num_nodes = 5,
	.nodes = { &qxm_camnoc_sf,
		   &qxm_rot,
		   &qxm_venus0,
		   &qxm_venus1,
		   &qxm_venus_arm9
	},
};

static struct qcom_icc_bcm bcm_sh5 = {
	.name = "SH5",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &acm_apps },
};

static struct qcom_icc_bcm bcm_sn0 = {
	.name = "SN0",
	.keepalive = true,
	.num_nodes = 1,
	.nodes = { &qns_gemnoc_sf },
};

static struct qcom_icc_bcm bcm_sh8 = {
	.name = "SH8",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qns_cdsp_gemnoc },
};

static struct qcom_icc_bcm bcm_sh10 = {
	.name = "SH10",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qnm_npu },
};

static struct qcom_icc_bcm bcm_ce0 = {
	.name = "CE0",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qxm_crypto },
};

static struct qcom_icc_bcm bcm_cn0 = {
	.name = "CN0",
	.keepalive = true,
	.num_nodes = 54,
	.nodes = { &qhm_tsif,
		   &xm_emmc,
		   &xm_sdc2,
		   &xm_sdc4,
		   &qhm_spdm,
		   &qnm_snoc,
		   &qhs_a1_noc_cfg,
		   &qhs_a2_noc_cfg,
		   &qhs_ahb2phy_north,
		   &qhs_ahb2phy_south,
		   &qhs_ahb2phy_west,
		   &qhs_aop,
		   &qhs_aoss,
		   &qhs_camera_cfg,
		   &qhs_camera_nrt_thrott_cfg,
		   &qhs_camera_rt_throttle_cfg,
		   &qhs_clk_ctl,
		   &qhs_compute_dsp_cfg,
		   &qhs_cpr_cx,
		   &qhs_cpr_mx,
		   &qhs_crypto0_cfg,
		   &qhs_ddrss_cfg,
		   &qhs_display_cfg,
		   &qhs_display_throttle_cfg,
		   &qhs_emmc_cfg,
		   &qhs_glm,
		   &qhs_gpuss_cfg,
		   &qhs_imem_cfg,
		   &qhs_ipa,
		   &qhs_mnoc_cfg,
		   &qhs_pcie_cfg,
		   &qhs_pdm,
		   &qhs_pimem_cfg,
		   &qhs_prng,
		   &qhs_qdss_cfg,
		   &qhs_qupv3_center,
		   &qhs_qupv3_north,
		   &qhs_sdc2,
		   &qhs_sdc4,
		   &qhs_snoc_cfg,
		   &qhs_spdm,
		   &qhs_tcsr,
		   &qhs_tlmm_north,
		   &qhs_tlmm_south,
		   &qhs_tlmm_west,
		   &qhs_tsif,
		   &qhs_ufs_mem_cfg,
		   &qhs_usb3_0,
		   &qhs_venus_cfg,
		   &qhs_venus_cvp_throttle_cfg,
		   &qhs_venus_throttle_cfg,
		   &qhs_vsense_ctrl_cfg,
		   &qns_cnoc_a2noc,
		   &srvc_cnoc
	},
};

static struct qcom_icc_bcm bcm_qup0 = {
	.name = "QUP0",
	.keepalive = false,
	.num_nodes = 2,
	.nodes = { &qhm_qup_center,
		   &qhm_qup_north
	},
};

static struct qcom_icc_bcm bcm_sn1 = {
	.name = "SN1",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qxs_imem },
};

static struct qcom_icc_bcm bcm_sn2 = {
	.name = "SN2",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qns_gemnoc_gc },
};

static struct qcom_icc_bcm bcm_sn4 = {
	.name = "SN4",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qxs_pimem },
};

static struct qcom_icc_bcm bcm_sn9 = {
	.name = "SN9",
	.keepalive = false,
	.num_nodes = 2,
	.nodes = { &qnm_aggre1_noc,
		   &qns_a1noc_snoc
	},
};

static struct qcom_icc_bcm bcm_sn11 = {
	.name = "SN11",
	.keepalive = false,
	.num_nodes = 2,
	.nodes = { &qnm_aggre2_noc,
		   &qns_a2noc_snoc
	},
};

static struct qcom_icc_bcm bcm_sn12 = {
	.name = "SN12",
	.keepalive = false,
	.num_nodes = 2,
	.nodes = { &qxm_pimem,
		   &xm_gic
	},
};

static struct qcom_icc_bcm bcm_sn14 = {
	.name = "SN14",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qns_pcie_gemnoc },
};

static struct qcom_icc_bcm bcm_sn15 = {
	.name = "SN15",
	.keepalive = false,
	.num_nodes = 1,
	.nodes = { &qnm_gemnoc },
};

static struct qcom_icc_bcm * const aggre1_noc_bcms[] = {
	&bcm_cn0,
	&bcm_qup0,
	&bcm_sn9,
};

static struct qcom_icc_node * const aggre1_noc_nodes[] = {
	[MASTER_A1NOC_CFG] = &qhm_a1noc_cfg,
	[MASTER_QUP_0] = &qhm_qup_center,
	[MASTER_TSIF] = &qhm_tsif,
	[MASTER_EMMC] = &xm_emmc,
	[MASTER_SDCC_2] = &xm_sdc2,
	[MASTER_SDCC_4] = &xm_sdc4,
	[MASTER_UFS_MEM] = &xm_ufs_mem,
	[A1NOC_SNOC_SLV] = &qns_a1noc_snoc,
	[SLAVE_SERVICE_A1NOC] = &srvc_aggre1_noc,
};

static const struct qcom_icc_desc sm7150_aggre1_noc = {
	.nodes = aggre1_noc_nodes,
	.num_nodes = ARRAY_SIZE(aggre1_noc_nodes),
	.bcms = aggre1_noc_bcms,
	.num_bcms = ARRAY_SIZE(aggre1_noc_bcms),
};

static struct qcom_icc_bcm * const aggre2_noc_bcms[] = {
	&bcm_ce0,
	&bcm_qup0,
	&bcm_sn11,
	&bcm_sn14,
};

static struct qcom_icc_node * const aggre2_noc_nodes[] = {
	[MASTER_A2NOC_CFG] = &qhm_a2noc_cfg,
	[MASTER_QDSS_BAM] = &qhm_qdss_bam,
	[MASTER_QUP_1] = &qhm_qup_north,
	[MASTER_CNOC_A2NOC] = &qnm_cnoc,
	[MASTER_CRYPTO_CORE_0] = &qxm_crypto,
	[MASTER_IPA] = &qxm_ipa,
	[MASTER_PCIE] = &xm_pcie3_0,
	[MASTER_QDSS_ETR] = &xm_qdss_etr,
	[MASTER_USB3] = &xm_usb3_0,
	[A2NOC_SNOC_SLV] = &qns_a2noc_snoc,
	[SLAVE_ANOC_PCIE_GEM_NOC] = &qns_pcie_gemnoc,
	[SLAVE_SERVICE_A2NOC] = &srvc_aggre2_noc,
};

static const struct qcom_icc_desc sm7150_aggre2_noc = {
	.nodes = aggre2_noc_nodes,
	.num_nodes = ARRAY_SIZE(aggre2_noc_nodes),
	.bcms = aggre2_noc_bcms,
	.num_bcms = ARRAY_SIZE(aggre2_noc_bcms),
};

static struct qcom_icc_bcm * const camnoc_virt_bcms[] = {
	&bcm_mm1,
};

static struct qcom_icc_node * const camnoc_virt_nodes[] = {
	[MASTER_CAMNOC_HF0_UNCOMP] = &qxm_camnoc_hf0_uncomp,
	[MASTER_CAMNOC_RT_UNCOMP] = &qxm_camnoc_rt_uncomp,
	[MASTER_CAMNOC_SF_UNCOMP] = &qxm_camnoc_sf_uncomp,
	[MASTER_CAMNOC_NRT_UNCOMP] = &qxm_camnoc_nrt_uncomp,
	[SLAVE_CAMNOC_UNCOMP] = &qns_camnoc_uncomp,
};

static const struct qcom_icc_desc sm7150_camnoc_virt = {
	.nodes = camnoc_virt_nodes,
	.num_nodes = ARRAY_SIZE(camnoc_virt_nodes),
	.bcms = camnoc_virt_bcms,
	.num_bcms = ARRAY_SIZE(camnoc_virt_bcms),
};

static struct qcom_icc_bcm * const compute_noc_bcms[] = {
	&bcm_sh10,
	&bcm_sh8,
};

static struct qcom_icc_node * const compute_noc_nodes[] = {
	[MASTER_NPU] = &qnm_npu,
	[SLAVE_CDSP_GEM_NOC] = &qns_cdsp_gemnoc,
};

static const struct qcom_icc_desc sm7150_compute_noc = {
	.nodes = compute_noc_nodes,
	.num_nodes = ARRAY_SIZE(compute_noc_nodes),
	.bcms = compute_noc_bcms,
	.num_bcms = ARRAY_SIZE(compute_noc_bcms),
};

static struct qcom_icc_bcm * const config_noc_bcms[] = {
	&bcm_cn0,
};

static struct qcom_icc_node * const config_noc_nodes[] = {
	[MASTER_SPDM] = &qhm_spdm,
	[SNOC_CNOC_MAS] = &qnm_snoc,
	[MASTER_QDSS_DAP] = &xm_qdss_dap,
	[SLAVE_A1NOC_CFG] = &qhs_a1_noc_cfg,
	[SLAVE_A2NOC_CFG] = &qhs_a2_noc_cfg,
	[SLAVE_AHB2PHY_NORTH] = &qhs_ahb2phy_north,
	[SLAVE_AHB2PHY_SOUTH] = &qhs_ahb2phy_south,
	[SLAVE_AHB2PHY_WEST] = &qhs_ahb2phy_west,
	[SLAVE_AOP] = &qhs_aop,
	[SLAVE_AOSS] = &qhs_aoss,
	[SLAVE_CAMERA_CFG] = &qhs_camera_cfg,
	[SLAVE_CAMERA_NRT_THROTTLE_CFG] = &qhs_camera_nrt_thrott_cfg,
	[SLAVE_CAMERA_RT_THROTTLE_CFG] = &qhs_camera_rt_throttle_cfg,
	[SLAVE_CLK_CTL] = &qhs_clk_ctl,
	[SLAVE_CDSP_CFG] = &qhs_compute_dsp_cfg,
	[SLAVE_RBCPR_CX_CFG] = &qhs_cpr_cx,
	[SLAVE_RBCPR_MX_CFG] = &qhs_cpr_mx,
	[SLAVE_CRYPTO_0_CFG] = &qhs_crypto0_cfg,
	[SLAVE_CNOC_DDRSS] = &qhs_ddrss_cfg,
	[SLAVE_DISPLAY_CFG] = &qhs_display_cfg,
	[SLAVE_DISPLAY_THROTTLE_CFG] = &qhs_display_throttle_cfg,
	[SLAVE_EMMC_CFG] = &qhs_emmc_cfg,
	[SLAVE_GLM] = &qhs_glm,
	[SLAVE_GRAPHICS_3D_CFG] = &qhs_gpuss_cfg,
	[SLAVE_IMEM_CFG] = &qhs_imem_cfg,
	[SLAVE_IPA_CFG] = &qhs_ipa,
	[SLAVE_CNOC_MNOC_CFG] = &qhs_mnoc_cfg,
	[SLAVE_PCIE_CFG] = &qhs_pcie_cfg,
	[SLAVE_PDM] = &qhs_pdm,
	[SLAVE_PIMEM_CFG] = &qhs_pimem_cfg,
	[SLAVE_PRNG] = &qhs_prng,
	[SLAVE_QDSS_CFG] = &qhs_qdss_cfg,
	[SLAVE_QUP_0] = &qhs_qupv3_center,
	[SLAVE_QUP_1] = &qhs_qupv3_north,
	[SLAVE_SDCC_2] = &qhs_sdc2,
	[SLAVE_SDCC_4] = &qhs_sdc4,
	[SLAVE_SNOC_CFG] = &qhs_snoc_cfg,
	[SLAVE_SPDM_WRAPPER] = &qhs_spdm,
	[SLAVE_TCSR] = &qhs_tcsr,
	[SLAVE_TLMM_NORTH] = &qhs_tlmm_north,
	[SLAVE_TLMM_SOUTH] = &qhs_tlmm_south,
	[SLAVE_TLMM_WEST] = &qhs_tlmm_west,
	[SLAVE_TSIF] = &qhs_tsif,
	[SLAVE_UFS_MEM_CFG] = &qhs_ufs_mem_cfg,
	[SLAVE_USB3] = &qhs_usb3_0,
	[SLAVE_VENUS_CFG] = &qhs_venus_cfg,
	[SLAVE_VENUS_CVP_THROTTLE_CFG] = &qhs_venus_cvp_throttle_cfg,
	[SLAVE_VENUS_THROTTLE_CFG] = &qhs_venus_throttle_cfg,
	[SLAVE_VSENSE_CTRL_CFG] = &qhs_vsense_ctrl_cfg,
	[SLAVE_CNOC_A2NOC] = &qns_cnoc_a2noc,
	[SLAVE_SERVICE_CNOC] = &srvc_cnoc,
};

static const struct qcom_icc_desc sm7150_config_noc = {
	.nodes = config_noc_nodes,
	.num_nodes = ARRAY_SIZE(config_noc_nodes),
	.bcms = config_noc_bcms,
	.num_bcms = ARRAY_SIZE(config_noc_bcms),
};

static struct qcom_icc_bcm * const dc_noc_bcms[] = {
};

static struct qcom_icc_node * const dc_noc_nodes[] = {
	[MASTER_CNOC_DC_NOC] = &qhm_cnoc_dc_noc,
	[SLAVE_GEM_NOC_CFG] = &qhs_gemnoc,
	[SLAVE_LLCC_CFG] = &qhs_llcc,
};

static const struct qcom_icc_desc sm7150_dc_noc = {
	.nodes = dc_noc_nodes,
	.num_nodes = ARRAY_SIZE(dc_noc_nodes),
	.bcms = dc_noc_bcms,
	.num_bcms = ARRAY_SIZE(dc_noc_bcms),
};

static struct qcom_icc_bcm * const gem_noc_bcms[] = {
	&bcm_sh0,
	&bcm_sh2,
	&bcm_sh3,
	&bcm_sh5,
};

static struct qcom_icc_node * const gem_noc_nodes[] = {
	[MASTER_AMPSS_M0] = &acm_apps,
	[MASTER_SYS_TCU] = &acm_sys_tcu,
	[MASTER_GEM_NOC_CFG] = &qhm_gemnoc_cfg,
	[MASTER_COMPUTE_NOC] = &qnm_cmpnoc,
	[MASTER_MNOC_HF_MEM_NOC] = &qnm_mnoc_hf,
	[MASTER_MNOC_SF_MEM_NOC] = &qnm_mnoc_sf,
	[MASTER_GEM_NOC_PCIE_SNOC] = &qnm_pcie,
	[MASTER_SNOC_GC_MEM_NOC] = &qnm_snoc_gc,
	[MASTER_SNOC_SF_MEM_NOC] = &qnm_snoc_sf,
	[MASTER_GRAPHICS_3D] = &qxm_gpu,
	[SLAVE_MSS_PROC_MS_MPU_CFG] = &qhs_mdsp_ms_mpu_cfg,
	[SLAVE_GEM_NOC_SNOC] = &qns_gem_noc_snoc,
	[SLAVE_LLCC] = &qns_llcc,
	[SLAVE_SERVICE_GEM_NOC] = &srvc_gemnoc,
};

static const struct qcom_icc_desc sm7150_gem_noc = {
	.nodes = gem_noc_nodes,
	.num_nodes = ARRAY_SIZE(gem_noc_nodes),
	.bcms = gem_noc_bcms,
	.num_bcms = ARRAY_SIZE(gem_noc_bcms),
};

static struct qcom_icc_bcm * const mc_virt_bcms[] = {
	&bcm_acv,
	&bcm_mc0,
};

static struct qcom_icc_node * const mc_virt_nodes[] = {
	[MASTER_LLCC] = &llcc_mc,
	[SLAVE_EBI_CH0] = &ebi,
};

static const struct qcom_icc_desc sm7150_mc_virt = {
	.nodes = mc_virt_nodes,
	.num_nodes = ARRAY_SIZE(mc_virt_nodes),
	.bcms = mc_virt_bcms,
	.num_bcms = ARRAY_SIZE(mc_virt_bcms),
};

static struct qcom_icc_bcm * const mmss_noc_bcms[] = {
	&bcm_mm0,
	&bcm_mm1,
	&bcm_mm2,
	&bcm_mm3,
};

static struct qcom_icc_node * const mmss_noc_nodes[] = {
	[MASTER_CNOC_MNOC_CFG] = &qhm_mnoc_cfg,
	[MASTER_CAMNOC_HF0] = &qxm_camnoc_hf,
	[MASTER_CAMNOC_NRT] = &qxm_camnoc_nrt,
	[MASTER_CAMNOC_RT] = &qxm_camnoc_rt,
	[MASTER_CAMNOC_SF] = &qxm_camnoc_sf,
	[MASTER_MDP_PORT0] = &qxm_mdp0,
	[MASTER_MDP_PORT1] = &qxm_mdp1,
	[MASTER_ROTATOR] = &qxm_rot,
	[MASTER_VIDEO_P0] = &qxm_venus0,
	[MASTER_VIDEO_P1] = &qxm_venus1,
	[MASTER_VIDEO_PROC] = &qxm_venus_arm9,
	[SLAVE_MNOC_SF_MEM_NOC] = &qns2_mem_noc,
	[SLAVE_MNOC_HF_MEM_NOC] = &qns_mem_noc_hf,
	[SLAVE_SERVICE_MNOC] = &srvc_mnoc,
};

static const struct qcom_icc_desc sm7150_mmss_noc = {
	.nodes = mmss_noc_nodes,
	.num_nodes = ARRAY_SIZE(mmss_noc_nodes),
	.bcms = mmss_noc_bcms,
	.num_bcms = ARRAY_SIZE(mmss_noc_bcms),
};

static struct qcom_icc_bcm * const system_noc_bcms[] = {
	&bcm_sn0,
	&bcm_sn1,
	&bcm_sn11,
	&bcm_sn12,
	&bcm_sn15,
	&bcm_sn2,
	&bcm_sn4,
	&bcm_sn9,
};

static struct qcom_icc_node * const system_noc_nodes[] = {
	[MASTER_SNOC_CFG] = &qhm_snoc_cfg,
	[A1NOC_SNOC_MAS] = &qnm_aggre1_noc,
	[A2NOC_SNOC_MAS] = &qnm_aggre2_noc,
	[MASTER_GEM_NOC_SNOC] = &qnm_gemnoc,
	[MASTER_PIMEM] = &qxm_pimem,
	[MASTER_GIC] = &xm_gic,
	[SLAVE_APPSS] = &qhs_apss,
	[SNOC_CNOC_SLV] = &qns_cnoc,
	[SLAVE_SNOC_GEM_NOC_GC] = &qns_gemnoc_gc,
	[SLAVE_SNOC_GEM_NOC_SF] = &qns_gemnoc_sf,
	[SLAVE_OCIMEM] = &qxs_imem,
	[SLAVE_PIMEM] = &qxs_pimem,
	[SLAVE_SERVICE_SNOC] = &srvc_snoc,
	[SLAVE_QDSS_STM] = &xs_qdss_stm,
	[SLAVE_TCU] = &xs_sys_tcu_cfg,
};

static const struct qcom_icc_desc sm7150_system_noc = {
	.nodes = system_noc_nodes,
	.num_nodes = ARRAY_SIZE(system_noc_nodes),
	.bcms = system_noc_bcms,
	.num_bcms = ARRAY_SIZE(system_noc_bcms),
};

static const struct of_device_id qnoc_of_match[] = {
	{ .compatible = "qcom,sm7150-aggre1-noc", .data = &sm7150_aggre1_noc },
	{ .compatible = "qcom,sm7150-aggre2-noc", .data = &sm7150_aggre2_noc },
	{ .compatible = "qcom,sm7150-camnoc-virt", .data = &sm7150_camnoc_virt },
	{ .compatible = "qcom,sm7150-compute-noc", .data = &sm7150_compute_noc },
	{ .compatible = "qcom,sm7150-config-noc", .data = &sm7150_config_noc },
	{ .compatible = "qcom,sm7150-dc-noc", .data = &sm7150_dc_noc },
	{ .compatible = "qcom,sm7150-gem-noc", .data = &sm7150_gem_noc },
	{ .compatible = "qcom,sm7150-mc-virt", .data = &sm7150_mc_virt },
	{ .compatible = "qcom,sm7150-mmss-noc", .data = &sm7150_mmss_noc },
	{ .compatible = "qcom,sm7150-system-noc", .data = &sm7150_system_noc },
	{ }
};
MODULE_DEVICE_TABLE(of, qnoc_of_match);

static struct platform_driver qnoc_driver = {
	.probe = qcom_icc_rpmh_probe,
	.remove = qcom_icc_rpmh_remove,
	.driver = {
		.name = "qnoc-sm7150",
		.of_match_table = qnoc_of_match,
		.sync_state = icc_sync_state,
	},
};

static int __init qnoc_driver_init(void)
{
	return platform_driver_register(&qnoc_driver);
}
core_initcall(qnoc_driver_init);

static void __exit qnoc_driver_exit(void)
{
	platform_driver_unregister(&qnoc_driver);
}
module_exit(qnoc_driver_exit);

MODULE_DESCRIPTION("Qualcomm SM7150 NoC driver");
MODULE_LICENSE("GPL");
