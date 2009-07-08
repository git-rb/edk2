/** @file
  Provides the parent dispatch service for the power button SMI source generator.

  @par Revision Reference:
  This Protocol is defined in Framework of EFI SMM Core Interface Spec
  Version 0.9.

Copyright (c) 2007, Intel Corporation
All rights reserved. This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EFI_SMM_POWER_BUTTON_DISPATCH_H_
#define _EFI_SMM_POWER_BUTTON_DISPATCH_H_

#include <PiDxe.h>

//
// Global ID for the Power Button SMI Protocol
//
#define EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL_GUID \
  { \
    0xb709efa0, 0x47a6, 0x4b41, {0xb9, 0x31, 0x12, 0xec, 0xe7, 0xa8, 0xee, 0x56 } \
  }

typedef struct _EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL  EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL;

//
// Related Definitions
//
//
// Power Button. Example, Use for changing LEDs before ACPI OS is on.
//    - DXE/BDS Phase
//    - OS Install Phase
//
typedef enum {
  PowerButtonEntry,
  PowerButtonExit
} EFI_POWER_BUTTON_PHASE;

typedef struct {
  EFI_POWER_BUTTON_PHASE  Phase;
} EFI_SMM_POWER_BUTTON_DISPATCH_CONTEXT;

//
// Member functions
//
/**
  Dispatch function for a Power Button SMI handler.

  @param[in]  DispatchHandle        Handle of this dispatch function.
  @param[in]  DispatchContext       Pointer to the dispatch function's context.
                                    The DispatchContext fields are filled in
                                    by the dispatching driver prior to
                                    invoking this dispatch function.

**/
typedef
VOID
(EFIAPI *EFI_SMM_POWER_BUTTON_DISPATCH)(
  IN  EFI_HANDLE                             DispatchHandle,
  IN  EFI_SMM_POWER_BUTTON_DISPATCH_CONTEXT  *DispatchContext
  );

/**
  Provides the parent dispatch service for a given SMI source generator

  @param[in]   This                  Pointer to the EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL instance.
  @param[in]   DispatchFunction      Function to install.
  @param[in]   DispatchContext       Pointer to the dispatch function's context.
                                     The caller fills this context in before calling
                                     the register function to indicate to the register
                                     function the Power Button SMI phase for which the dispatch
                                     function should be invoked.
  @param[out]  DispatchHandle        Handle generated by the dispatcher to track the function instance.

  @retval      EFI_SUCCESS           The dispatch function has been successfully
                                     registered and the SMI source has been enabled.
  @retval      EFI_DEVICE_ERROR      The driver was unable to enable the SMI source.
  @retval      EFI_OUT_OF_RESOURCES  Not enough memory (system or SMM) to manage this
                                     child.
  @retval      EFI_INVALID_PARAMETER DispatchContext is invalid. The Power Button SMI
                                     phase is not within valid range.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_POWER_BUTTON_REGISTER)(
  IN EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL           *This,
  IN EFI_SMM_POWER_BUTTON_DISPATCH                    DispatchFunction,
  IN EFI_SMM_POWER_BUTTON_DISPATCH_CONTEXT            *DispatchContext,
  OUT EFI_HANDLE                                      *DispatchHandle
  );

/**
  Unregisters a power-button service.

  @param[in]  This                  Pointer to the EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL instance.
  @param[in]  DispatchHandle        Handle of the service to remove.

  @retval     EFI_SUCCESS           The dispatch function has been successfully
                                    unregistered and the SMI source has been
				    disabled if there are no other registered
				    child dispatch functions for this SMI
				    source.
  @retval     EFI_INVALID_PARAMETER Handle is invalid.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_POWER_BUTTON_UNREGISTER)(
  IN EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL            *This,
  IN EFI_HANDLE                                        DispatchHandle
  );

/**
  @par Protocol Description:
  Provides the parent dispatch service for the SMM power button SMI source generator.

**/
struct _EFI_SMM_POWER_BUTTON_DISPATCH_PROTOCOL {
  ///
  ///  Installs a child service to be dispatched by this protocol.
  ///
  EFI_SMM_POWER_BUTTON_REGISTER   Register;

  ///
  ///  Removes a child service dispatched by this protocol.
  ///
  EFI_SMM_POWER_BUTTON_UNREGISTER UnRegister;
};

extern EFI_GUID gEfiSmmPowerButtonDispatchProtocolGuid;

#endif