#!/bin/bash

echo "Copying kernel and bootfiles from deploy dir"

for file in Image initramfs.uImage; do
	cp ${DEPLOY_DIR_IMAGE}/$file .
done

VERSION_ID=${FIRMWARE_VERSION}
echo ${VERSION_ID} > VERSION

if [ "${image_type}" == "update" ]; then
	cp ${DEPLOY_DIR_IMAGE}/boot-update.scr ./boot.scr
elif [ "${image_type}" == "recovery" ]; then
	cp ${DEPLOY_DIR_IMAGE}/boot-recovery.scr ./boot.scr
elif [ "${image_type}" == "production" ]; then
	cp ${DEPLOY_DIR_IMAGE}/boot-production.scr ./boot.scr
elif [ "${image_type}" == "production_scan" ]; then
	cp ${DEPLOY_DIR_IMAGE}/boot-scan.scr ./boot.scr

	# Copy production_scan_image squash fs
	cp ${ROOTFS} rootfs.img
	openssl dgst -sha512 -sign ${signing_key} -out rootfs.img.sig rootfs.img
fi
