# Copyright (c) 1993 by Sanjay Ghemawat
#############################################################################
# Duration Dialog
#
# Commands
#
# get_duration <leader> <title> <doc> <min> <max> <init> <var>
#               Get a number of days in the range <min>..<max>.
#               The initial displayed value is <init>.
#               <title>/<doc> are used to prompt the user.  If the
#               user confirms the selection, set <var> to the selected number
#               and return true.  Else return false.

# Hidden global variables
#
#       duration_done                Is duration interaction finished

set duration_done 0

proc get_duration {leader title doc min max init var} {
    duration_make

    set result [duration_interact $leader $title $doc $min $max $init]
    if $result {
        upvar $var returnVar
        set returnVar [.duration_dialog.days get]
    }
    return $result
}

proc duration_make {} {
    set f .duration_dialog
    if [winfo exists $f] {return}

    toplevel $f -class Dialog
    wm title $f Dialog
    wm protocol $f WM_DELETE_WINDOW {set duration_done 0}

    frame $f.top -class Pane
    frame $f.mid -class Pane

    message $f.doc -aspect 400 -text {Uninitialized message} -justify center
    pack $f.doc -in $f.top -side top -expand 1 -fill both

    spinbox $f.days
    label $f.dayslabel -text {Days}

    make_buttons $f.bot 1 {
        {Cancel         {set duration_done 0}}
        {Okay           {set duration_done 1}}
    }

    pack $f.days -in $f.mid -side left -expand 1 -padx 5m -pady 5m
    pack $f.dayslabel -in $f.mid -side right -expand 1 -padx {0 5m} -pady 5m
    pack $f.top -side top -expand 1 -fill both
    pack $f.mid -side top -expand 1 -fill both
    pack $f.bot -side bottom -fill both

    bind $f <Control-c> {set duration_done 0}
    bind $f <Return>    {set duration_done 1}

    wm withdraw $f
    update
}

proc duration_interact {leader title doc min max init} {
    global duration_done
    set f .duration_dialog

    # Fix dialog contents
    $f.doc configure -text $doc
    $f.days configure -from $min -to $max -increment 1
    $f.days set $init
    wm title $f $title

    # Run dialog
    set duration_done -1
    dialog_run $leader $f duration_done
    return $duration_done
}
