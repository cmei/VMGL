#!/usr/bin/env bash
# Trigger suspend followed by resume of GL apps by sending SIGUSR2 to the GL apps
# (as documented in VMGL/extra/suspend-resume-helper/HowTo).
GL_APP_GREP_PATTERN="glxgears\|quake3"
ps aux | \
    grep "$GL_APP_GREP_PATTERN" | \
    grep -v 'gdb\|emacs\|grep' | \
    awk '{print $2}' | \
    xargs -n1 kill -USR2
