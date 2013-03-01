REM @echo off
if exist *.o del *.o

set PORT_PATH="portable/nxp_rv_cortexm3"
set LIB_NAME="cm3_rv"

echo [TNKernel]: Start building TNKernel LIB for PIC32
echo.

echo [TNKernel]: Common layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o cm_rtns.o core/common/cm_rtns.c

echo [TNKernel]: System layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fcm_cxgt.o  core/system/sy_fcm_cxgt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fcm_tist.o  core/system/sy_fcm_tist.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fcm_tigt.o  core/system/sy_fcm_tigt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_dat.o       core/system/sy_dat.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fce_stsy.o  core/system/sy_fce_stsy.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fce_tiip.o  core/system/sy_fce_tiip.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fce_tstk.o  core/system/sy_fce_tstk.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fnc_stsy.o  core/system/sy_fnc_stsy.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fnc_tiip.o  core/system/sy_fnc_tiip.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_fnc_tstk.o  core/system/sy_fnc_tstk.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_rtn_itfn.o  core/system/sy_rtn_itfn.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sy_rtn_ttfn.o  core/system/sy_rtn_ttfn.c

echo [TNKernel]: Task layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_refr.o  core/task/ts_fce_refr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_refr.o  core/task/ts_fnc_refr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_iref.o  core/task/ts_fce_iref.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_iref.o  core/task/ts_fnc_iref.c

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_wcmp.o  core/task/ts_rtn_wcmp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_trbl.o  core/task/ts_rtn_trbl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_tnrb.o  core/task/ts_rtn_tnrb.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_sdms.o  core/task/ts_rtn_sdms.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_scpr.o  core/task/ts_rtn_scpr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_fntr.o  core/task/ts_rtn_fntr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_cwac.o  core/task/ts_rtn_cwac.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_rtn_crpr.o  core/task/ts_rtn_crpr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_wakp.o  core/task/ts_fce_wakp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_term.o  core/task/ts_fce_term.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_supd.o  core/task/ts_fce_supd.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_slep.o  core/task/ts_fce_slep.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_rlwt.o  core/task/ts_fce_rlwt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_resm.o  core/task/ts_fce_resm.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_iwkp.o  core/task/ts_fce_iwkp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_isup.o  core/task/ts_fce_isup.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_irlw.o  core/task/ts_fce_irlw.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_ires.o  core/task/ts_fce_ires.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_iact.o  core/task/ts_fce_iact.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_exit.o  core/task/ts_fce_exit.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_delt.o  core/task/ts_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_crte.o  core/task/ts_fce_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_chpr.o  core/task/ts_fce_chpr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fce_actv.o  core/task/ts_fce_actv.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_wakp.o  core/task/ts_fnc_wakp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_term.o  core/task/ts_fnc_term.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_supd.o  core/task/ts_fnc_supd.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_slep.o  core/task/ts_fnc_slep.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_rlwt.o  core/task/ts_fnc_rlwt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_resm.o  core/task/ts_fnc_resm.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_iwkp.o  core/task/ts_fnc_iwkp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_isup.o  core/task/ts_fnc_isup.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_irlw.o  core/task/ts_fnc_irlw.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_ires.o  core/task/ts_fnc_ires.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_iact.o  core/task/ts_fnc_iact.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_exit.o  core/task/ts_fnc_exit.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_delt.o  core/task/ts_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_crte.o  core/task/ts_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_chpr.o  core/task/ts_fnc_chpr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ts_fnc_actv.o  core/task/ts_fnc_actv.c

echo [TNKernel]: Semaphore layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_sign.o  core/semph/sm_fnc_sign.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_poll.o  core/semph/sm_fnc_poll.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_isig.o  core/semph/sm_fnc_isig.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_ipol.o  core/semph/sm_fnc_ipol.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_delt.o  core/semph/sm_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_crte.o  core/semph/sm_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fnc_asqu.o  core/semph/sm_fnc_asqu.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_sign.o  core/semph/sm_fce_sign.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_poll.o  core/semph/sm_fce_poll.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_isig.o  core/semph/sm_fce_isig.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_ipol.o  core/semph/sm_fce_ipol.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_delt.o  core/semph/sm_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_crte.o  core/semph/sm_fce_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o sm_fce_asqu.o  core/semph/sm_fce_asqu.c

