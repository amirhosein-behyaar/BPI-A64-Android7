$(call inherit-product, device/softwinner/tulip-common/tulip_64_bit.mk)
$(call inherit-product, build/target/product/full_base.mk)
$(call inherit-product, device/softwinner/tulip-common/tulip-common.mk)
$(call inherit-product-if-exists, device/softwinner/tulip-m64-lcd7/modules/modules.mk)

PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=true
    
DEVICE_PACKAGE_OVERLAYS := device/softwinner/tulip-m64-lcd7/overlay \
                           $(DEVICE_PACKAGE_OVERLAYS)

PRODUCT_PACKAGES += Launcher3

PRODUCT_PACKAGES += \
    WallpaperPicker \
    ESFileExplorer \
    VideoPlayer \
    Saturn \
    Bluetooth \
    Update
#   PartnerChromeCustomizationsProvider

PRODUCT_COPY_FILES += \
    device/softwinner/tulip-m64-lcd7/kernel:kernel \
    device/softwinner/tulip-m64-lcd7/fstab.sun50iw1p1:root/fstab.sun50iw1p1 \
    device/softwinner/tulip-m64-lcd7/init.sun50iw1p1.rc:root/init.sun50iw1p1.rc \
    device/softwinner/tulip-m64-lcd7/init.recovery.sun50iw1p1.rc:root/init.recovery.sun50iw1p1.rc \
    device/softwinner/tulip-m64-lcd7/ueventd.sun50iw1p1.rc:root/ueventd.sun50iw1p1.rc \
    device/softwinner/common/verity/rsa_key/verity_key:root/verity_key \
    device/softwinner/tulip-m64-lcd7/modules/modules/sunxi_tr.ko:root/sunxi_tr.ko \
    device/softwinner/tulip-m64-lcd7/modules/modules/sw-device.ko:recovery/root/sw-device.ko \
    device/softwinner/tulip-m64-lcd7/modules/modules/gt9xxnew_ts.ko:recovery/root/gt9xxnew_ts.ko

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.software.verified_boot.xml:system/etc/permissions/android.software.verified_boot.xml \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml

PRODUCT_COPY_FILES += \
    device/softwinner/tulip-m64-lcd7/configs/camera.cfg:system/etc/camera.cfg \
    device/softwinner/tulip-m64-lcd7/configs/media_profiles.xml:system/etc/media_profiles.xml \
    device/softwinner/tulip-m64-lcd7/configs/gt9xxnew_ts.kl:system/usr/keylayout/gt9xxnew_ts.kl \
    device/softwinner/tulip-m64-lcd7/configs/axp81x-supplyer.kl:system/usr/keylayout/axp81x-supplyer.kl \
    device/softwinner/tulip-m64-lcd7/configs/sunxi_ir_recv.kl:system/usr/keylayout/sunxi_ir_recv.kl \
    device/softwinner/tulip-m64-lcd7/configs/tp.idc:system/usr/idc/tp.idc

PRODUCT_COPY_FILES += \
    device/softwinner/tulip-m64-lcd7/hawkview/sensor_list_cfg.ini:system/etc/hawkview/sensor_list_cfg.ini

# bootanimation
#PRODUCT_COPY_FILES += \
#    device/softwinner/tulip-m64-hdmi/media/bootanimation.zip:system/media/bootanimation.zip

# Radio Packages and Configuration Flie
$(call inherit-product, device/softwinner/common/rild/radio_common.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/huawei/mu509/huawei_mu509.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/Oviphone/em55/oviphone_em55.mk)

PRODUCT_PROPERTY_OVERRIDES += \
	ro.frp.pst=/dev/block/by-name/frp

# limit dex2oat threads to improve thermals
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.boot-dex2oat-threads=4 \
    dalvik.vm.dex2oat-threads=3 \
    dalvik.vm.image-dex2oat-threads=4

PRODUCT_PROPERTY_OVERRIDES += \
	dalvik.vm.dex2oat-flags=--no-watch-dog \
	dalvik.vm.jit.codecachesize=0 \
	ro.am.reschedule_service=true

PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp,adb \
    ro.adb.secure=0 \
    rw.logger=0 \

PRODUCT_PROPERTY_OVERRIDES += \
    ro.product.first_api_level=24

PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapsize=384m \
    dalvik.vm.heapstartsize=8m \
    dalvik.vm.heapgrowthlimit=80m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=512k \
    dalvik.vm.heapmaxfree=8m \
    ro.zygote.disable_gl_preload=false

# scense_control
PRODUCT_PROPERTY_OVERRIDES += \
    sys.p_bootcomplete= true \
    sys.p_debug=false

PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=160

PRODUCT_PROPERTY_OVERRIDES += \
    ro.spk_dul.used=false

PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.timezone=Asia/Shanghai \
    persist.sys.country=US \
    persist.sys.language=en

PRODUCT_LOCALES := en_US zh_CN

# stoarge
PRODUCT_PROPERTY_OVERRIDES += \
    persist.fw.force_adoptable=true

PRODUCT_CHARACTERISTICS := tablet

PRODUCT_AAPT_CONFIG := tvdpi xlarge hdpi xhdpi large
PRODUCT_AAPT_PREF_CONFIG := tvdpi

$(call inherit-product-if-exists, vendor/google/products/gms_base.mk)

PRODUCT_BRAND := BPI
PRODUCT_NAME := tulip_m64_lcd7
PRODUCT_DEVICE := tulip-m64-lcd7
PRODUCT_MODEL := Bananapi
PRODUCT_MANUFACTURER := Sinovoip
