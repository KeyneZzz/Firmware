/****************************************************************************
 * binfmt/symtab_findorderedbyvalue.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stddef.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <nuttx/symtab.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/***********************************************************************
 * Public Functions
 ***********************************************************************/

/****************************************************************************
 * Name: symtab_findorderedbyvalue
 *
 * Description:
 *   Find the symbol in the symbol table whose value closest (but not greater
 *   than), the provided value. This version assumes that table is ordered
 *   with respect to symbol name.
 *
 * Returned Value:
 *   A reference to the symbol table entry if an entry with the matching
 *   name is found; NULL is returned if the entry is not found.
 *
 ****************************************************************************/

FAR const struct symtab_s *
symtab_findorderedbyvalue(FAR const struct symtab_s *symtab,
                          FAR void *value, int nsyms)
{
  int low  = 0;
  int high = nsyms -1;
  int mid;

  /* Loop until the range has been isolated to a single symbol table
   * entry that may or may not match the search name.
   */

  DEBUGASSERT(symtab != NULL);
  while (low < high)
    {
      /* Compare the name to the one in the middle.  (or just below
       * the middle in the case where one is even and one is odd).
       */

      mid = (low + high) >> 1;
      if ( value < symtab[mid].sym_value)
        {
          high = mid - 1;
        }
      else if (value > symtab[mid].sym_value)
        {
          low = mid + 1;
        }
      else /* if (value == symtab[mid].sym_value) */
        {
          return &symtab[mid];
        }
    }

 /* low == high... One final check.  We might not have actually tested
  * the final symtab[] name.
  *
  *   Example: Only the last pass through loop, suppose low = 1, high = 2,
  *   mid = 1, and symtab[high].sym_name == name.  Then we would get here with
  *   low = 2, high = 2, but symtab[2].sym_name was never tested.
  */

  return value == symtab[low].sym_value ? &symtab[low] : NULL;
}

