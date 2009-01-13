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

#ifndef TASKSPEC_PARSER_H
#define TASKSPEC_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif


/* current task spec version string */
#define CURRENT_VERSTR "RL-Glue-3.0"

/* problem types */
#define TS_EPISODIC   1
#define TS_CONTINUING 2
#define TS_OTHER      0

/* special range values */
#define RV_NOTSPECIAL 0
#define RV_NEGINF     1
#define RV_POSINF     2
#define RV_UNSPEC     3 /* unspecified */


typedef struct {
	int min, max;
	char special_min, special_max;
} int_range_t;

typedef struct {
	double min, max;
	char special_min, special_max;
} double_range_t;


typedef struct {
	char *version;              /* task spec version string */
	char problem_type;
	double discount_factor;
	int num_int_obs;            /* number of integral observation dimensions */
	int_range_t *int_obs;       /* array of length num_int_obs,
								   specifying range of each dimension */
	int num_double_obs;         /* number of real (floating point) observation dimensions */
	double_range_t *double_obs; /* array of length num_double_obs,
								   specifying range of each dimension */
	int charcount_obs;          /* number of characters in observation */
	int num_int_act;            /* number of integral action dimensions */
	int_range_t *int_act;       /* array of length num_int_act,
								   specifying range of each dimension */
	int num_double_act;         /* number of real (floating point) action dimensions */
	double_range_t *double_act; /* array of length num_double_act,
								   specifying range of each dimension */
	int charcount_act;          /* number of characters in action */
	double_range_t reward;      /* range of (environmentally determined) reward */
	char *extra_spec;           /* string of extra specifications (not parsed) */
} taskspec_t;


/* int dec_taskspec( taskspec_t *ts, const char *ts_str )
 * 
 * Decode (i.e., parse) a given task specification string.
 *
 * Returns 0 on success,
 *         1 if task spec version is unsupported or not recognized, or
 *        -1 on failure.
 */
int dec_taskspec( taskspec_t *ts, const char *ts_str );

/* enc_taskspec( const taskspec_t *ts, char *ts_str, size_t buf_len )
 * 
 * Encode (i.e., generate) a task specification string given a task
 * spec structure. The given string buffer (in which to write the
 * result) is assumed to have length buf_len.
 *
 * Returns 0 on success, -1 on failure.
 */
int enc_taskspec( const taskspec_t *ts, char *ts_str, size_t buf_len );

/* int free_taskspec_struct( taskspec_t *ts )
 *
 * Free any dynamically allocated arrays in the given task spec
 * structure. This is available merely for convenience, but be careful
 * not to give it a bad task spec struct.
 *
 * Returns 0 on success, -1 on failure.
 */
int free_taskspec_struct( taskspec_t *ts );


#ifdef __cplusplus
}
#endif

#endif
