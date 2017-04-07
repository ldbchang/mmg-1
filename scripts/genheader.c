/* =============================================================================
**  This file is part of the mmg software package for the tetrahedral
**  mesh modification.
**  Copyright (c) Bx INP/Inria/UBordeaux/UPMC, 2004- .
**
**  mmg is free software: you can redistribute it and/or modify it
**  under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  mmg is distributed in the hope that it will be useful, but WITHOUT
**  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
**  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
**  License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License and of the GNU General Public License along with mmg (in
**  files COPYING.LESSER and COPYING). If not, see
**  <http://www.gnu.org/licenses/>. Please read their terms carefully and
**  use this copy of the mmg distribution only if you accept them.
** =============================================================================
*/

/****************************************
 * genheader.c
 *
 * generate an executable to generate
 * headers for Fortran mmg users.
 * Used by makefile
 ****************************************/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/*
  Program: genheader

  Generate headers (for fortran) setting MMG5_DATA_PTR
  macro to the correct value.

  Parameters:
  argc - must be 3
  argv - contains the names wanted for the Fortran header

  Returns:
  EXIT_SUCCESS

*/
int main (int argc, char ** argv)
{
  char * header_f       = NULL;
  char * libmmg_h       = NULL;
  char * libmmg_include = NULL;
  char * genfort        = NULL;
  char * cmd            = NULL;
  FILE * file           = NULL;

  if (argc != 5)
    {
      fprintf(stderr, "usage : %s headerNameFortran.h headerNameC.h"
              " libmmgtypesf_include_dir genfort.pl\n",argv[0]);
      return EXIT_FAILURE;
    }

  header_f       = argv[1];
  libmmg_h       = argv[2];
  libmmg_include = argv[3];
  genfort        = argv[4];

  /* Fortran header */
  file = fopen (header_f,"w");

  if ( file == NULL )
    return(EXIT_FAILURE);

  fprintf(file,"! /* This file is automatically generated by the"
          "\"genheader.c\" code\n");
  fprintf(file,"! ** and the \"genfort.pl\" script (scripts directory).\n");
  fprintf(file,"! ** Do not modified it by hand, it will be discarded.\n");
  fprintf(file,"! **\n");
  fprintf(file,"! ** This scripts recopy the macros of the libmmg*.h file\n");
  fprintf(file,"! ** and convert enumerations into macros.\n");
  fprintf(file,"! **\n");
  fprintf(file,"! ** Note: \"genfort.pl\" is automatically called by the ");
  fprintf(file,"\"genheader.c\"\n! ** executable code.\n");
  fprintf(file,"! **\n");
  fprintf(file,"! ** See the \"libmmg*.h\" file for a correct displaying of"
          " the documentation.\n");
  fprintf(file,"! */\n\n");

  /* Include libmmgtypesf.h if needed. */
  if(strstr(libmmg_h,"libmmgtypes.h")==NULL) {
    fprintf(file,"#include \"%s/libmmgtypesf.h\"\n\n",libmmg_include);
  }
  else {
    /* Compute the size of the C pointer for the Fortran programm */
    fprintf(file, "#define MMG5_DATA_PTR_T INTEGER(kind=%d)\n",
            (int)sizeof(void*));
  }
  fclose(file);

  /* Generate Fortran header */
  if (NULL == (cmd = (char*)malloc((strlen(genfort)+
                                    strlen(libmmg_h)+
                                    strlen(header_f)+128)*sizeof(char))))
    return EXIT_FAILURE;
  sprintf(cmd, "perl %s -f %s >> %s;",
          genfort, libmmg_h, header_f);
  fprintf(stdout, "%s\n", cmd);
  if (-1 == system(cmd))
    return EXIT_FAILURE;
  free(cmd);

  return(0);
}
