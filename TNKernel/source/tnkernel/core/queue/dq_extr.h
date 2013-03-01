/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        dq_extr.h
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


#ifndef __DQ_EXTR_H
#define __DQ_EXTR_H

TN_RETVAL tnec_queue_create(TN_DQUE_S *dque, void **data_fifo, TN_UWORD num_entries);
TN_RETVAL tnec_queue_delete(TN_DQUE_S *dque);

TN_RETVAL tnec_queue_receive(TN_DQUE_S * dque, void **data_ptr, TN_TIMEOUT timeout);
TN_RETVAL tnec_queue_receive_polling(TN_DQUE_S *dque, void **data_ptr);
TN_RETVAL tnec_queue_ireceive(TN_DQUE_S *dque, void **data_ptr);

TN_RETVAL tnec_queue_send(TN_DQUE_S *dque, void *data_ptr, TN_TIMEOUT timeout);
TN_RETVAL tnec_queue_send_polling(TN_DQUE_S *dque, void *data_ptr);
TN_RETVAL tnec_queue_isend_polling(TN_DQUE_S *dque, void *data_ptr);


TN_RETVAL tnnc_queue_create(TN_DQUE_S *dque, void **data_fifo, TN_UWORD num_entries);
TN_RETVAL tnnc_queue_delete(TN_DQUE_S *dque);

TN_RETVAL tnnc_queue_receive(TN_DQUE_S * dque, void **data_ptr, TN_TIMEOUT timeout);
TN_RETVAL tnnc_queue_receive_polling(TN_DQUE_S *dque, void **data_ptr);
TN_RETVAL tnnc_queue_ireceive(TN_DQUE_S *dque, void **data_ptr);

TN_RETVAL tnnc_queue_send(TN_DQUE_S *dque, void *data_ptr, TN_TIMEOUT timeout);
TN_RETVAL tnnc_queue_send_polling(TN_DQUE_S *dque, void *data_ptr);
TN_RETVAL tnnc_queue_isend_polling(TN_DQUE_S *dque, void *data_ptr);

#endif  /* __DQ_EXTR_H */