echo [TNKernel]: Event layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_rtn_sewq.o  core/event/ev_rtn_sewq.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_wtpl.o  core/event/ev_fnc_wtpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_wate.o  core/event/ev_fnc_wate.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_sete.o  core/event/ev_fnc_sete.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_iwat.o  core/event/ev_fnc_iwat.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_iset.o  core/event/ev_fnc_iset.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_iclr.o  core/event/ev_fnc_iclr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_delt.o  core/event/ev_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_crte.o  core/event/ev_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fnc_cler.o  core/event/ev_fnc_cler.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_wtpl.o  core/event/ev_fce_wtpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_wate.o  core/event/ev_fce_wate.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_sete.o  core/event/ev_fce_sete.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_iwat.o  core/event/ev_fce_iwat.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_iset.o  core/event/ev_fce_iset.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_iclr.o  core/event/ev_fce_iclr.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_delt.o  core/event/ev_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_crte.o  core/event/ev_fce_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o ev_fce_cler.o  core/event/ev_fce_cler.c

echo [TNKernel]: Mutex layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_dat.o       core/mutex/mx_dat.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_rtn_dulm.o  core/mutex/mx_rtn_dulm.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_rtn_elmx.o  core/mutex/mx_rtn_elmx.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_rtn_fmbp.o  core/mutex/mx_rtn_fmbp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_rtn_rtbl.o  core/mutex/mx_rtn_rtbl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_rtn_tlmx.o  core/mutex/mx_rtn_tlmx.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fnc_unlk.o  core/mutex/mx_fnc_unlk.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fnc_lock.o  core/mutex/mx_fnc_lock.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fnc_lkpl.o  core/mutex/mx_fnc_lkpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fnc_delt.o  core/mutex/mx_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fnc_crte.o  core/mutex/mx_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fce_unlk.o  core/mutex/mx_fce_unlk.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fce_lock.o  core/mutex/mx_fce_lock.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fce_lkpl.o  core/mutex/mx_fce_lkpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fce_delt.o  core/mutex/mx_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mx_fce_crte.o  core/mutex/mx_fce_crte.c

echo [TNKernel]: Data queue layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_crte.o  core/queue/dq_fce_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_delt.o  core/queue/dq_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_ircv.o  core/queue/dq_fce_ircv.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_ispl.o  core/queue/dq_fce_ispl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_rcpl.o  core/queue/dq_fce_rcpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_recv.o  core/queue/dq_fce_recv.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_send.o  core/queue/dq_fce_send.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fce_sepl.o  core/queue/dq_fce_sepl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_crte.o  core/queue/dq_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_delt.o  core/queue/dq_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_ircv.o  core/queue/dq_fnc_ircv.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_ispl.o  core/queue/dq_fnc_ispl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_rcpl.o  core/queue/dq_fnc_rcpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_recv.o  core/queue/dq_fnc_recv.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_send.o  core/queue/dq_fnc_send.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_fnc_sepl.o  core/queue/dq_fnc_sepl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_rtn_fird.o  core/queue/dq_rtn_fird.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o dq_rtn_fiwr.o  core/queue/dq_rtn_fiwr.c

echo [TNKernel]: Memory pool layer...

armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_crte.o  core/mpool/mp_fce_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_delt.o  core/mpool/mp_fce_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_gtpl.o  core/mpool/mp_fce_gtpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_igtp.o  core/mpool/mp_fce_igtp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_irel.o  core/mpool/mp_fce_irel.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_mget.o  core/mpool/mp_fce_mget.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fce_rels.o  core/mpool/mp_fce_rels.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_crte.o  core/mpool/mp_fnc_crte.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_delt.o  core/mpool/mp_fnc_delt.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_gtpl.o  core/mpool/mp_fnc_gtpl.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_igtp.o  core/mpool/mp_fnc_igtp.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_irel.o  core/mpool/mp_fnc_irel.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_mget.o  core/mpool/mp_fnc_mget.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_fnc_rels.o  core/mpool/mp_fnc_rels.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_rtn_getm.o  core/mpool/mp_rtn_getm.c
armcc -c --cpu Cortex-M3 -g -O%1 -I "c:\Keil\ARM\RV31\INC" --apcs=interwork --split_sections -o mp_rtn_putm.o  core/mpool/mp_rtn_putm.c

echo [TNKernel]: Portable layer...
armasm --cpu Cortex-M3 -g --apcs=interwork --cpreproc -I "c:\Keil\ARM\RV31\INC"              -o port.o         %PORT_PATH%/port.S

