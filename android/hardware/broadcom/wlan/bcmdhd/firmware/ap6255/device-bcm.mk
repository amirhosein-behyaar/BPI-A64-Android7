#
# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

-include hardware/broadcom/wlan/bcmdhd/config/config-bcm.mk

WIFI_DRIVER_FW_PATH_STA    := "/system/vendor/modules/fw_bcm43455c0_ag.bin"
WIFI_DRIVER_FW_PATH_P2P    := "/system/vendor/modules/fw_bcm43455c0_ag_p2p.bin"
WIFI_DRIVER_FW_PATH_AP     := "/system/vendor/modules/fw_bcm43455c0_ag_apsta.bin"

PRODUCT_COPY_FILES += \
	hardware/broadcom/wlan/bcmdhd/firmware/ap6255/fw_bcm43455c0_ag.bin:system/vendor/modules/fw_bcm43455c0_ag.bin \
	hardware/broadcom/wlan/bcmdhd/firmware/ap6255/fw_bcm43455c0_ag_apsta.bin:system/vendor/modules/fw_bcm43455c0_ag_apsta.bin \
	hardware/broadcom/wlan/bcmdhd/firmware/ap6255/fw_bcm43455c0_ag_p2p.bin:system/vendor/modules/fw_bcm43455c0_ag_p2p.bin \
	hardware/broadcom/wlan/bcmdhd/firmware/ap6255/nvram_ap6255.txt:system/vendor/modules/nvram.txt \
	hardware/broadcom/wlan/bcmdhd/firmware/ap6255/BCM4345C0.hcd:system/vendor/modules/BCM4345C0.hcd
