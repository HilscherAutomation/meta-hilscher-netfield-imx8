all: srk-fuse imx-boot boot-scr

srk-fuse: hab/signed/srk-fuse

boot-scr_in: hab/boot.scr hab/csf_additional_images.in
boot-scr: boot-scr_in hab/signed/boot-scr

imx-boot_in: hab/flash.bin hab/flash.log hab/print_fit_hab.log hab/csf_fit.in hab/csf_spl.in
imx-boot: imx-boot_in hab/signed/u-boot

hab/signed/srk-fuse hab/signed/u-boot hab/signed/boot-scr:
	cd hab && ../tools/csf.$(@F)

clean:
	rm -rf hab/signed
