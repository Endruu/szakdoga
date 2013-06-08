# Generated by the VisualDSP++ IDDE

# Note:  Any changes made to this Makefile will be lost the next time the
# matching project file is loaded into the IDDE.  If you wish to preserve
# changes, rename this file and run it externally to the IDDE.

# The syntax of this Makefile is such that GNU Make v3.77 or higher is
# required.

# The current working directory should be the directory in which this
# Makefile resides.

# Supported targets:
#     szakdoga_Debug
#     szakdoga_Debug_clean

# Define this variable if you wish to run this Makefile on a host
# other than the host that created it and VisualDSP++ may be installed
# in a different directory.

ADI_DSP=C:\Program Files\Analog Devices\VisualDSP 5.0


# $VDSP is a gmake-friendly version of ADI_DIR

empty:=
space:= $(empty) $(empty)
VDSP_INTERMEDIATE=$(subst \,/,$(ADI_DSP))
VDSP=$(subst $(space),\$(space),$(VDSP_INTERMEDIATE))

RM=cmd /C del /F /Q

#
# Begin "szakdoga_Debug" configuration
#

ifeq ($(MAKECMDGOALS),szakdoga_Debug)

szakdoga_Debug : ./Debug/szakdoga.dxe 

Debug/bilinear.doj :bilinear.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h 
	@echo ".\bilinear.c"
	$(VDSP)/ccblkfn.exe -c .\bilinear.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\bilinear.doj -MM

./Debug/direct1.doj :./direct1.asm 
	@echo ".\direct1.asm"
	$(VDSP)/easmblkfn.exe .\direct1.asm -proc ADSP-BF537 -file-attr ProjectName=szakdoga -g -si-revision 0.2 -o .\Debug\direct1.doj -MM

Debug/ecomplex.doj :ecomplex.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h 
	@echo ".\ecomplex.c"
	$(VDSP)/ccblkfn.exe -c .\ecomplex.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\ecomplex.doj -MM

Debug/error.doj :error.c error.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/sys/stdio_bf.h 
	@echo ".\error.c"
	$(VDSP)/ccblkfn.exe -c .\error.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\error.doj -MM

Debug/generate_iir.doj :generate_iir.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h 
	@echo ".\generate_iir.c"
	$(VDSP)/ccblkfn.exe -c .\generate_iir.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\generate_iir.doj -MM

Debug/implement.doj :implement.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/sys/stdio_bf.h 
	@echo ".\implement.c"
	$(VDSP)/ccblkfn.exe -c .\implement.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\implement.doj -MM

Debug/initialize.doj :initialize.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/blackfin.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF537.h $(VDSP)/Blackfin/include/cdefBF534.h $(VDSP)/Blackfin/include/defBF534.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/defBF537.h 
	@echo ".\initialize.c"
	$(VDSP)/ccblkfn.exe -c .\initialize.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\initialize.doj -MM

Debug/interrupts.doj :interrupts.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/blackfin.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF537.h $(VDSP)/Blackfin/include/cdefBF534.h $(VDSP)/Blackfin/include/defBF534.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/defBF537.h 
	@echo ".\interrupts.c"
	$(VDSP)/ccblkfn.exe -c .\interrupts.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\interrupts.doj -MM

Debug/main.doj :main.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/sys/stdio_bf.h $(VDSP)/Blackfin/include/string.h 
	@echo ".\main.c"
	$(VDSP)/ccblkfn.exe -c .\main.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\main.doj -MM

Debug/pzk_container.doj :pzk_container.c $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/sys/stdio_bf.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract2float_conv.h global.h global_var.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h 
	@echo ".\pzk_container.c"
	$(VDSP)/ccblkfn.exe -c .\pzk_container.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\pzk_container.doj -MM

Debug/state.doj :state.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/sys/stdio_bf.h 
	@echo ".\state.c"
	$(VDSP)/ccblkfn.exe -c .\state.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\state.doj -MM

Debug/transform.doj :transform.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/math.h $(VDSP)/Blackfin/include/ymath.h $(VDSP)/Blackfin/include/math_bf.h 
	@echo ".\transform.c"
	$(VDSP)/ccblkfn.exe -c .\transform.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\transform.doj -MM

