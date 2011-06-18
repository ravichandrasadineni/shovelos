/*
 * lapic.c
 *
 *  Created on: Apr 2, 2011
 *      Author: cds
 */

#include "lapic.h"
#include "msr.h"

enum local_apic_register {

	LAPICID   = 0x00000020, /* Read/Write - Local Apic ID Register */
	LAPICVER  = 0x00000030, /* Read  Only - Local Apic Version Register */
	LAPICTPR  = 0x00000080, /* Read/Write - task priority register */
	LAPICAPR  = 0x00000090, /* Read  Only - arbitration priority register */
	LAPICPPR  = 0x000000A0, /* Read  Only - processor priority register */
	LAPICEOI  = 0x000000B0, /* Write Only - end of interrupt */
	LAPICRRD  = 0x000000C0, /* Read  Only - remote read register */
	LAPICLDR  = 0x000000D0, /* Read/Write - local destination register */
	LAPICDFR  = 0x000000E0, /* Read/Write - destination format register */

	LAPICSPUR = 0x000000F0, /* Read/Write - Spurious Interrupt Vector Register */

	LAPICISR0 = 0x00000100, /* Read Only  - In-Service Register (ISR); bits 31:0 */
	LAPICISR1 = 0x00000110, /* Read Only  - In-Service Register (ISR); bits 63:32 */
	LAPICISR2 = 0x00000120, /* Read Only  - In-Service Register (ISR); bits 95:64 */
	LAPICISR3 = 0x00000130, /* Read Only  - In-Service Register (ISR); bits 127:96 */
	LAPICISR4 = 0x00000140, /* Read Only  - In-Service Register (ISR); bits 159:128 */
	LAPICISR5 = 0x00000150, /* Read Only  - In-Service Register (ISR); bits 191:160 */
	LAPICISR6 = 0x00000160, /* Read Only  - In-Service Register (ISR); bits 223:192 */
	LAPICISR7 = 0x00000170, /* Read Only  - In-Service Register (ISR); bits 255:224 */

	LAPICTMR0 = 0x00000180, /* Read Only  - Trigger Mode Register (TMR); bits 31:0 */
	LAPICTMR1 = 0x00000190, /* Read Only  - Trigger Mode Register (TMR); bits 63:32 */
	LAPICTMR2 = 0x000001A0, /* Read Only  - Trigger Mode Register (TMR); bits 95:64 */
	LAPICTMR3 = 0x000001B0, /* Read Only  - Trigger Mode Register (TMR); bits 127:96 */
	LAPICTMR4 = 0x000001C0, /* Read Only  - Trigger Mode Register (TMR); bits 159:128 */
	LAPICTMR5 = 0x000001D0, /* Read Only  - Trigger Mode Register (TMR); bits 191:160 */
	LAPICTMR6 = 0x000001E0, /* Read Only  - Trigger Mode Register (TMR); bits 223:192 */
	LAPICTMR7 = 0x000001F0, /* Read Only  - Trigger Mode Register (TMR); bits 255:224 */

	LAPICIRR0 = 0x00000200, /* Read Only  - Interrupt Request Register (IRR); bits 31:0 */
	LAPICIRR1 = 0x00000210, /* Read Only  - Interrupt Request Register (IRR); bits 63:32 */
	LAPICIRR2 = 0x00000220, /* Read Only  - Interrupt Request Register (IRR); bits 95:64 */
	LAPICIRR3 = 0x00000230, /* Read Only  - Interrupt Request Register (IRR); bits 127:96 */
	LAPICIRR4 = 0x00000240, /* Read Only  - Interrupt Request Register (IRR); bits 159:128 */
	LAPICIRR5 = 0x00000250, /* Read Only  - Interrupt Request Register (IRR); bits 191:160 */
	LAPICIRR6 = 0x00000260, /* Read Only  - Interrupt Request Register (IRR); bits 223:192 */
	LAPICIRR7 = 0x00000270, /* Read Only  - Interrupt Request Register (IRR); bits 255:224 */

	LAPICESR  = 0x00000280, /* Read Only  - Error Status Register */

	LAPICLVT  = 0x000002F0, /* Read/Write - LVT CMCI Register */
	LAPICICR  = 0x00000300, /* Read/Write - Interrupt Command Register (ICR); bits 0-31 */

	/*** TODO - MORE ***/
};

