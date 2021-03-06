//
//  Copyright (c) 2011, ARM Limited. All rights reserved.
//  
//  This program and the accompanying materials                          
//  are licensed and made available under the terms and conditions of the BSD License         
//  which accompanies this distribution.  The full text of the license may be found at        
//  http://opensource.org/licenses/bsd-license.php                                            
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
//
//

#include <AsmMacroIoLib.h>
#include <Base.h>
#include <Library/PcdLib.h>
#include <AutoGen.h>

  INCLUDE AsmMacroIoLib.inc
  
  IMPORT  CEntryPoint
  IMPORT  ArmReadMpidr
  EXPORT  _ModuleEntryPoint
  
  PRESERVE8
  AREA    PrePeiCoreEntryPoint, CODE, READONLY
  
StartupAddr        DCD      CEntryPoint

_ModuleEntryPoint
  // Identify CPU ID
  bl    ArmReadMpidr
  // Get ID of this CPU in Multicore system
  LoadConstantToReg (FixedPcdGet32(PcdArmPrimaryCoreMask), r1)
  and   r5, r0, r1
  
  // Get the top of the primary stacks (and the base of the secondary stacks)
  LoadConstantToReg (FixedPcdGet32(PcdCPUCoresStackBase), r1)
  LoadConstantToReg (FixedPcdGet32(PcdCPUCorePrimaryStackSize), r2)
  add   r1, r1, r2

  // Is it the Primary Core ?
  LoadConstantToReg (FixedPcdGet32(PcdArmPrimaryCore), r3)
  cmp   r5, r3
  beq   _SetupPrimaryCoreStack

_SetupSecondaryCoreStack
  // r1 contains the base of the secondary stacks

  // Get the Core Position (ClusterId * 4) + CoreId
  GetCorePositionInStack(r0, r5, r2)
  // The stack starts at the top of the stack region. Add '1' to the Core Position to get the top of the stack
  add   r0, r0, #1

  // StackOffset = CorePos * StackSize
  LoadConstantToReg (FixedPcdGet32(PcdCPUCoreSecondaryStackSize), r2)
  mul   r0, r0, r2
  // SP = StackBase + StackOffset
  add   sp, r1, r0

_PrepareArguments
  // The PEI Core Entry Point has been computed by GenFV and stored in the second entry of the Reset Vector
  LoadConstantToReg (FixedPcdGet32(PcdFvBaseAddress), r2)
  add   r2, r2, #4
  ldr   r1, [r2]

  // Move sec startup address into a data register
  // Ensure we're jumping to FV version of the code (not boot remapped alias)
  ldr   r3, StartupAddr
  
  // Jump to PrePeiCore C code
  //    r0 = mp_id
  //    r1 = pei_core_address
  mov   r0, r5
  blx   r3

_SetupPrimaryCoreStack
  // r1 contains the top of the primary stack
  LoadConstantToReg (FixedPcdGet32(PcdPeiGlobalVariableSize), r2)

  // The reserved space for global variable must be 8-bytes aligned for pushing
  // 64-bit variable on the stack
  SetPrimaryStack (r1, r2, r3)
  b     _PrepareArguments

_NeverReturn
  b _NeverReturn

  END