Debug/uart.doj :uart.c global.h global_var.h $(VDSP)/Blackfin/include/fract.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fract_math.h $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/fr2x16_math.h $(VDSP)/Blackfin/include/fr2x16_base.h $(VDSP)/Blackfin/include/r2x16_base.h $(VDSP)/Blackfin/include/libetsi.h $(VDSP)/Blackfin/include/fract2float_conv.h $(VDSP)/Blackfin/include/complex.h $(VDSP)/Blackfin/include/complex_typedef.h $(VDSP)/Blackfin/include/cycles.h $(VDSP)/Blackfin/include/xcycle_count.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/cycle_count_bf.h error.h $(VDSP)/Blackfin/include/sys/exception.h $(VDSP)/Blackfin/include/blackfin.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF537.h $(VDSP)/Blackfin/include/cdefBF534.h $(VDSP)/Blackfin/include/defBF534.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/defBF537.h 
	@echo ".\uart.c"
	$(VDSP)/ccblkfn.exe -c .\uart.c -file-attr ProjectName=szakdoga -O -Ov100 -ipa -g -Oa -structs-do-not-overlap -no-multiline -D ETSI_SOURCE -D __SET_ETSI_FLAGS=1 -double-size-64 -decls-strong -flags-compiler --diag_warning,implicit_func_decl -warn-protos -l etsi*co -double-size-any -si-revision 0.2 -proc ADSP-BF537 -o .\Debug\uart.doj -MM

./Debug/szakdoga.dxe :./ADSP-BF537.ldf $(VDSP)/Blackfin/lib/bf534_rev_0.0/crtsf532y.doj ./Debug/bilinear.doj ./Debug/direct1.doj ./Debug/ecomplex.doj ./Debug/error.doj ./Debug/generate_iir.doj ./Debug/implement.doj ./Debug/initialize.doj ./Debug/interrupts.doj ./Debug/main.doj ./Debug/pzk_container.doj ./Debug/state.doj ./Debug/transform.doj ./Debug/uart.doj $(VDSP)/Blackfin/lib/cplbtab537.doj $(VDSP)/Blackfin/lib/bf534_rev_0.0/libsmall532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/__initsbsz532.doj $(VDSP)/Blackfin/lib/bf534_rev_0.0/libio532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libc532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libm3free532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libevent532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libx532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libcpp532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libcpprt532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libf64ieee532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libdsp532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/libsftflt532y.dlb $(VDSP)/Blackfin/lib/libetsi532.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/Debug/libssl537y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/Debug/libdrv537y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.0/idle532y.doj $(VDSP)/Blackfin/lib/bf534_rev_0.0/librt_fileio532y.dlb 
	@echo "Linking..."
	$(VDSP)/ccblkfn.exe .\Debug\bilinear.doj .\Debug\direct1.doj .\Debug\ecomplex.doj .\Debug\error.doj .\Debug\generate_iir.doj .\Debug\implement.doj .\Debug\initialize.doj .\Debug\interrupts.doj .\Debug\main.doj .\Debug\pzk_container.doj .\Debug\state.doj .\Debug\transform.doj .\Debug\uart.doj -T .\ADSP-BF537.ldf -map .\Debug\szakdoga.map.xml -L .\Debug -add-debug-libpaths -flags-link -od,.\Debug -o .\Debug\szakdoga.dxe -proc ADSP-BF537 -si-revision 0.2 -flags-link -MM

endif

ifeq ($(MAKECMDGOALS),szakdoga_Debug_clean)

szakdoga_Debug_clean:
	-$(RM) "Debug\bilinear.doj"
	-$(RM) ".\Debug\direct1.doj"
	-$(RM) "Debug\ecomplex.doj"
	-$(RM) "Debug\error.doj"
	-$(RM) "Debug\generate_iir.doj"
	-$(RM) "Debug\implement.doj"
	-$(RM) "Debug\initialize.doj"
	-$(RM) "Debug\interrupts.doj"
	-$(RM) "Debug\main.doj"
	-$(RM) "Debug\pzk_container.doj"
	-$(RM) "Debug\state.doj"
	-$(RM) "Debug\transform.doj"
	-$(RM) "Debug\uart.doj"
	-$(RM) ".\Debug\szakdoga.dxe"
	-$(RM) ".\Debug\*.ipa"
	-$(RM) ".\Debug\*.opa"
	-$(RM) ".\Debug\*.ti"
	-$(RM) ".\Debug\*.pgi"
	-$(RM) ".\*.rbld"

endif


