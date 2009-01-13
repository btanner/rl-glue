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


#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> /* used to detect errors in string-to-number conversions */

#include <rlglue/utils/C/TaskSpec_Parser.h>


/* for buffering during range processing. */
#define VECT_BUF_LEN 10

/* Some utility functions (not intended for use outside this file): */

/* Each "get dims" function returns a pointer to a dynamically
   allocated array and stores the array length to num_dims; If array
   length is zero, num_dims is set to 0 and NULL is returned. If an
   error occurs during parsing, NULL is returned and num_dims is set
   to -1.
*/
int_range_t *get_int_dims( const char *range_str, int *num_dims );
double_range_t *get_double_dims( const char *range_str, int *num_dims );

/* Return pointer to first non-space character in s;
   if no non-space character is found, returns NULL. */
char const *find_nonspace( const char *s );

/* Return pointer to space (white space character) immediately before
   first character of any of the possible task spec string entries
   that would indicate the end of a list of ranges: end_delim, INTS,
   DOUBLES, CHARCOUNT . Note that comparisons are (approximately) not
   case sensitive, e.g., either "INTS" or "ints" is valid. However,
   the end_delim string is searched for as is (i.e., comparison IS
   case sensitive).

   If no such location is found, returns NULL. */
char *find_end_ranges_list( const char *s, const char *end_delim );


