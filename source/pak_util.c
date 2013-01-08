/*****************************************************************************\
 *
 * PAK file utilities
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: pak_util.c,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This source file describes functions to read and work with PAK files
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/


#include "pak_util.h"

/*---------------------------------------------------------------------------*/

PAK_file *pak_fopen(const char *fname, FILE *PAK_archive)
{
  PAK_file *file;
  uint32_t dirOffset, dirSize;
  char tmpName[56];
  int i;
  int foundIt = 0;

  file = (PAK_file *)malloc(sizeof(PAK_file));
  if(!file)
  {
    fprintf(stderr, "Not enough memory\n");
    return (PAK_file *)NULL;
  }
  file->PAK_archive = PAK_archive;
  fseek(PAK_archive, 4, SEEK_SET);
  fread(&dirOffset, 4, 1, PAK_archive);
  fread(&dirSize, 4, 1, PAK_archive);
  fseek(PAK_archive, dirOffset, SEEK_SET);

  for(i=0;i<((signed int)dirSize/64);i++)
  {
    fread(tmpName, 56, 1, PAK_archive);
    fread(&file->offset, 4, 1, PAK_archive);
    file->filepos = file->offset;
    fread(&file->size, 4, 1, PAK_archive);
    if(strcmp(tmpName, fname) == 0)
    {
      foundIt = 1;
      break;
    }
  }

  if(!foundIt)
  {
    pak_fclose(file);
    file = NULL;
  }
  return file;
}

/*---------------------------------------------------------------------------*/

void pak_fclose(PAK_file *stream)
{
  free(stream);
}

/*---------------------------------------------------------------------------*/

size_t pak_fread(void *ptr, size_t size, size_t nmemb, PAK_file *stream)
{
  fseek(stream->PAK_archive, stream->filepos, SEEK_SET);
  int readBytes = fread(ptr, size, nmemb, stream->PAK_archive);
  stream->filepos += readBytes;
  return readBytes;
}

/*---------------------------------------------------------------------------*/

int pak_fseek(PAK_file *stream, long offset, int whence)
{
  switch(whence)
  {
    case SEEK_SET:
      fseek(stream->PAK_archive, stream->offset + offset, SEEK_SET);
      stream->filepos = stream->offset + offset;
      break;
    case SEEK_CUR:
      fseek(stream->PAK_archive, stream->filepos + offset, SEEK_SET);
      stream->filepos += offset;
      break;
    case SEEK_END:
      fseek(stream->PAK_archive, stream->offset + stream->size + offset,
	  SEEK_SET);
      stream->filepos = stream->offset + stream->size + offset;
      break;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

long pak_ftell(PAK_file *stream)
{
  return stream->filepos;
}
