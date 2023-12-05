# Copyright (c) 1993 by Sanjay Ghemawat
##############################################################################
# Various Support Routines

# Initialization
set ical_state(views)   {}
set ical_state(view)    {}
set ical_state(clip)    {}
set ical_state(search)  {}

proc support_init {} {
    global month_name ical
    catch {unset month_name}

    set month_name(1)   January
    set month_name(2)   February
    set month_name(3)   March
    set month_name(4)   April
    set month_name(5)   May
    set month_name(6)   June
    set month_name(7)   July
    set month_name(8)   August
    set month_name(9)   September
    set month_name(10)  October
    set month_name(11)  November
    set month_name(12)  December

    global weekday_name
    catch {unset weekday_name}

    set weekday_name(1) Sunday
    set weekday_name(2) Monday
    set weekday_name(3) Tuesday
    set weekday_name(4) Wednesday
    set weekday_name(5) Thursday
    set weekday_name(6) Friday
    set weekday_name(7) Saturday

    # Parse arguments
    ical_parse_args
    if [string compare [info commands tk] ""] {
        support_init_tk
    }

    # Hooks
    create-hook ical-startup
    create-hook ical-exit

    create-hook item-create
    create-hook todo-item-done
    create-hook alarm-fire
    create-hook item-select
    create-hook item-unselect
    create-hook item-popup

    create-hook dayview-startup
    create-hook dayview-close
    create-hook dayview-set-date

    # Load various customization files
    ical_load_file $ical(libparent)/site.tcl
    ical_load_file $ical(library)/site.tcl
    ical_load_file ~/.tk/ical/user.tcl

    run-hook ical-startup
}

proc support_init_tk {} {
    global ical

    # Generate display type identifier
    set width  [winfo screenwidth .]
    set height [winfo screenheight .]
    set visual [winfo screenvisual .]
    set ical(displaytype) [set visual]_[set width]_[set height]

    # Parse tk arguments before calculating preferences
    ical_parse_tk_args

    # Handle preferences before user code gets loaded.
    pref_init

    # Special handling for background colors
    if [info exists ical(bg)] {tk_setPalette $ical(bg)}

    # Remove default window
    wm withdraw .
}

