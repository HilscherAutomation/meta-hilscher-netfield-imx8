#!/bin/bash

echo "Copying kernel and bootfiles from deploy dir:"

echo "Installing ${DEPLOY_DIR_IMAGE}/fitImage-core-image-minimal-initramfs*.bin in boot partition ..."
cp ${DEPLOY_DIR_IMAGE}/fitImage-core-image-minimal-initramfs*.bin Image

VERSION_ID=${FIRMWARE_VERSION}
echo ${VERSION_ID} > VERSION

if [ "${image_type}" == "update" ]; then
	echo "Installing ${DEPLOY_DIR_IMAGE}/boot-update.scr in boot partition ..."
	cp ${DEPLOY_DIR_IMAGE}/boot-update.scr ./boot.scr
elif [ "${image_type}" == "recovery" ]; then
	echo "Installing ${DEPLOY_DIR_IMAGE}/boot-script-fit/boot-recovery.scr in boot partition ..."
	cp ${DEPLOY_DIR_IMAGE}/boot-script-fit/boot-recovery.scr ./boot.scr
elif [ "${image_type}" == "production" ]; then
	echo "Installing ${DEPLOY_DIR_IMAGE}/boot-production.scr in boot partition ..."
	cp ${DEPLOY_DIR_IMAGE}/boot-production.scr ./boot.scr
elif [ "${image_type}" == "production_scan" ]; then
	echo "Installing ${DEPLOY_DIR_IMAGE}/boot-scan.scr in boot partition ..."
	cp ${DEPLOY_DIR_IMAGE}/boot-scan.scr ./boot.scr

	echo "Installing ${ROOTFS} in boot partition ..."
	# Copy production_scan_image squash fs
	cp ${ROOTFS} rootfs.img
	openssl dgst ${engine_params} -sha512 -sign ${signing_key} -out rootfs.img.sig rootfs.img
fi
