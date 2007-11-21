/*****************************************************************************\
 *
 * PAK file utilities
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: pak_util.h,v 1.1 2003/02/22 19:48:57 totto Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you do not find the GNU General public license in the file 'GPL'
 * which should be distributed along with the rest of this program,
 * you can normally find it at http://www.gnu.org/licenses/gpl.html
 *
 *
 * If you have comments, suggestions or otherwise wish to contact
 * me, write an email to:
 *
 * Tor Arvid Lund <totto@boredom.nu>
 *
\*****************************************************************************/


/*****************************************************************************\
 *
 * This header file describes functions to read and work with PAK files
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRCNCF 1    /* Could not create file */
#define ERRCNOF 2    /* Could not open file */
#define ERRNEM  4    /* Not enough memory */

/*---------------------------------------------------------------------------*/

typedef struct{
  char name[56];
  unsigned long int offset;
  unsigned long int size;
}PAK_DirEntry;

typedef struct{
  FILE *PAK_archive;
  unsigned long int offset;
  unsigned long int size;
  long filepos;
}PAK_file;

/* The purpose of these functions is to make a (sort of) equivalent to
 * the well-known functions (fopen, fseek, fread, ...) for reading
 * files */

/* This function returns a pointer toa file within a PAK archive */
PAK_file *pak_fopen(char *fname, FILE *PAK_archive);

/* This function frees the pointer to the PAK_file stream */
void pak_fclose(PAK_file *stream);

/* This function reads data from a file within a PAK archive */
size_t pak_fread(void *ptr, size_t size, size_t nmemb, PAK_file *stream);

/* This function sets the file position indicator */
int pak_fseek(PAK_file *stream, long offset, int whence);

/* This function returns the file position indicator */
long pak_ftell(PAK_file *stream);
