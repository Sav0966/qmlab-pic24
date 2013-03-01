/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        port_extr.h
 *  Component:   PORT
 *  Compiler:    Microchip C30 v.2.xx - 3.11 (PIC24/dsPIC)
 *
 *  Ver. 2.5
 *
 *  Copyright © 2004,2008 Yuri Tiomkin
 *  Copyright © 2008 Alex Borisov (PIC24/dsPIC port)
 *
 *  All rights reserved.
 *
 *  Permission to use, copy, modify, and distribute this software in source and binary forms and its documentation
 *  for any purpose and without fee is hereby granted, provided that the above copyright notice appear in all copies
 *  and that both that copyright notice and this permission notice appear in supporting documentation.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PORT_EXTR_H
#define __PORT_EXTR_H

#if defined(__dsPIC30F__) || defined(__dsPIC33F__)
    #include "mchp_c30_dspic\\port_defs.h"
#elif defined(__PIC24F__) || defined(__PIC24H__)
    #include "mchp_c30_pic24\\port_defs.h"
#elif defined(__PIC32MX__)
    #include "mchp_c32_pic32\\port_defs.h"
#elif (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
    #include "nxp_rv_cortexm3\\port_defs.h"
#else
    #error "Please define the CORE"
#endif


/* ================================================================================================================== */
#endif  /* __PORT_EXTR_H */
