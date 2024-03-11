# Copyright (c) 1993 by Sanjay Ghemawat
#############################################################################
# Autopurge Settings Dialog
#
# Commands
#
# get_autopurge_settings <leader> <daysinit> <daysvar> <safeinit> <safevar>
#               Prompt the user for autopurge settings (delay in days, and if it should be silent).
#               The initial displayed delay value is <daysinit>, and the initial safe value is <safeinit>.
#               If the
#               user confirms the selection, set <daysvar> to the selected number
#               and return true.  Else return false.

# Hidden global variables
#
#       ap_dialog_done                Is the interaction finished

set ap_dialog_done 0
set safe 0
set title "Autopurge Settings"
set doc "Specify the number of days between an item being deleted and it being automatically purged from the delete history. (Set to 0 to disable.)"

proc get_autopurge_settings {leader daysinit daysvar safeinit safevar} {
    global safe
    ap_dialog_make

    # set safe $safeinit

    set result [ap_dialog_interact $leader $daysinit $safeinit]
    if $result {
        set f .ap_settings_dialog
        upvar $daysvar daysVarOut
        set daysVarOut [$f.days get]
        upvar $safevar safeVarOut
        set safeVarOut $safe
    }
    return $result
}

proc ap_dialog_make {} {
    global title
    global doc
    global safe
    set f .ap_settings_dialog
    if [winfo exists $f] {return}

    toplevel $f -class Dialog
    wm title $f $title
    wm protocol $f WM_DELETE_WINDOW {set ap_dialog_done 0}

    frame $f.top -class Pane
    frame $f.mid -class Pane
    frame $f.mid2 -class Pane

    message $f.doc -aspect 400 -text $doc -justify center
    pack $f.doc -in $f.top -side top -expand 1 -fill both

    spinbox $f.days -from 0 -to 365 -increment 1
    label $f.dayslabel -text {Days}
    checkbutton $f.safebox -text {Ask before purging items} -variable safe

    make_buttons $f.bot 1 {
        {Cancel         {set ap_dialog_done 0}}
        {Okay           {set ap_dialog_done 1}}
    }

    pack $f.days -in $f.mid -side left -expand 1 -padx 5m -pady 5m
    pack $f.dayslabel -in $f.mid -side right -expand 1 -padx {0 5m} -pady 5m
    pack $f.safebox -in $f.mid2 -side bottom -expand 1 -pady 5m
    pack $f.top -side top -expand 1 -fill both
    pack $f.mid -side top -expand 1 -fill both
    pack $f.mid2 -side top -expand 1 -fill both
    pack $f.bot -side bottom -fill both

    bind $f <Control-c> {set ap_dialog_done 0}
    bind $f <Return>    {set ap_dialog_done 1}

    wm withdraw $f
    update
}

proc ap_dialog_interact {leader daysinit safeinit} {
    global ap_dialog_done
    global safe
    set f .ap_settings_dialog

    # set initial values
    $f.days set $daysinit
    set safe $safeinit

    # Run dialog
    set ap_dialog_done -1
    dialog_run $leader $f ap_dialog_done
    return $ap_dialog_done
}
