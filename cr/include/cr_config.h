#ifndef CR_CONFIG_H_DOHQYRES
#define CR_CONFIG_H_DOHQYRES

/**********************************************************************/
/*****     Useful configuration stuff     *****/
/**********************************************************************/
#define CR_LOOP_ON_ERROR

/* Make it so X11 commands aren't buffered.  Makes it easier debug to when calls fail.
 * Set a breakpoint at _XError to break at the failed call.
 */
#define CR_X11_SYNC_CALLS

#endif /* end of include guard: CR_CONFIG_H_DOHQYRES */