int dec_taskspec( taskspec_t *ts, const char *ts_str )
{
	char const *cp, *cp_next; /* character pointers (for string parsing) */
	char *cp_candidate_next; /* when deciding between two possible
								"next" character pointers. */
	char *tmpstr; /* temporary strings for passing substrings of the 
					 task spec string to other functions. */

	char *endptr; /* used for detecting errors in string-to-double
					 conversion with strtod */

	/* set all pointers to NULL in the taskspec struct; this is
	   necessary for clean calls to free_taskspec_struct. */
	ts->version = NULL;
	ts->int_obs = NULL;
	ts->double_obs = NULL;
	ts->int_act = NULL;
	ts->double_act = NULL;
	ts->extra_spec = NULL;

	/* initialize all counts to zero */
	ts->num_int_obs = 0;
	ts->num_double_obs = 0;
	ts->charcount_obs = 0;
	ts->num_int_act = 0;
	ts->num_double_act = 0;
	ts->charcount_act = 0;

	/*
	 * Determine version:
	 *
	 */
	cp = find_nonspace( ts_str );
	if (cp == NULL
		|| strncasecmp( cp, "VERSION", strlen("VERSION") ))
		return -1;
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	if (cp == NULL)
		return -1;
	if (strncmp( cp, CURRENT_VERSTR, strlen(CURRENT_VERSTR) ))
		return 1; /* unrecognized or unsupported version */

	/* prepare to save copy of version string */
	cp_next = strchr( cp, ' ' );
	if (cp_next == NULL)
		return -1;

	/* save version string to taskspec struct */
	ts->version = (char *)malloc( cp_next-cp+1 );
	if (ts->version == NULL) /* insufficient memory!? */
		return -1;
	strncpy( ts->version, cp, cp_next-cp );
	cp = cp_next+1;

	/*
	 * Determine problem type:
	 *
	 */
	cp = find_nonspace( cp );
	if (cp == NULL
		|| strncasecmp( cp, "PROBLEMTYPE", strlen("PROBLEMTYPE") )) {
		free_taskspec_struct( ts );
		return -1;
	}
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	if (cp == NULL) {
		free_taskspec_struct( ts );
		return -1;
	}
	if (!strncmp( cp, "episodic", strlen("episodic") )) {
		ts->problem_type = TS_EPISODIC;
	} else if (!strncmp( cp, "continuing", strlen("continuing") )) {
		ts->problem_type = TS_CONTINUING;
	} else {
		ts->problem_type = TS_OTHER;
	}

	/*
	 * Determine discount factor:
	 *
	 */
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp ); 
	if (cp == NULL
		|| strncasecmp( cp, "DISCOUNTFACTOR", strlen("DISCOUNTFACTOR") )) {
		free_taskspec_struct( ts );
		return -1;
	}
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	if (cp == NULL || isalpha( *cp )) {
		free_taskspec_struct( ts );
		return -1;
	}
	cp_next = strchr( cp, ' ' );
	if (cp_next == NULL) {
		free_taskspec_struct( ts );
		return -1;
	}
	tmpstr = (char *)malloc( cp_next-cp+1 );
	if (tmpstr == NULL) { /* insufficient memory!? */
		free_taskspec_struct( ts );
		return -1;
	}
	strncpy( tmpstr, cp, cp_next-cp );
	errno = 0;
	ts->discount_factor = strtod( tmpstr, &endptr );
	if (ts->discount_factor == 0
		&& (endptr == tmpstr || errno == ERANGE)) {
		free( tmpstr );
		free_taskspec_struct( ts );
		return -1; /* given discount factor is invalid */
	}
	free( tmpstr );
	if (ts->discount_factor < 0. || ts->discount_factor > 1.) {
		free_taskspec_struct( ts );
		return -1; /* discount factor out of bounds */
	}
	cp = cp_next+1;

	/*
	 * Observation space specifications:
	 *
	 */
	cp = find_nonspace( cp );
	if (cp == NULL
		|| strncasecmp( cp, "OBSERVATIONS", strlen("OBSERVATIONS") )) {
		free_taskspec_struct( ts );
		return -1;
	}
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	while (cp != NULL
		   && strncasecmp( cp, "ACTIONS", strlen("ACTIONS") )) {

		/* determine dimension type */
		if (!strncasecmp( cp, "INTS", strlen("INTS") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );

			cp_next = find_end_ranges_list( cp, "ACTIONS" );
			cp_candidate_next = find_end_ranges_list( cp, "actions" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			tmpstr = (char *)malloc( cp_next-cp );
			if (tmpstr == NULL) { /* insufficient memory!? */
				free_taskspec_struct( ts );
				return -1;
			}
			strncpy( tmpstr, cp, cp_next-1-cp );
			ts->int_obs = get_int_dims( tmpstr, &(ts->num_int_obs) );
			free( tmpstr );
			if (ts->num_int_obs == -1) {
				free_taskspec_struct( ts );
				return -1;
			}

		} else if (!strncasecmp( cp, "DOUBLES", strlen("DOUBLES") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );

			cp_next = find_end_ranges_list( cp, "ACTIONS" );
			cp_candidate_next = find_end_ranges_list( cp, "actions" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			tmpstr = (char *)malloc( cp_next-cp );
			if (tmpstr == NULL) { /* insufficient memory!? */
				free_taskspec_struct( ts );
				return -1;
			}
			strncpy( tmpstr, cp, cp_next-1-cp );
			ts->double_obs = get_double_dims( tmpstr, &(ts->num_double_obs) );
			free( tmpstr );
			if (ts->num_double_obs == -1) {
				free_taskspec_struct( ts );
				return -1;
			}

		} else if (!strncasecmp( cp, "CHARCOUNT", strlen("CHARCOUNT") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );
			cp_next = find_end_ranges_list( cp, "ACTIONS" );
			cp_candidate_next = find_end_ranges_list( cp, "actions" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			/* read the character count integer */
			errno = 0;
			ts->charcount_obs = strtol( cp, &endptr, 0 );
			if (endptr == cp || errno == ERANGE
				|| ts->charcount_obs < 0) {
				free_taskspec_struct( ts );
				return -1; /* count read failed or count is negative */
			}

		} else { 

			free_taskspec_struct( ts );
			return -1; /* unrecognized dimension type */

		}

		cp = cp_next;

	}
	if (cp == NULL) {
		free_taskspec_struct( ts );
		return -1; /* early task spec string termination */
	}

	/*
	 * Action space specifications:
	 *
	 */
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	while (cp != NULL
		   && strncasecmp( cp, "REWARDS", strlen("REWARDS") )) {

		/* determine dimension type */
		if (!strncasecmp( cp, "INTS", strlen("INTS") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );

			cp_next = find_end_ranges_list( cp, "REWARDS" );
			cp_candidate_next = find_end_ranges_list( cp, "rewards" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			tmpstr = (char *)malloc( cp_next-cp );
			if (tmpstr == NULL) { /* insufficient memory!? */
				free_taskspec_struct( ts );
				return -1;
			}
			strncpy( tmpstr, cp, cp_next-1-cp );
			ts->int_act = get_int_dims( tmpstr, &(ts->num_int_act) );
			free( tmpstr );
			if (ts->num_int_act == -1) {
				free_taskspec_struct( ts );
				return -1;
			}

		} else if (!strncasecmp( cp, "DOUBLES", strlen("DOUBLES") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );

			cp_next = find_end_ranges_list( cp, "REWARDS" );
			cp_candidate_next = find_end_ranges_list( cp, "rewards" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			tmpstr = (char *)malloc( cp_next-cp );
			if (tmpstr == NULL) { /* insufficient memory!? */
				free_taskspec_struct( ts );
				return -1;
			}
			strncpy( tmpstr, cp, cp_next-1-cp );
			ts->double_act = get_double_dims( tmpstr, &(ts->num_double_act) );
			free( tmpstr );
			if (ts->num_double_act == -1) {
				free_taskspec_struct( ts );
				return -1;
			}

		} else if (!strncasecmp( cp, "CHARCOUNT", strlen("CHARCOUNT") )) {
			
			cp = strchr( cp, ' ' );
			cp = find_nonspace( cp );
			cp_next = find_end_ranges_list( cp, "REWARDS" );
			cp_candidate_next = find_end_ranges_list( cp, "rewards" );
			if (cp_next == NULL && cp_candidate_next == NULL) {
				free_taskspec_struct( ts );
				return -1; /* early task spec string termination */
			}
			if (cp_next == NULL
				|| (cp_candidate_next != NULL && cp_candidate_next < cp_next))
				cp_next = cp_candidate_next;

			if (cp == cp_next)
				continue;

			/* read the character count integer */
			errno = 0;
			ts->charcount_act = strtol( cp, &endptr, 0 );
			if (endptr == cp || errno == ERANGE
				|| ts->charcount_act < 0) {
				free_taskspec_struct( ts );
				return -1; /* count read failed or count is negative */
			}

		} else { 

			free_taskspec_struct( ts );
			return -1; /* unrecognized dimension type */

		}

		cp = cp_next;

	}
	if (cp == NULL) {
		free_taskspec_struct( ts );
		return -1; /* early task spec string termination */
	}

	/*
	 * Reward range:
	 *
	 */
	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	if (*cp != '(') {
		free_taskspec_struct( ts );
		return -1; /* malformed reward range */
	}
	cp = find_nonspace( cp+1 );
	if (cp == NULL || *cp == '\0') {
		free_taskspec_struct( ts );
		return -1; /* early task spec string termination */
	}
	
	if (isalpha( *cp )) { /* a special value? */

		if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
			ts->reward.special_min = RV_UNSPEC;
		} else if (!strncasecmp( cp, "NEGINF", strlen("NEGINF") )) {
			ts->reward.special_min = RV_NEGINF;
		} else {
			free_taskspec_struct( ts );
			return -1; /* malformed expression: unrecognized special type */
		}

	} else { /* attempt to extract the double */

		errno = 0;
		ts->reward.min = strtod( cp, &endptr );
		if (endptr == cp || errno == ERANGE) {
			free_taskspec_struct( ts );
			return -1; /* malformed expression: invalid double string */
		}

	}

	cp = strchr( cp, ' ' );
	cp = find_nonspace( cp );
	if (cp == NULL) {
		free_taskspec_struct( ts ); 
		return -1; /* early task spec string termination */
	}

	if (isalpha( *cp )) { /* a special value? */

		if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
			ts->reward.special_max = RV_UNSPEC;
		} else if (!strncasecmp( cp, "POSINF", strlen("POSINF") )) {
			ts->reward.special_max = RV_POSINF;
		} else {
			free_taskspec_struct( ts );
			return -1; /* malformed expression: unrecognized special type */
		}

	} else { /* attempt to extract the double */

		errno = 0;
		ts->reward.max = strtod( cp, &endptr );
		if (endptr == cp || errno == ERANGE) {
			free_taskspec_struct( ts );
			return -1; /* malformed expression: invalid double string */
		}

	}
	
	cp_next = strchr( cp, ')' );
	cp = strchr( cp, ' ' );
	if (cp == NULL || cp_next == NULL || *(cp_next+1) == '\0' ) {
		free_taskspec_struct( ts );
		return -1; /* early task spec string termination */
	}
	if (cp < cp_next) {
		cp = find_nonspace( cp );
		if (*cp != ')') {
			free_taskspec_struct( ts );
			return -1; /* invalid tuple size */
		}
	}
	cp = cp_next+1;

	/*
	 * Extra spec string:
	 *
	 */
	cp = find_nonspace( cp );
	if (cp == NULL
		|| strncasecmp( cp, "EXTRA", strlen("EXTRA") )) {
		free_taskspec_struct( ts );
		return -1; /* early task spec string termination */
	}
	cp = strchr( cp, ' ' );
	if (cp != NULL
		&& (cp = find_nonspace( cp )) != NULL) {
		ts->extra_spec = (char *)malloc( strlen(cp)+1 );
		if (ts->extra_spec == NULL) {
			free_taskspec_struct( ts );
			return -1; /* insufficient memory!? */
		}
		strncpy( ts->extra_spec, cp, strlen(cp) );
	}

	return 0;
}


int enc_taskspec( const taskspec_t *ts, char *ts_str, size_t buf_len )
{
	int i;
	char *cp, *cp_end;
	int nb; /* number of characters printed */

	if (buf_len < 2) /* ignore impractical buffer sizes */
		return -1;

	cp = ts_str; /* step through the buffer as the task spec string is built */
	cp_end = ts_str+buf_len-1; /* point to end of available string buffer */

	if (ts->version != NULL) {
		nb = snprintf( cp, cp_end-cp, "VERSION %s ", ts->version );
		if (nb < strlen("VERSION ")+strlen(ts->version)+1
			|| cp+nb > ts_str+buf_len-2)
			return -1;
		cp += nb;
	} else {
		return -1;
	}

	switch (ts->problem_type) {
	case TS_EPISODIC:
		nb = snprintf( cp, cp_end-cp, "PROBLEMTYPE episodic " );
		if (nb < strlen("PROBLEMTYPE episodic ")
			|| cp+nb >= cp_end)
			return -1;
		break;
	case TS_CONTINUING:
		nb = snprintf( cp, cp_end-cp, "PROBLEMTYPE continuing " );
		if (nb < strlen("PROBLEMTYPE continuing ")
			|| cp+nb >= cp_end)
			return -1;
		break;
	case TS_OTHER:
		nb = snprintf( cp, cp_end-cp, "PROBLEMTYPE other " );
		if (nb < strlen("PROBLEMTYPE other ")
			|| cp+nb >= cp_end)
			return -1;
		break;
	default:
		return -1;
	}
	cp += nb;

	nb = snprintf( cp, cp_end-cp, "DISCOUNTFACTOR %f ", ts->discount_factor );
	if (nb < strlen("DISCOUNTFACTOR ") || cp+nb >= cp_end)
		return -1;
	cp += nb;

	/*
	 * Print observations structure:
	 *
	 */
	nb = snprintf( cp, cp_end-cp, "OBSERVATIONS " );
	if (nb < strlen("OBSERVATIONS ") || cp+nb >= cp_end)
		return -1;
	cp += nb;
	
	/* print observation INTS */
	if (ts->num_int_obs > 0) {
		nb = snprintf( cp, cp_end-cp, "INTS " );
		if (nb < strlen("INTS ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
		for (i = 0; i < ts->num_int_obs; i++) {
			nb = snprintf( cp, cp_end-cp, "(" );
			if (nb < 1 || cp+nb >= cp_end)
				return -1;
			cp += nb;
			if ((ts->int_obs+i)->special_min == RV_NEGINF) {
				nb = snprintf( cp, cp_end-cp, "NEGINF, " );
				if (nb < strlen("NEGINF, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->int_obs+i)->special_min == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC, " );
				if (nb < strlen("UNSPEC, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%d, ", (ts->int_obs+i)->min );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
			if ((ts->int_obs+i)->special_max == RV_POSINF) {
				nb = snprintf( cp, cp_end-cp, "POSINF) " );
				if (nb < strlen("POSINF) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->int_obs+i)->special_max == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC) " );
				if (nb < strlen("UNSPEC) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%d) ", (ts->int_obs+i)->max );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
		}
	}

	/* print observation DOUBLES */
	if (ts->num_double_obs > 0) {
		nb = snprintf( cp, cp_end-cp, "DOUBLES " );
		if (nb < strlen("DOUBLES ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
		for (i = 0; i < ts->num_double_obs; i++) {
			nb = snprintf( cp, cp_end-cp, "(" );
			if (nb < 1 || cp+nb >= cp_end)
				return -1;
			cp += nb;
			if ((ts->double_obs+i)->special_min == RV_NEGINF) {
				nb = snprintf( cp, cp_end-cp, "NEGINF, " );
				if (nb < strlen("NEGINF, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->double_obs+i)->special_min == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC, " );
				if (nb < strlen("UNSPEC, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%f, ", (ts->double_obs+i)->min );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
			if ((ts->double_obs+i)->special_max == RV_POSINF) {
				nb = snprintf( cp, cp_end-cp, "POSINF) " );
				if (nb < strlen("POSINF) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->double_obs+i)->special_max == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC) " );
				if (nb < strlen("UNSPEC) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%f) ", (ts->double_obs+i)->max );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
		}
	}

	if (ts->charcount_obs > 0) {
		nb = snprintf( cp, cp_end-cp, "CHARCOUNT %d ", ts->charcount_obs );
		if (nb < strlen("CHARCOUNT ")+1 || cp+nb >= cp_end)
			return -1;
		cp += nb;
	}

	/*
	 * Print actions structure:
	 *
	 */
	nb = snprintf( cp, cp_end-cp, "ACTIONS " );
	if (nb < strlen("ACTIONS ") || cp+nb >= cp_end)
		return -1;
	cp += nb;
	
	/* print action INTS */
	if (ts->num_int_act > 0) {
		nb = snprintf( cp, cp_end-cp, "INTS " );
		if (nb < strlen("INTS ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
		for (i = 0; i < ts->num_int_act; i++) {
			nb = snprintf( cp, cp_end-cp, "(" );
			if (nb < 1 || cp+nb >= cp_end)
				return -1;
			cp += nb;
			if ((ts->int_act+i)->special_min == RV_NEGINF) {
				nb = snprintf( cp, cp_end-cp, "NEGINF, " );
				if (nb < strlen("NEGINF, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->int_act+i)->special_min == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC, " );
				if (nb < strlen("UNSPEC, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%d, ", (ts->int_act+i)->min );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
			if ((ts->int_act+i)->special_max == RV_POSINF) {
				nb = snprintf( cp, cp_end-cp, "POSINF) " );
				if (nb < strlen("POSINF) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->int_act+i)->special_max == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC) " );
				if (nb < strlen("UNSPEC) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%d) ", (ts->int_act+i)->max );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
		}
	}

	/* print action DOUBLES */
	if (ts->num_double_act > 0) {
		nb = snprintf( cp, cp_end-cp, "DOUBLES " );
		if (nb < strlen("DOUBLES ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
		for (i = 0; i < ts->num_double_act; i++) {
			nb = snprintf( cp, cp_end-cp, "(" );
			if (nb < 1 || cp+nb >= cp_end)
				return -1;
			cp += nb;
			if ((ts->double_act+i)->special_min == RV_NEGINF) {
				nb = snprintf( cp, cp_end-cp, "NEGINF, " );
				if (nb < strlen("NEGINF, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->double_act+i)->special_min == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC, " );
				if (nb < strlen("UNSPEC, ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%f, ", (ts->double_act+i)->min );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
			if ((ts->double_act+i)->special_max == RV_POSINF) {
				nb = snprintf( cp, cp_end-cp, "POSINF) " );
				if (nb < strlen("POSINF) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else if ((ts->double_act+i)->special_max == RV_UNSPEC) {
				nb = snprintf( cp, cp_end-cp, "UNSPEC) " );
				if (nb < strlen("UNSPEC) ") || cp+nb >= cp_end)
					return -1;
				cp += nb;
			} else {
				nb = snprintf( cp, cp_end-cp, "%f) ", (ts->double_act+i)->max );
				if (nb < 2 || cp+nb >= cp_end)
					return -1;
				cp += nb;
			}
		}
	}

	if (ts->charcount_act > 0) {
		nb = snprintf( cp, cp_end-cp, "CHARCOUNT %d ", ts->charcount_act );
		if (nb < strlen("CHARCOUNT ")+1 || cp+nb >= cp_end)
			return -1;
		cp += nb;
	}

	/*
	 * Print the reward range:
	 *
	 */
	nb = snprintf( cp, cp_end-cp, "REWARDS (" );
	if (nb < strlen("REWARDS (") || cp+nb >= cp_end)
		return -1;
	cp += nb;
	if ((ts->reward).special_min == RV_NEGINF) {
		nb = snprintf( cp, cp_end-cp, "NEGINF, " );
		if (nb < strlen("NEGINF, ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
	} else if ((ts->reward).special_min == RV_UNSPEC) {
		nb = snprintf( cp, cp_end-cp, "UNSPEC, " );
		if (nb < strlen("UNSPEC, ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
	} else {
		nb = snprintf( cp, cp_end-cp, "%f, ", (ts->reward).min );
		if (nb < 2 || cp+nb >= cp_end)
			return -1;
		cp += nb;
	}
	if ((ts->reward).special_max == RV_POSINF) {
		nb = snprintf( cp, cp_end-cp, "POSINF) " );
		if (nb < strlen("POSINF) ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
	} else if ((ts->reward).special_max == RV_UNSPEC) {
		nb = snprintf( cp, cp_end-cp, "UNSPEC) " );
		if (nb < strlen("UNSPEC) ") || cp+nb >= cp_end)
			return -1;
		cp += nb;
	} else {
		nb = snprintf( cp, cp_end-cp, "%f) ", (ts->reward).max );
		if (nb < 2 || cp+nb >= cp_end)
			return -1;
		cp += nb;
	}

	/* Extra spec string */
	if (ts->extra_spec != NULL) {
		nb = snprintf( cp, cp_end-cp, "EXTRA %s", ts->extra_spec );
		if (nb < strlen("EXTRA ")+strlen(ts->extra_spec)
			|| cp+nb >= cp_end)
			return -1;
		cp += nb;
	}
	*cp = '\0';

	return 0;
}


int free_taskspec_struct( taskspec_t *ts )
{
	/* There is probably a better, fault-tolerant way to do this... */

	/* free allocated arrays; note that free ignores NULL pointers */
	free( ts->version );
	free( ts->int_obs );
	free( ts->double_obs );
	free( ts->int_act );
	free( ts->double_act );
	free( ts->extra_spec );

	/* set all pointers to NULL in the taskspec struct */
	ts->version = NULL;
	ts->int_obs = NULL;
	ts->double_obs = NULL;
	ts->int_act = NULL;
	ts->double_act = NULL;
	ts->extra_spec = NULL;

	/* initialize all counts to zero */
	ts->num_int_obs = 0;
	ts->num_double_obs = 0;
	ts->charcount_obs = 0;
	ts->num_int_act = 0;
	ts->num_double_act = 0;
	ts->charcount_act = 0;

	return 0;
}


int_range_t *get_int_dims( const char *range_str, int *num_dims )
{
	char const *cp,
		*cp_close; /* point to the closing paren of the current pair */
	int i;

	int_range_t current_ran; /* range struct for processing each pair of parens */
	int repeat_count; /* number of times to repeat a tuple */						   
	
	int_range_t *ran_vect;
	int buf_len;
	int_range_t *tmp_ir_ptr; /* used when growing the int_range_t array buffer */
	
	char *endptr; /* used for detecting errors in string-to-integer
					 conversion with strtol */

	/* ignore empty parameter strings */
	if (range_str == NULL) {
		*num_dims = 0;
		return NULL;
	}

	cp = range_str;
	
	/* allocate array of range structs; this will grow in length as more
	   ranges are read. */
	buf_len = VECT_BUF_LEN;
	*num_dims = 0;
	ran_vect = (int_range_t *)malloc( VECT_BUF_LEN*sizeof(int_range_t) );

	/* step through each pair of parens */
	while ((cp = strchr( cp, '(' )) != NULL) {

		cp_close = strchr( cp, ')' );
		if (cp_close == NULL) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: missing closing paren */
			return NULL;
		}

		current_ran.special_min = current_ran.special_max = RV_NOTSPECIAL;
		repeat_count = 1;

		/* 
		 * Extract the integers, checking for special values:
		 *
		 */

		/* first entry: either repeat-count or min-value */
		cp = find_nonspace( cp+1 );
		if (cp == cp_close) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: an empty tuple! */
			return NULL;
		}

		if (isalpha( *cp )) { /* a special value? */

			if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
				current_ran.special_min = RV_UNSPEC;
			} else if (!strncasecmp( cp, "NEGINF", strlen("NEGINF") )) {
				current_ran.special_min = RV_NEGINF;
			} else {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: unrecognized special type */
				return NULL;
			}

		} else { /* attempt to extract the integer */

			errno = 0;
			current_ran.min = strtol( cp, &endptr, 0 );
			if (endptr == cp || errno == ERANGE) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: invalid integer string */
				return NULL;
			}

		}

		/* second entry: either min-value (if previous was a
		   repeat-count) or max-value.

           NOTE: for now, I assume this is the max-value and deal with
		   the special case of a 3-tuple (in which a repeat-count is
		   specified) only if a third entry is found before the
		   closing paren. */
		cp = strchr( cp, ' ' );
		cp = find_nonspace( cp );
		if (cp == cp_close || cp == NULL) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: a one tuple! */
			return NULL;
		}

		if (isalpha( *cp )) { /* a special value? */

			if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
				current_ran.special_max = RV_UNSPEC;
			} else if (!strncasecmp( cp, "POSINF", strlen("POSINF") )) {
				current_ran.special_max = RV_POSINF;
			} else {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: unrecognized special type */
				return NULL;
			}

		} else { /* attempt to extract the integer */

			errno = 0;
			current_ran.max = strtol( cp, &endptr, 0 );
			if (endptr == cp || errno == ERANGE) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: invalid integer string */
				return NULL;
			}

		}

		/* move forward to last (third) entry or closing paren */
		cp = strchr( cp, ' ' );
		if (cp == NULL)
			cp = cp_close;
		else
			cp = find_nonspace( cp );

		if (cp < cp_close) { /* Third entry is present, thus a
								repeat-count was given. */

			if (current_ran.special_min != RV_NOTSPECIAL
				|| current_ran.special_max == RV_POSINF) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: repeat-count
								   cannot be a special value, or min
								   special value cannot be positive
								   infinity. */
				return NULL;
			}

			/* adjust values in int_range struct to handle third entry */
			repeat_count = current_ran.min;
			current_ran.min = current_ran.max;
			current_ran.special_min = current_ran.special_max;

			if (repeat_count < 1) {
				free( ran_vect );
				*num_dims = -1; /* invalid repeat-count */
				return NULL;
			}
			
			if (isalpha( *cp )) { /* a special value? */

				if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
					current_ran.special_max = RV_UNSPEC;
				} else if (!strncasecmp( cp, "POSINF", strlen("POSINF") )) {
					current_ran.special_max = RV_POSINF;
				} else {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: unrecognized special type */
					return NULL;
				}

			} else { /* attempt to extract the integer */

				errno = 0;
				current_ran.max = strtol( cp, &endptr, 0 );
				if (endptr == cp || errno == ERANGE) {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: invalid integer string */
					return NULL;
				}

			}

			cp = strchr( cp, ' ' );
			if (cp != NULL && cp < cp_close) {
				cp = find_nonspace( cp );
				if (cp != cp_close) {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: invalid tuple size */
					return NULL;
				}
			}

		}
		cp = cp_close+1;		

		(*num_dims) += repeat_count;
		if (*num_dims > buf_len) { /* grow buffer if necessary */
			buf_len = (*num_dims)+VECT_BUF_LEN;
			tmp_ir_ptr = (int_range_t *)realloc( ran_vect, buf_len*sizeof(int_range_t) );
			if (tmp_ir_ptr == NULL) {
				free( ran_vect );
				*num_dims = -1; /* realloc call failed */
				return NULL;
			}
			ran_vect = tmp_ir_ptr;
		}
		
		/* finally, add this int range struct to the array */
		for (i = 0; i < repeat_count; i++) {
			(ran_vect+(*num_dims)-repeat_count+i)->min = current_ran.min;
			(ran_vect+(*num_dims)-repeat_count+i)->max = current_ran.max;
			(ran_vect+(*num_dims)-repeat_count+i)->special_min = current_ran.special_min;
			(ran_vect+(*num_dims)-repeat_count+i)->special_max = current_ran.special_max;
		}

	}

	/* free unused buffer space */
	if (buf_len > *num_dims) {
		tmp_ir_ptr = realloc( ran_vect, (*num_dims)*sizeof(int_range_t) );
		if (tmp_ir_ptr == NULL) {
			free( ran_vect );
			*num_dims = -1; /* memory error! */
			return NULL;
		}
		ran_vect = tmp_ir_ptr;
	}

	return ran_vect;
}


double_range_t *get_double_dims( const char *range_str, int *num_dims )
{
	char const *cp,
		*cp_close; /* point to the closing paren of the current pair */
	int i;

	double_range_t current_ran; /* range struct for processing each pair of parens */
	int repeat_count; /* number of times to repeat a tuple */						   
	
	double_range_t *ran_vect;
	int buf_len;
	double_range_t *tmp_ir_ptr; /* used when growing the double_range_t array buffer */
	
	char *endptr; /* used for detecting errors in string-to-number
					 conversions with strtol and strtod */

	/* ignore empty parameter strings */
	if (range_str == NULL) {
		*num_dims = 0;
		return NULL;
	}

	cp = range_str;
	
	/* allocate array of range structs; this will grow in length as more
	   ranges are read. */
	buf_len = VECT_BUF_LEN;
	*num_dims = 0;
	ran_vect = (double_range_t *)malloc( VECT_BUF_LEN*sizeof(double_range_t) );

	/* step through each pair of parens */
	while ((cp = strchr( cp, '(' )) != NULL) {

		cp_close = strchr( cp, ')' );
		if (cp_close == NULL) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: missing closing paren */
			return NULL;
		}

		current_ran.special_min = current_ran.special_max = RV_NOTSPECIAL;
		repeat_count = 1;

		/* 
		 * Extract the numbers, checking for special values:
		 *
		 */

		/* first entry: either repeat-count or min-value */
		cp = find_nonspace( cp+1 );
		if (cp == cp_close) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: an empty tuple! */
			return NULL;
		}

		if (isalpha( *cp )) { /* a special value? */

			if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
				current_ran.special_min = RV_UNSPEC;
			} else if (!strncasecmp( cp, "NEGINF", strlen("NEGINF") )) {
				current_ran.special_min = RV_NEGINF;
			} else {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: unrecognized special type */
				return NULL;
			}

		} else { /* attempt to extract the double */

			errno = 0;
			current_ran.min = strtod( cp, &endptr );
			if (endptr == cp || errno == ERANGE) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: invalid double string */
				return NULL;
			}

		}

		/* second entry: either min-value (if previous was a
		   repeat-count) or max-value.

           NOTE: for now, I assume this is the max-value and deal with
		   the special case of a 3-tuple (in which a repeat-count is
		   specified) only if a third entry is found before the
		   closing paren. */
		cp = strchr( cp, ' ' );
		cp = find_nonspace( cp );
		if (cp == cp_close || cp == NULL) {
			free( ran_vect );
			*num_dims = -1; /* malformed expression: a one tuple! */
			return NULL;
		}

		if (isalpha( *cp )) { /* a special value? */

			if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
				current_ran.special_max = RV_UNSPEC;
			} else if (!strncasecmp( cp, "POSINF", strlen("POSINF") )) {
				current_ran.special_max = RV_POSINF;
			} else {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: unrecognized special type */
				return NULL;
			}

		} else { /* attempt to extract the double */

			errno = 0;
			current_ran.max = strtod( cp, &endptr );
			if (endptr == cp || errno == ERANGE) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: invalid double string */
				return NULL;
			}

		}

		/* move forward to last (third) entry or closing paren */
		cp = strchr( cp, ' ' );
		if (cp == NULL)
			cp = cp_close;
		else
			cp = find_nonspace( cp );

		if (cp < cp_close) { /* Third entry is present, thus a
								repeat-count was given. */

			if (current_ran.special_min != RV_NOTSPECIAL
				|| current_ran.special_max == RV_POSINF) {
				free( ran_vect );
				*num_dims = -1; /* malformed expression: repeat-count
								   cannot be a special value, or min
								   special value cannot be positive
								   infinity. */
				return NULL;
			}

			/* adjust values in double_range struct to handle third entry */
			repeat_count = (int)current_ran.min;
			current_ran.min = current_ran.max;
			current_ran.special_min = current_ran.special_max;

			if (repeat_count < 1) {
				free( ran_vect );
				*num_dims = -1; /* invalid repeat-count */
				return NULL;
			}
			
			if (isalpha( *cp )) { /* a special value? */

				if (!strncasecmp( cp, "UNSPEC", strlen("UNSPEC") )) {
					current_ran.special_max = RV_UNSPEC;
				} else if (!strncasecmp( cp, "POSINF", strlen("POSINF") )) {
					current_ran.special_max = RV_POSINF;
				} else {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: unrecognized special type */
					return NULL;
				}

			} else { /* attempt to extract the double */

				errno = 0;
				current_ran.max = strtod( cp, &endptr );
				if (endptr == cp || errno == ERANGE) {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: invalid double string */
					return NULL;
				}

			}

			cp = strchr( cp, ' ' );
			if (cp != NULL && cp < cp_close) {
				cp = find_nonspace( cp );
				if (cp != cp_close) {
					free( ran_vect );
					*num_dims = -1; /* malformed expression: invalid tuple size */
					return NULL;
				}
			}

		}
		cp = cp_close+1;		

		(*num_dims) += repeat_count;
		if (*num_dims > buf_len) { /* grow buffer if necessary */
			buf_len = (*num_dims)+VECT_BUF_LEN;
			tmp_ir_ptr = (double_range_t *)realloc( ran_vect, buf_len*sizeof(double_range_t) );
			if (tmp_ir_ptr == NULL) {
				free( ran_vect );
				*num_dims = -1; /* realloc call failed */
				return NULL;
			}
			ran_vect = tmp_ir_ptr;
		}
		
		/* finally, add this double range struct to the array */
		for (i = 0; i < repeat_count; i++) {
			(ran_vect+(*num_dims)-repeat_count+i)->min = current_ran.min;
			(ran_vect+(*num_dims)-repeat_count+i)->max = current_ran.max;
			(ran_vect+(*num_dims)-repeat_count+i)->special_min = current_ran.special_min;
			(ran_vect+(*num_dims)-repeat_count+i)->special_max = current_ran.special_max;
		}

	}

	/* free unused buffer space */
	if (buf_len > *num_dims) {
		tmp_ir_ptr = realloc( ran_vect, (*num_dims)*sizeof(double_range_t) );
		if (tmp_ir_ptr == NULL) {
			free( ran_vect );
			*num_dims = -1; /* memory error! */
			return NULL;
		}
		ran_vect = tmp_ir_ptr;
	}

	return ran_vect;
}


char *find_end_ranges_list( const char *s, const char *end_delim )
{
	char *cp;
	char *cp_ints, *cp_doubles, *cp_charcount, *cp_delim;
	
	if (s == NULL || end_delim == NULL)
		return NULL; /* ignore NULL string pointers */

	cp = strstr( s, "INTS" );
	cp_ints = strstr( s, "ints" );
	if (cp_ints == NULL
		|| (cp != NULL && cp < cp_ints)) {
		cp_ints = cp;
	}

	cp = strstr( s, "DOUBLES" );
	cp_doubles = strstr( s, "doubles" );
	if (cp_doubles == NULL
		|| (cp != NULL && cp < cp_doubles)) {
		cp_doubles = cp;
	}

	cp = strstr( s, "CHARCOUNT" );
	cp_charcount = strstr( s, "charcount" );
	if (cp_charcount == NULL
		|| (cp != NULL && cp < cp_charcount)) {
		cp_charcount = cp;
	}

	cp_delim = strstr( s, end_delim );

	/* find the earliest occurring pointer */
	cp = cp_ints;
	if (cp == NULL || (cp_doubles != NULL && cp_doubles < cp))
		cp = cp_doubles;
	if (cp == NULL || (cp_charcount != NULL && cp_charcount < cp))
		cp = cp_charcount;
	if (cp == NULL || (cp_delim != NULL && cp_delim < cp))
		cp = cp_delim;

	return cp;
}


char const *find_nonspace( const char *s )
{
	char const *cp = s;

	if (s == NULL)
		return NULL; /* ignore NULL string pointers */

	while (*cp != '\0' && *cp == ' ')
		cp++;

	if (*cp == '\0')
		return NULL;
	else
		return cp;
}


#ifdef __cplusplus
}
#endif
