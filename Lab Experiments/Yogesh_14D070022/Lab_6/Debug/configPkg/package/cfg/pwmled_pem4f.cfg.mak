# invoke SourceDir generated makefile for pwmled.pem4f
pwmled.pem4f: .libraries,pwmled.pem4f
.libraries,pwmled.pem4f: package/cfg/pwmled_pem4f.xdl
	$(MAKE) -f /home/praton/Tiva_workspace/pwmled_EK_TM4C123GXL_TI/src/makefile.libs

clean::
	$(MAKE) -f /home/praton/Tiva_workspace/pwmled_EK_TM4C123GXL_TI/src/makefile.libs clean

