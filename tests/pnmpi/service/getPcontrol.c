/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2018 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2018 RWTH Aachen University, Federal Republic of Germany
 *
 *
 * P^nMPI is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation version 2.1 dated February 1999.
 *
 * P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with P^nMPI; if not, write to the
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin St, Fifth Floor
 *   Boston, MA 02110, USA
 *
 *
 * Written by Martin Schulz, schulzm@llnl.gov.
 *
 * LLNL-CODE-402774
 */

/* This test case will check, if the module is able to query arguments. There
 * will be tests for querying the same and a different module for valid
 * arguments and querying an unknown argument in this test file. */

#include <stdio.h>
#include <stdlib.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#ifndef TEST_MODULE
#define TEST_MODULE 0
#endif


void PNMPI_RegistrationPoint()
{
  /* Only the first module in the stack should execute this test. Other handles
   * will be used to store arguments only. */
  int self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    PNMPI_Error("Can't get module ID.\n");
  if (self != 0)
    return;

  int buffer;
  int ret = PNMPI_Service_GetPcontrol(TEST_MODULE, &buffer);
  switch (ret)
    {
    case PNMPI_SUCCESS: printf("GetPcontrol: %d\n", buffer); break;
    case PNMPI_NOMODULE:
      PNMPI_Warning("GetPcontrol: %s\n", PNMPI_Service_strerror(ret));
      break;

    default: PNMPI_Error("Unknown error: %d\n", ret); break;
    }
}


/* CONFIGS: self other no_module
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: pcontrol on\n
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: pcontrol off\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * PASS-self: GetPcontrol: 1
 *
 * COMPILE_FLAGS-other: -DTEST_MODULE=1
 * PASS-other: GetPcontrol: 0
 *
 * COMPILE_FLAGS-no_module: -DTEST_MODULE=999
 * PASS-no_module: GetPcontrol: Module not found
 */
