

@echo off
if exist *.o del *.o

set PORT_PATH=
if "%4"=="__PIC24F__"   set PORT_PATH="portable/mchp_c30_pic24"
if "%4"=="__PIC24H__"   set PORT_PATH="portable/mchp_c30_pic24"
if "%4"=="__dsPIC30F__" set PORT_PATH="portable/mchp_c30_dspic"
if "%4"=="__dsPIC33F__" set PORT_PATH="portable/mchp_c30_dspic"
if PORT_PATH=="" goto :stop

set LIB_NAME=
if "%4"=="__PIC24F__"   set LIB_NAME="pic24"
if "%4"=="__PIC24H__"   set LIB_NAME="pic24"
if "%4"=="__dsPIC30F__" set LIB_NAME="dspic"
if "%4"=="__dsPIC33F__" set LIB_NAME="dspic"


echo [TNKernel]: Start building TNKernel LIB for %4 (%3)
echo.

echo [TNKernel]: Common layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o cm_rtns.o      core/common/cm_rtns.c


echo [TNKernel]: System layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fcm_cxgt.o  core/system/sy_fcm_cxgt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fcm_tist.o  core/system/sy_fcm_tist.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fcm_tigt.o  core/system/sy_fcm_tigt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_dat.o       core/system/sy_dat.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fce_stsy.o  core/system/sy_fce_stsy.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fce_tiip.o  core/system/sy_fce_tiip.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fce_tstk.o  core/system/sy_fce_tstk.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fnc_stsy.o  core/system/sy_fnc_stsy.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fnc_tiip.o  core/system/sy_fnc_tiip.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_fnc_tstk.o  core/system/sy_fnc_tstk.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_rtn_itfn.o  core/system/sy_rtn_itfn.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sy_rtn_ttfn.o  core/system/sy_rtn_ttfn.c

echo [TNKernel]: Task layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_refr.o  core/task/ts_fce_refr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_refr.o  core/task/ts_fnc_refr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_iref.o  core/task/ts_fce_iref.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_iref.o  core/task/ts_fnc_iref.c

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_wcmp.o  core/task/ts_rtn_wcmp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_trbl.o  core/task/ts_rtn_trbl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_tnrb.o  core/task/ts_rtn_tnrb.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_sdms.o  core/task/ts_rtn_sdms.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_scpr.o  core/task/ts_rtn_scpr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_fntr.o  core/task/ts_rtn_fntr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_cwac.o  core/task/ts_rtn_cwac.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_rtn_crpr.o  core/task/ts_rtn_crpr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_wakp.o  core/task/ts_fce_wakp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_term.o  core/task/ts_fce_term.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_supd.o  core/task/ts_fce_supd.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_slep.o  core/task/ts_fce_slep.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_rlwt.o  core/task/ts_fce_rlwt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_resm.o  core/task/ts_fce_resm.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_iwkp.o  core/task/ts_fce_iwkp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_isup.o  core/task/ts_fce_isup.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_irlw.o  core/task/ts_fce_irlw.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_ires.o  core/task/ts_fce_ires.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_iact.o  core/task/ts_fce_iact.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_exit.o  core/task/ts_fce_exit.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_delt.o  core/task/ts_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_crte.o  core/task/ts_fce_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_chpr.o  core/task/ts_fce_chpr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fce_actv.o  core/task/ts_fce_actv.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_wakp.o  core/task/ts_fnc_wakp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_term.o  core/task/ts_fnc_term.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_supd.o  core/task/ts_fnc_supd.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_slep.o  core/task/ts_fnc_slep.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_rlwt.o  core/task/ts_fnc_rlwt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_resm.o  core/task/ts_fnc_resm.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_iwkp.o  core/task/ts_fnc_iwkp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_isup.o  core/task/ts_fnc_isup.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_irlw.o  core/task/ts_fnc_irlw.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_ires.o  core/task/ts_fnc_ires.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_iact.o  core/task/ts_fnc_iact.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_exit.o  core/task/ts_fnc_exit.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_delt.o  core/task/ts_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_crte.o  core/task/ts_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_chpr.o  core/task/ts_fnc_chpr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ts_fnc_actv.o  core/task/ts_fnc_actv.c

echo [TNKernel]: Semaphore layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_sign.o  core/semph/sm_fnc_sign.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_poll.o  core/semph/sm_fnc_poll.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_isig.o  core/semph/sm_fnc_isig.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_ipol.o  core/semph/sm_fnc_ipol.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_delt.o  core/semph/sm_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_crte.o  core/semph/sm_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fnc_asqu.o  core/semph/sm_fnc_asqu.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_sign.o  core/semph/sm_fce_sign.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_poll.o  core/semph/sm_fce_poll.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_isig.o  core/semph/sm_fce_isig.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_ipol.o  core/semph/sm_fce_ipol.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_delt.o  core/semph/sm_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_crte.o  core/semph/sm_fce_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o sm_fce_asqu.o  core/semph/sm_fce_asqu.c