cd _lib
if exist tnkernel_%LIB_NAME%.a del tnkernel_%LIB_NAME%.a
cd ..

echo [TNKernel]: Build LIB for %LIB_NAME%

armar -r "_lib\tnkernel_%LIB_NAME%.a" cm_rtns.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" sy_dat.o sy_fce_stsy.o sy_fce_tiip.o sy_fce_tstk.o sy_fnc_stsy.o sy_fnc_tiip.o sy_fnc_tstk.o sy_rtn_itfn.o sy_rtn_ttfn.o sy_fcm_tist.o sy_fcm_tigt.o sy_fcm_cxgt.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" ts_fce_actv.o ts_fce_chpr.o ts_fce_crte.o ts_fce_delt.o ts_fce_exit.o ts_fce_iact.o ts_fce_ires.o ts_fce_irlw.o ts_fce_isup.o ts_fce_wakp.o ts_fce_term.o ts_fce_supd.o ts_fce_slep.o ts_fce_rlwt.o ts_fce_resm.o ts_fce_iwkp.o ts_fnc_wakp.o ts_fnc_term.o ts_fnc_supd.o ts_fnc_slep.o ts_fnc_rlwt.o ts_fnc_resm.o ts_fnc_iwkp.o ts_fnc_isup.o ts_fnc_irlw.o ts_fnc_ires.o ts_fnc_iact.o ts_fnc_exit.o ts_fnc_delt.o ts_fnc_crte.o ts_fnc_chpr.o ts_fnc_actv.o ts_rtn_crpr.o ts_rtn_cwac.o ts_rtn_fntr.o ts_rtn_scpr.o ts_rtn_sdms.o ts_rtn_tnrb.o ts_rtn_trbl.o ts_rtn_wcmp.o ts_fce_refr.o ts_fnc_refr.o ts_fce_iref.o ts_fnc_iref.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" sm_fce_asqu.o sm_fce_sign.o sm_fce_poll.o sm_fce_isig.o sm_fce_ipol.o sm_fce_delt.o sm_fce_crte.o sm_fnc_sign.o sm_fnc_poll.o sm_fnc_isig.o sm_fnc_ipol.o sm_fnc_delt.o sm_fnc_crte.o sm_fnc_asqu.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" ev_fce_cler.o ev_fce_wtpl.o ev_fce_wate.o ev_fce_sete.o ev_fce_iwat.o ev_fce_iset.o ev_fce_iclr.o ev_fce_delt.o ev_fce_crte.o ev_fnc_wtpl.o ev_fnc_wate.o ev_fnc_sete.o ev_fnc_iwat.o ev_fnc_iset.o ev_fnc_iclr.o ev_fnc_delt.o ev_fnc_crte.o ev_fnc_cler.o ev_rtn_sewq.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" mx_dat.o mx_fnc_unlk.o mx_fnc_lock.o mx_fnc_lkpl.o mx_fnc_delt.o mx_fnc_crte.o mx_fce_unlk.o mx_fce_lock.o mx_fce_lkpl.o mx_fce_delt.o mx_fce_crte.o mx_rtn_dulm.o mx_rtn_elmx.o mx_rtn_fmbp.o mx_rtn_rtbl.o mx_rtn_tlmx.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" dq_fce_crte.o dq_fce_delt.o dq_fce_ircv.o dq_fce_ispl.o dq_fce_rcpl.o dq_fce_recv.o dq_fce_send.o dq_fce_sepl.o dq_fnc_crte.o dq_fnc_delt.o dq_fnc_ircv.o dq_fnc_ispl.o dq_fnc_rcpl.o dq_fnc_recv.o dq_fnc_send.o dq_fnc_sepl.o dq_rtn_fird.o dq_rtn_fiwr.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" mp_fce_crte.o mp_fce_delt.o mp_fce_gtpl.o mp_fce_igtp.o mp_fce_irel.o mp_fce_mget.o mp_fce_rels.o mp_fnc_crte.o mp_fnc_delt.o mp_fnc_gtpl.o mp_fnc_igtp.o mp_fnc_irel.o mp_fnc_mget.o mp_fnc_rels.o mp_rtn_getm.o mp_rtn_putm.o
armar -r "_lib\tnkernel_%LIB_NAME%.a" port.o

if exist *.o del *.o

echo [TNKernel]: DONE...
echo.
echo.
goto :end

:end
