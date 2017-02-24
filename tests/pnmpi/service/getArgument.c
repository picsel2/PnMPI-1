/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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


#ifndef TEST_ARGUMENT
#define TEST_ARGUMENT "foo"
#endif

#ifndef TEST_MODULE
#define TEST_MODULE 0
#endif


void PNMPI_RegistrationPoint()
{
  /* Only the first module in the stack should execute this test. Other handles
   * will be used to store arguments only. */
  int self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    pnmpi_error("Can't get module ID.\n");
  if (self != 0)
    return;

  const char *buffer;
  int ret = PNMPI_Service_GetArgument(TEST_MODULE, TEST_ARGUMENT, &buffer);
  switch (ret)
    {
    case PNMPI_SUCCESS:
      printf("GetArgument: %s=%s\n", TEST_ARGUMENT, buffer);
      break;
    case PNMPI_NOMODULE:
    case PNMPI_NOARG:
      pnmpi_warning("GetArgument: %s\n", PNMPI_Service_strerror(ret));
      break;

    default: pnmpi_error("Unknown error: %d\n", ret); break;
    }
}


/* CONFIGS: found_self found_other found_second not_found no_module
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: argument foo bar\n
 * PNMPICONF: argument bar foo\n
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: argument hello world\n
 *
 * RUN: @PNMPIZE@ -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ @TESTBIN_MPI_C@
 *
 * PASS-found_self: GetArgument: foo=bar
 *
 * COMPILE_FLAGS-found_other: -DTEST_MODULE=1 -DTEST_ARGUMENT=\"hello\"
 * PASS-found_other: GetArgument: hello=world
 *
 * COMPILE_FLAGS-found_second: -DTEST_ARGUMENT=\"bar\"
 * PASS-found_second: GetArgument: bar=foo
 *
 * COMPILE_FLAGS-not_found: -DTEST_ARGUMENT=\"hello\"
 * PASS-not_found: GetArgument: Argument not found
 *
 * COMPILE_FLAGS-no_module: -DTEST_MODULE=999
 * PASS-no_module: GetArgument: Module not found
 */
