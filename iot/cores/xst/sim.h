
#ifndef _XST_SIM_H_
#define _XST_SIM_H_

#include <XST_IOT.h>

typedef struct {
	__IO uint32_t DATA[4];
} XSTIOT_Registers_TypeDef;

#define XSTIOT_Registers_BASE (CMSDK_APB_BASE + 0xEC00)
#define XSTIOT_Registers      ((XSTIOT_Registers_TypeDef *) XSTIOT_Registers_BASE)

#endif // _XST_SIM_H_
