# @configure_input@
# Copyright (c) 1993 by Sanjay Ghemawat
###############################################################################
# Ical initialization.
#
# This code is linked into the ical binaries and executed at startup.

proc ical_init {} {
    set tv [info tclversion]
    if {[package vcompare $tv 7.5] < 0} {
        puts stderr "Ical requires at least Tcl version 7.5"
        exit 1
    }

    # Make sure there is "HOME" environment variable so that
    # tilde expansion does not blow up.
    global env
    if ![info exists env(HOME)] {set env(HOME) "/"}

    # Initialize options
    global ical
    set ical(author)            {sanjay@pa.dec.com}
    set ical(version)           {3.0.4}
    set ical(mailer)            {Mail}
    set ical(startdate)         [date today]
    set ical(iconic)            0
    set ical(prefs)             {}
    set ical(geometry)          {}
    set ical(iconposition)      {}

    # set library path for both platforms
    global tcl_platform
    if [string equal $tcl_platform(platform) windows] {
        # tcl files are beside the executable on windows
        set ical(libparent)         [.]
        set ical(library)           [./tcl]
    } else {
        # tcl files are in the directory set during compilation on linux
        # TECHNICALLY when compiling in windows these values are not substituted
        # so on windows this is a syntax error
        # but it's fine, its an interpreted language and we take a different branch (lol)
        set ical(libparent)         {@pkglibdir@}
        set ical(library)           {@icallibdir@}
    }

    # Handle environment variables
    if [info exists env(ICAL_LIBRARY)] {set ical(library) $env(ICAL_LIBRARY)}
    if [info exists env(CALENDAR)] {
        set ical(calendar) $env(CALENDAR)
    } else {
        set ical(calendar) [ical_expand_file_name ~/.calendar]
    }

    # Auto-loading path
    global auto_path
    set auto_path [concat\
                   [list $ical(library) $ical(library)/tcllib]\
                   $auto_path]

    set have_tk [string compare [info commands tk] ""]
    support_init
    if $have_tk ical_tk_init
}

proc ical_tk_init {} {
    global tk_version
    if {[package vcompare $tk_version 4.1] < 0} {
        puts stderr "Ical requires at least Tk version 4.1"
        exit 1
    }

    # Handle geometry value already parsed by Tk
    global geometry ical
    if [info exists geometry] {set ical(geometry) $geometry}

    # Load Tk support code
    tk_support_init
}
