# Copyright (c) 1993 by Sanjay Ghemawat
#############################################################################
# File Dialog
#
# Commands
#
#       get_file_name <leader> <title> <message> <var> [<init>]
#               Get file name from user.  The name is stored in <var>.
#               Returns true iff user does not cancel operation.

proc get_file_name {leader title message var {init {}}} {
    # leader and message are ignored, but kept for compatibility
    set result [tk_getOpenFile -title $title -initialdir $init]
    if {$result ne ""} {
        upvar $var returnVar
        set returnVar $result
        return 1;
    }
    return 0;
}