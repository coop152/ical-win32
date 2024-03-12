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

# effects - Automatically remove items from the delete history that are older than the configured date.
proc check_for_autopurge {} {
    set now [date today]
    set purge_delay [cal option AutoPurgeDelay]
    if {$purge_delay == 0} { 
        # autopurge disabled, don't do anything
        return
    }
    set purge_thresh [expr {$now - $purge_delay}]

    # save history mode state, go into history mode and purge old items, then restore history mode state
    set oldhistorymode $::ical_state(historymode)
    set ::ical_state(historymode) 1
    
    cal historymode 1

    set safe [cal option AutoPurgeSafe]
    perform_autopurge $purge_thresh $safe
    set ::ical_state(historymode) $oldhistorymode
    cal historymode $oldhistorymode
}

proc perform_autopurge {d safe} {
    set count 0
    set items {}
    cal query 0 $d item item_date {
        # don't mass delete important items
        if {[$item important]} {continue}
        incr count
        lappend items [list $item $item_date]
    }

    # don't bother the user if there's nothing to delete
    if {$count == 0} { return }

    if {!$safe} {
        # just delete without showing a list
        set user_choice "no"
    } else {
        set user_choice [yes_no_cancel [ical_leader] "$count old item(s) in the delete history are set to be autopurged.\nPermanently delete them?" "List items" "Delete" "Cancel"] 
    }
    if {$user_choice == "yes"} { # yes to seeing a listing
        set l [ItemListing]
        # last argument of 0 to hide important items
        $l fromlist $items
        tkwait window .$l
        if {[yes_or_no [ical_leader] "Delete those items?"]} {
            delete_item_list $items
        } else { return }
    } elseif {$user_choice == "no"} { # no to seeing a listing (delete immediately)
        delete_item_list $items
    } else {
        # user cancelled, do nothing
        return
    }
}

# effects Startup a thread that will do the right thing at each midnight.
proc start_midnight_thread {} {
    set now [ical_time now]
    set split [ical_time split $now]
    set offset [expr "([lindex $split 0]*60*60 +\
                       [lindex $split 1]*60 +\
                       [lindex $split 2])"]

    # try autopurge both at program start and at midnight
    check_for_autopurge

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