echo [TNKernel]: Event layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_rtn_sewq.o  core/event/ev_rtn_sewq.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_wtpl.o  core/event/ev_fnc_wtpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_wate.o  core/event/ev_fnc_wate.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_sete.o  core/event/ev_fnc_sete.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_iwat.o  core/event/ev_fnc_iwat.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_iset.o  core/event/ev_fnc_iset.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_iclr.o  core/event/ev_fnc_iclr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_delt.o  core/event/ev_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_crte.o  core/event/ev_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fnc_cler.o  core/event/ev_fnc_cler.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_wtpl.o  core/event/ev_fce_wtpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_wate.o  core/event/ev_fce_wate.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_sete.o  core/event/ev_fce_sete.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_iwat.o  core/event/ev_fce_iwat.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_iset.o  core/event/ev_fce_iset.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_iclr.o  core/event/ev_fce_iclr.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_delt.o  core/event/ev_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_crte.o  core/event/ev_fce_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o ev_fce_cler.o  core/event/ev_fce_cler.c

echo [TNKernel]: Mutex layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_dat.o       core/mutex/mx_dat.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_rtn_dulm.o  core/mutex/mx_rtn_dulm.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_rtn_elmx.o  core/mutex/mx_rtn_elmx.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_rtn_fmbp.o  core/mutex/mx_rtn_fmbp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_rtn_rtbl.o  core/mutex/mx_rtn_rtbl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_rtn_tlmx.o  core/mutex/mx_rtn_tlmx.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fnc_unlk.o  core/mutex/mx_fnc_unlk.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fnc_lock.o  core/mutex/mx_fnc_lock.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fnc_lkpl.o  core/mutex/mx_fnc_lkpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fnc_delt.o  core/mutex/mx_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fnc_crte.o  core/mutex/mx_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fce_unlk.o  core/mutex/mx_fce_unlk.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fce_lock.o  core/mutex/mx_fce_lock.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fce_lkpl.o  core/mutex/mx_fce_lkpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fce_delt.o  core/mutex/mx_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mx_fce_crte.o  core/mutex/mx_fce_crte.c

echo [TNKernel]: Data queue layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_crte.o  core/queue/dq_fce_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_delt.o  core/queue/dq_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_ircv.o  core/queue/dq_fce_ircv.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_ispl.o  core/queue/dq_fce_ispl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_rcpl.o  core/queue/dq_fce_rcpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_recv.o  core/queue/dq_fce_recv.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_send.o  core/queue/dq_fce_send.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fce_sepl.o  core/queue/dq_fce_sepl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_crte.o  core/queue/dq_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_delt.o  core/queue/dq_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_ircv.o  core/queue/dq_fnc_ircv.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_ispl.o  core/queue/dq_fnc_ispl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_rcpl.o  core/queue/dq_fnc_rcpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_recv.o  core/queue/dq_fnc_recv.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_send.o  core/queue/dq_fnc_send.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_fnc_sepl.o  core/queue/dq_fnc_sepl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_rtn_fird.o  core/queue/dq_rtn_fird.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o dq_rtn_fiwr.o  core/queue/dq_rtn_fiwr.c

echo [TNKernel]: Memory pool layer...

pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_crte.o  core/mpool/mp_fce_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_delt.o  core/mpool/mp_fce_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_gtpl.o  core/mpool/mp_fce_gtpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_igtp.o  core/mpool/mp_fce_igtp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_irel.o  core/mpool/mp_fce_irel.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_mget.o  core/mpool/mp_fce_mget.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fce_rels.o  core/mpool/mp_fce_rels.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_crte.o  core/mpool/mp_fnc_crte.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_delt.o  core/mpool/mp_fnc_delt.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_gtpl.o  core/mpool/mp_fnc_gtpl.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_igtp.o  core/mpool/mp_fnc_igtp.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_irel.o  core/mpool/mp_fnc_irel.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_mget.o  core/mpool/mp_fnc_mget.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_fnc_rels.o  core/mpool/mp_fnc_rels.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_rtn_getm.o  core/mpool/mp_rtn_getm.c
pic30-gcc -c -x c -g -Wall -omf=%3 -mtext=tnk_code -mlarge-code -m%2 -mlarge-scalar -mconst-in-code -D%4 -O%1 -o mp_rtn_putm.o  core/mpool/mp_rtn_putm.c

echo [TNKernel]: Portable layer...
pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port.o         %PORT_PATH%/port.S

REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_cirq.o %PORT_PATH%/port_af_cirq.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_ffsa.o %PORT_PATH%/port_af_ffsa.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_rtsr.o %PORT_PATH%/port_af_rtsr.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_stki.o %PORT_PATH%/port_af_stki.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_svsr.o %PORT_PATH%/port_af_svsr.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_swce.o %PORT_PATH%/port_af_swce.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_swcx.o %PORT_PATH%/port_af_swcx.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_encr.o %PORT_PATH%/port_af_encr.S
REM pic30-gcc -c      -g       -omf=%3                                                                  -D%4      -o port_af_excr.o %PORT_PATH%/port_af_excr.S


