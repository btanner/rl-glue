/* 
 Copyright (C) 2009, Scott Livingston
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */

/* DESCRIPTION:
 * 
 * The purpose of this file is to test encoding and decoding task spec
 * strings using the C task spec parser. The function for printing
 * from the taskspec_t structure directly (i.e., without "encoding" it
 * into a task spec string) might be useful more generally outside of
 * this test file.
 *
 * Scott Livingston
 * 12 Jan 2009
 *
 */


#include <stdio.h>
#include <string.h>

#include <rlglue/utils/C/TaskSpec_Parser.h>


#define BUF_SIZE 1024

int tests_failed=0;
int test_count=0;

void check_fail(int condition){
	test_count++;
	if(condition!=0){
		printf("Failed check: %d\n",test_count);
		tests_failed++;
	}
}


int main( int argc, char **argv )
{
	FILE *test_fp;
	char *cp;
	char test_str[BUF_SIZE];
	char out_str[BUF_SIZE];
	taskspec_t ts;
	int dec_result;
	
	/* attempt to open the file of test task spec strings */
	if (argc == 1) {
		test_fp = fopen( "test_cases.txt", "r" );
	} else if (argc == 2) {
		test_fp = fopen( argv[1], "r" );
	} else {
		printf( "Usage: %s [test-strings-file]\n", argv[0] );
		return 1;
	}
	if (test_fp == NULL) {
		perror( "main, fopen" );
		return -1;
	}
	
	/* the read, test loop */
	while (!feof( test_fp )) {
		
		if (fgets( test_str, BUF_SIZE, test_fp ) == NULL)
			break;
		
		/* strip read line of a new-line character, if present */
		if ((cp = strchr( test_str, '\n' )) != NULL)
			*cp = '\0';
		
		printf( "Test string:      %s\n", test_str );
		dec_result = dec_taskspec( &ts, test_str );
		check_fail(dec_result);
		switch( dec_result ) {
				
			
			case 0:
				enc_taskspec( &ts, out_str, BUF_SIZE );
				free_taskspec_struct( &ts );
				printf( "Resulting string: %s\n\n", out_str );
				break;
				
			case 1:
				printf( "Unrecognized or unsupported task spec string version.\n\n" );
				break;
				
			case -1:
				fprintf( stderr, "dec_taskspec (task spec string parsing) failed.\n\n" );
				break;
				
			default:
				fprintf( stderr, "dec_taskspec returned: %d\n\n", dec_result );
				break;
				
		}
		
	}
	
	fclose( test_fp );
	
	if(tests_failed!=0)
		printf("Failed %d / %d checks in %s\n",tests_failed,test_count, __FILE__);
	else
		printf("Passed all %d checks in %s\n",test_count,__FILE__);
	return tests_failed;
	
}
