# Copyright (c) 1993 by Sanjay Ghemawat
#
# Startup script for X-based ical

proc ical_tk_script {} {
    # Parse arguments (some argument parsing has already been done
    # by startup.tcl)
    global argv ical

    set popup 0
    while {[llength $argv] != 0} {
        set arg [lindex $argv 0]
        set argv [lrange $argv 1 end]

        switch -- $arg {
            "-popup" {set popup 1}
            default  {ical_usage}
        }
    }

    # Load calendar
    calendar cal $ical(calendar)
    trigger fire reconfig
    trigger fire keybind

    if {$popup} {
        # Create item listing
        set l [ItemListing]
        $l mainwindow
        $l dayrange $ical(startdate) $ical(startdate)
        if {$ical(geometry) != ""} {
            catch {wm geometry [$l window] $ical(geometry)}
        }
    } else {
        # Various background threads
        io_thread
        start_alarmer
        start_midnight_thread

        # Create initial view
        set dv [ical_newview]
        $dv set_date $ical(startdate)
        if {$ical(iconic)} {wm iconify [$dv window]}
        if {$ical(geometry) != ""} {
            catch {wm geometry [$dv window] $ical(geometry)}
        }
        if {$ical(iconposition) != ""} {
            eval [list wm iconposition [$dv window]] $ical(iconposition)
        }
    }
}

# effects - Automatically remove items from the delete history that are 
proc autopurge {} {
    set now [date today]
    set purge_delay [cal option AutoPurgeDelay]
    if {$purge_delay == 0} { 
        # autopurge disabled, don't do anything
        return
    }
    set purge_thresh [expr {$now - $purge_delay}]
    
    # save history mode state, go into history mode and purge old items, then restore history mode state
    set oldhistorymode $::ical_state(historymode)
    set $::ical_state(historymode) 1
    cal historymode 1
    ask_to_deleteallbefore $purge_thresh

    set $::ical_state(historymode) $oldhistorymode
    cal historymode $oldhistorymode
}

# effects Startup a thread that will do the right thing at each midnight.
proc start_midnight_thread {} {
    set now [ical_time now]
    set split [ical_time split $now]
    set offset [expr "([lindex $split 0]*60*60 +\
                       [lindex $split 1]*60 +\
                       [lindex $split 2])"]

    # try autopurge both at program start and at midnight
    autopurge

    # Find time remaining today and schedule an event after that much time.
    # Note: the "after" command takes time in milliseconds.
    after [expr (24*60*60 - $offset)*1000] {
        trigger fire midnight
        start_midnight_thread
    }
}

# Handle background errors
proc bgerror {message} {
    global ical errorInfo

    # XXX Fairly stupid handling of unknown color errors
    if [string match "unknown color name*" $message] {
        option add *Foreground black interactive
        option add *Background white interactive
        error_notify "" "$message.\n\nMost likely, you specified a bad color name in your X resources.  Please fix the problem and restart ical."
        return
    }

    set message "Ical Version $ical(version)\n\n$message\n\nTrace:\n$errorInfo"
    bug_notify $ical(mailer) $ical(author) $message
}