cd _lib
if exist tnkernel_%LIB_NAME%_%3.a del tnkernel_%LIB_NAME%_%3.a
cd ..

echo [TNKernel]: Build LIB for %LIB_NAME% (%3)

pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" cm_rtns.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" sy_dat.o sy_fce_stsy.o sy_fce_tiip.o sy_fce_tstk.o sy_fnc_stsy.o sy_fnc_tiip.o sy_fnc_tstk.o sy_rtn_itfn.o sy_rtn_ttfn.o sy_fcm_tist.o sy_fcm_tigt.o sy_fcm_cxgt.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" ts_fce_actv.o ts_fce_chpr.o ts_fce_crte.o ts_fce_delt.o ts_fce_exit.o ts_fce_iact.o ts_fce_ires.o ts_fce_irlw.o ts_fce_isup.o ts_fce_wakp.o ts_fce_term.o ts_fce_supd.o ts_fce_slep.o ts_fce_rlwt.o ts_fce_resm.o ts_fce_iwkp.o ts_fnc_wakp.o ts_fnc_term.o ts_fnc_supd.o ts_fnc_slep.o ts_fnc_rlwt.o ts_fnc_resm.o ts_fnc_iwkp.o ts_fnc_isup.o ts_fnc_irlw.o ts_fnc_ires.o ts_fnc_iact.o ts_fnc_exit.o ts_fnc_delt.o ts_fnc_crte.o ts_fnc_chpr.o ts_fnc_actv.o ts_rtn_crpr.o ts_rtn_cwac.o ts_rtn_fntr.o ts_rtn_scpr.o ts_rtn_sdms.o ts_rtn_tnrb.o ts_rtn_trbl.o ts_rtn_wcmp.o ts_fce_refr.o ts_fnc_refr.o ts_fce_iref.o ts_fnc_iref.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" sm_fce_asqu.o sm_fce_sign.o sm_fce_poll.o sm_fce_isig.o sm_fce_ipol.o sm_fce_delt.o sm_fce_crte.o sm_fnc_sign.o sm_fnc_poll.o sm_fnc_isig.o sm_fnc_ipol.o sm_fnc_delt.o sm_fnc_crte.o sm_fnc_asqu.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" ev_fce_cler.o ev_fce_wtpl.o ev_fce_wate.o ev_fce_sete.o ev_fce_iwat.o ev_fce_iset.o ev_fce_iclr.o ev_fce_delt.o ev_fce_crte.o ev_fnc_wtpl.o ev_fnc_wate.o ev_fnc_sete.o ev_fnc_iwat.o ev_fnc_iset.o ev_fnc_iclr.o ev_fnc_delt.o ev_fnc_crte.o ev_fnc_cler.o ev_rtn_sewq.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" mx_dat.o mx_fnc_unlk.o mx_fnc_lock.o mx_fnc_lkpl.o mx_fnc_delt.o mx_fnc_crte.o mx_fce_unlk.o mx_fce_lock.o mx_fce_lkpl.o mx_fce_delt.o mx_fce_crte.o mx_rtn_dulm.o mx_rtn_elmx.o mx_rtn_fmbp.o mx_rtn_rtbl.o mx_rtn_tlmx.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" dq_fce_crte.o dq_fce_delt.o dq_fce_ircv.o dq_fce_ispl.o dq_fce_rcpl.o dq_fce_recv.o dq_fce_send.o dq_fce_sepl.o dq_fnc_crte.o dq_fnc_delt.o dq_fnc_ircv.o dq_fnc_ispl.o dq_fnc_rcpl.o dq_fnc_recv.o dq_fnc_send.o dq_fnc_sepl.o dq_rtn_fird.o dq_rtn_fiwr.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" mp_fce_crte.o mp_fce_delt.o mp_fce_gtpl.o mp_fce_igtp.o mp_fce_irel.o mp_fce_mget.o mp_fce_rels.o mp_fnc_crte.o mp_fnc_delt.o mp_fnc_gtpl.o mp_fnc_igtp.o mp_fnc_irel.o mp_fnc_mget.o mp_fnc_rels.o mp_rtn_getm.o mp_rtn_putm.o
pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" port.o
REM pic30-ar.exe -omf=%3 q "_lib\tnkernel_%LIB_NAME%_%3.a" port_af_cirq.o port_af_ffsa.o port_af_rtsr.o port_af_stki.o port_af_svsr.o port_af_swce.o port_af_swcx.o port_af_encr.o port_af_excr.o

if exist *.o del *.o

echo [TNKernel]: DONE...
echo.
echo.
goto :end

:stop
echo ERROR: Please define core type

:end
