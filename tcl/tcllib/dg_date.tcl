# Copyright (c) 1993 by Sanjay Ghemawat
#############################################################################
# Date Dialog
#
# Commands
#
# get_date   <leader> <title> <doc> <init> <var>
#               Get a date from the user through selecting a day, month and year.
#               The initial displayed value is <init>.
#               <title>/<doc> are used to prompt the user.  If the
#               user confirms the selection, set <var> to the selected number
#               and return true.  Else return false.

# Hidden global variables
#
#       date_done                Is date interaction finished

set date_done 0

proc get_date {leader title doc init var} {
    date_make

    set result [date_interact $leader $title $doc $init]
    if $result {
        # get date from the user's inputs
        set f .date_dialog
        set day [$f.day get]
        set month [$f.month get]
        set year [$f.year get]
        set d [date make $day $month $year]
        # and return it
        upvar $var returnVar
        set returnVar $d
    }
    return $result
}

proc date_make {} {
    set f .date_dialog
    if [winfo exists $f] {return}

    toplevel $f -class Dialog
    wm title $f Dialog
    wm protocol $f WM_DELETE_WINDOW {set date_done 0}

    frame $f.top -class Pane
    frame $f.mid -class Pane

    message $f.doc -aspect 400 -text {Uninitialized message} -justify center
    pack $f.doc -in $f.top -side top -expand 1 -fill both

    spinbox $f.day -width 2 -from 1 -to 31 -increment 1
    spinbox $f.month -width 2 -from 1 -to 12 -increment 1 -command update_day_maximum
    spinbox $f.year -width 4 -from 0 -to 9999 -increment 1 -command update_day_maximum

    label $f.datesep1 -text "/"
    label $f.datesep2 -text "/"

    make_buttons $f.bot 1 {
        {Cancel         {set date_done 0}}
        {Okay           {set date_done 1}}
    }

    pack $f.day -in $f.mid -side left -padx 5m -pady 5m
    pack $f.datesep1 -in $f.mid -side left
    pack $f.month -in $f.mid -side left -padx 5m -pady 5m
    pack $f.datesep2 -in $f.mid -side left
    pack $f.year -in $f.mid -side left -padx 5m -pady 5m
    pack $f.top -side top -expand 1 -fill both
    pack $f.mid -side top -expand 1 -fill both
    
    pack $f.bot -side bottom -fill both

    bind $f <Control-c> {set date_done 0}
    bind $f <Return>    {set date_done 1}

    wm withdraw $f
    update
}

proc date_interact {leader title doc init} {
    global date_done
    set f .date_dialog

    # Fix dialog contents
    $f.doc configure -text $doc
    set d [date today]
    $f.day set [date monthday $d]
    $f.month set [date month $d]
    $f.year set [date year $d]
    update_day_maximum

    wm title $f $title

    # Run dialog
    set date_done -1
    dialog_run $leader $f date_done
    return $date_done
}

# change the maximum of the day spinbox to match the given month and year.
proc update_day_maximum {} {
    set f .date_dialog

    set day 1
    set month [$f.month get]
    set year [$f.year get]

    set d [date make $day $month $year]
    set dmax [date monthsize $d]

    $f.day configure -to $dmax
}