proc ical_load_file {name} {
    # See if the file exists.  We wrap the "file exists ..." command
    # inside a "catch" to handle errors generated by tilde expansion.
    set e 0
    catch {set e [file exists $name]}
    if $e {
        if [catch {uplevel #0 [list source $name]} msg] {
            error_notify "" "Error loading \"$name\"\n\n$msg"
        }
    }
}

proc ical_filenames {} {
    set list ""
    lappend list [cal main]
    cal forincludes x {
        lappend list $x
    }
    return $list
}

# Return title for calendar file name
proc ical_title {calendar} {
    # First look in calendar itself
    if ![catch {set title [cal option -calendar $calendar Title]}] {
        return $title
    }

    # No title in calendar:  Generate title from file name.
    if ![string compare $calendar [cal main]] {
        return [string cat [file tail $calendar] " (Current Calendar)"]
    } else {
        return [file tail $calendar]
    }
}

# effects - If listName contains specified string, remove it from the list
#           and return 1.  Else return 0.

proc lremove {listName string} {
    upvar $listName list

    set i [lsearch -exact $list $string]
    if {$i >= 0} {
        set list [lreplace $list $i $i]
        return 1
    } else {
        return 0
    }
}

# requires - item occurs on date.  leader is either "" or a top-level window.
# effects -  If item does not repeat, return "unnecessary".
#
#            If item repeats, but user interaction says it is ok to
#            modify all items, return "ok".
#
#            If item repeats, but user interaction says it is ok to
#            modify this instance, split item up so that modifications
#            will only happen to the instance on date, and return "instance".
#
#            If item repeats, and user interaction cancels the current
#            operation, return "cancel".

set ical_state(last) {}
set ical_state(modifylast) 0

proc repeat_check {leader item date} {
    if [catch {set cal [$item calendar]}] {return "cancel"}

    global ical_state
    set last $ical_state(last)
    set lastmod $ical_state(modifylast)
    set ical_state(last) $item
    set ical_state(modifylast) 0

    # Nothing to check if item does not repeat
    if ![$item repeats] {return "unnecessary"}

    # See if user already said it was okay to modify this item
    if {![string compare $item $last] && $lastmod} {
        set ical_state(modifylast) 1
        return "unnecessary"
    }

    set result [yes_no_cancel $leader\
                "This item repeats.  Should all occurrences be changed?"\
                "Yes" "Just this one" "Cancel"]

    if {$result == "cancel"} {return "cancel"}
    if {$result == "yes"} {
        set ical_state(modifylast) 1
        return "ok"
    }

    # Split item into two items.  One item only occurs on this
    # date.  The other item covers the rest of the occurrences.

    set copy [$item clone]
    $copy deleteon $date
    $item date $date

    cal add $copy $cal

    return "instance"
}

# effects - Print usage message and exist
proc ical_usage {} {
    puts stderr {Usage: ical [options]
          -calendar <file>          ; Calendar file
          -date <date>              ; Start ical on specified date
          -list                     ; List imminent items
          -show [+<days>]           ; Like "-list" but covers specified range
          -print (1|2|4|8|10|month) ; Generate postscript for specified range
    If on X display --
          -iconic                   ; Start iconified
          -iconposition <x,y>       ; Initial icon position
          -popup                    ; Just display imminent items
          -fg <color>               ; Foreground color
          -bg <color>               ; Background color
          -geometry <geometry>      ; Initial window geometry}
    exit 1
}

# effects - Parse some standard arguments
proc ical_parse_args {} {
    global argv ical
    set mono 0

    set oldargv $argv
    set argv {}
    while {[llength $oldargv] > 0} {
        set arg [lindex $oldargv 0]
        set oldargv [lrange $oldargv 1 end]

        if {![string compare $arg "-calendar"] && ([llength $oldargv] >= 1)} {
            set ical(calendar) [lindex $oldargv 0]
            set oldargv [lrange $oldargv 1 end]
            continue
        }

        if {![string compare $arg "-date"] && ([llength $oldargv] >= 1)} {
            set spec [lindex $oldargv 0]
            if [catch {set date [date_parse $spec]} msg] {
                puts stderr "ical: $msg"
                ical_usage
            }
            set ical(startdate) $date
            set oldargv [lrange $oldargv 1 end]
            continue
        }

        lappend argv $arg
    }
}

# effects - Parse some standard Tk arguments
proc ical_parse_tk_args {} {
    global argv ical

    set oldargv $argv
    set argv {}
    while {[llength $oldargv] > 0} {
        set arg [lindex $oldargv 0]
        set oldargv [lrange $oldargv 1 end]

        if {![string compare $arg "-iconic"]} {
            set ical(iconic) 1
            continue
        }

        if {![string compare $arg "-iconposition"] && ([llength $oldargv] >= 1)} {
            if ![scan [lindex $oldargv 0] "%d,%d" x y] {ical_usage}
            set ical(iconposition) [list $x $y]
            set oldargv [lrange $oldargv 1 end]
            continue
        }

        if {![string compare $arg "-bg"] && ([llength $oldargv] >= 1)} {
            set ical(bg) [lindex $oldargv 0]
            set oldargv [lrange $oldargv 1 end]
            continue
        }

        if {![string compare $arg "-fg"] && ([llength $oldargv] >= 1)} {
            lappend ical(prefs) "option add *Foreground [lindex $oldargv 0]"
            set oldargv [lrange $oldargv 1 end]
            continue
        }

        lappend argv $arg
    }
}

proc date_parse {spec} {
    if {[date extract $spec date j1 j2] && ("x$j1$j2" == "x")} {return $date}
    error "could not parse \"$spec\" as a date"
}

proc tk_support_init {} {
    # Ical specific keys
    global keymap

    # Used during item editing
    set keymap(item) {
        # that is Ctrl+Shift+X, note the capital X
        <Control-Key-X>         ical_delete
        <Control-Key-c>         ical_copy
        <Control-Key-x>         ical_cut_or_hide
        <Control-Key-w>         ical_delete_selection
        <Control-Key-y>         ical_insert_selection
    }

    # Used for invoking commands
    set keymap(command) {
        <Control-Key-v>         ical_paste
        <Key-Left>              ical_last_day
        <Key-Right>             ical_next_day
        <Key-Down>              ical_next_week
        <Key-Up>                ical_last_week
        <Control-Key-s>         ical_search_forward
        <Control-Key-r>         ical_search_backward
        <Key-Escape>            ical_unselect
        <Control-Key-g>         ical_unselect
        <Meta-Key-n>            ical_cycle_through_items
        <Key-Next>              ical_cycle_through_items
        <Key-Tab>               ical_cycle_through_items
    }

    ical_init_bindings

    # Catch calendar reconfigurations
    trigger on reconfig {ical_reconfig_options}
    trigger on keybind {ical_recompute_bindings}
}

proc ical_init_bindings {} {
    # Record current focus window so we can find current view easily
    bind Dayview <FocusIn> {ical_focus_on %W}

    # Do not allow normal tab bindings
    bind Dayview <Tab> {break}

    # Event tag for disabling X coordinate scanning
    bind YScan <2>              {%W scan mark 0 %y; break}
    bind YScan <B2-Motion>      {%W scan dragto 0 %y; break}

    # Configure menu whenever it gets the focus or is mapped
    bind Menu <FocusIn> {ical_config_menu %W}
    bind Menu <Map>     {ical_config_menu %W}
}

proc ical_recompute_bindings {} {
    global keymap

    ical_make_bindings          IcalItem                $keymap(item)
    ical_make_bindings          IcalCommand             $keymap(command)
    itemwindow_make_bindings    IcalItemEditBindings

    # Read any user defined key bindings from calendar
    catch {ical_make_bindings IcalUser [cal option Keybindings]}

    # Append "break" to all non-empty bindings so that after any
    # of these bindings fire, no other binding fires.
    foreach tag {IcalUser IcalItem IcalCommand IcalItemEditBindings} {
        ical_add_bind_breaks $tag
    }
}

# requires      "bindings" is a list of pairs of form {<key> <command>}
# effects       Sets $tag to have the specified set of bindings.
proc ical_make_bindings {tag bindings} {
    foreach s [bind $tag] {bind $tag $s ""}
    foreach {s c} $bindings {bind $tag $s $c}
}

# For each binding associated with "$tag", add a "break" command to
# the end of the binding command.
proc ical_add_bind_breaks {tag} {
    foreach seq [bind $tag] {
        bind $tag $seq "[bind $tag $seq]; break"
    }
}

proc ical_reconfig_options {} {
    global dv_state
    set dv_state(state:overflow)        [cal option AllowOverflow]
    set dv_state(state:ampm)            [cal option AmPm]
    set dv_state(state:mondayfirst)     [cal option MondayFirst]
}

#### Menu-item enabling code ####

# An ical action X can be setup to be disabled or enabled based on the
# value of an entry in the global variable "ical_enable" by setting
# "ical_action_enabler($X)" to the name of entry.
#
# Currently the following entries in "ical_enable" are supported:
#       ical_enable(always)     always set to "normal" 
#       ical_enable(writable)   "normal" iff main calendar is writable
#       ical_enable(item)       "normal" iff an item is selected
#       ical_enable(appt)       "normal" iff an appointment is selected
#       ical_enable(witem)      "normal" iff a writable item is selected
#       ical_enable(wappt)      "normal" iff a writable appointment is selected

# The following proc is called whenever a menu is activated.  It enables
# or disables the menu entries according to the current state of the
# application.
proc ical_config_menu {w} {
    global ical_enable ical_action_enabler

    # Set-up the variables controlling enable/disable
    set ical_enable(always) normal
    set ical_enable(item) disabled
    set ical_enable(appt) disabled
    set ical_enable(witem) disabled
    set ical_enable(wappt) disabled
    set ical_enable(writable) normal

    if ![catch {set item [ical_find_selection]}] {
        set ical_enable(item) normal
        catch {
            if ![cal readonly [$item calendar]] {
                set ical_enable(witem) normal
            }
        }
        if [$item is appt] {
            set ical_enable(appt) normal
            catch {
                if ![cal readonly [$item calendar]] {
                    set ical_enable(wappt) normal
                }
            }
        }
    }
    if [cal readonly] {set ical_enable(writable) disabled}

    # Now process menu entries
    set last [$w index last]
    for {set i 0} {$i <= $last} {incr i} {
        catch {
            set cmd [lindex [$w entrycget $i -command] 0]
            $w entryconfigure $i\
                -state $ical_enable($ical_action_enabler($cmd))
        }
    }
}
