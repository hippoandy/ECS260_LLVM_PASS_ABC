#include <stdio.h>

/***
** Reimplement this function to check, and print Out-of-Bounds Errors
***/
void OutofBoundsError( unsigned long arr_size, unsigned long arr_index, unsigned long line_num, char* file_name ) {
  //printf( "Out-of-Bounds Checking...\n" );
  //printf( "%lu, %lu, %lu\n", arr_size, arr_index, line_num );
  if( arr_index >= arr_size || arr_index < 0 ) {
    printf( "%s:%lu: Array Out of Bounds Error (index %lu, size %lu).\n", file_name, line_num, arr_index, arr_size );
  }
}
