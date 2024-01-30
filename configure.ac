AC_INIT([ical],[3.0.4])
AM_INIT_AUTOMAKE([subdir-objects])

# Create config files (config files are not currently used so commented out)
# AC_CONFIG_HEADERS([config.h])

############################################################################
# Check for programs

AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LN_S
AC_PATH_PROGS([MAILER],[Mail mail],[Mail])
AC_PROG_CXX
AC_PROG_SED

############################################################################
# Find Tcl/Tk

AS_MESSAGE([checking Tcl/Tk installation...])

#### Find tclsh, tclConfig.sh and tkConfig.sh
SC_PROG_TCLSH()
SC_PATH_TCLCONFIG()
SC_PATH_TKCONFIG()

#### Load the Tcl/Tk configuration files 
. $TCL_BIN_DIR/tclConfig.sh
. $TK_BIN_DIR/tkConfig.sh

#### Export variables discovered during Tcl/Tk configuration

AC_SUBST(TCL_LIB_SPEC)
AC_SUBST(TCL_PREFIX)
AC_SUBST(TCL_EXEC_PREFIX)
AC_SUBST(TCL_LIBS)
AC_SUBST(TK_LIB_SPEC)
AC_SUBST(TK_LIBS)
AC_SUBST(TK_PREFIX)
AC_SUBST(TK_EXEC_PREFIX)
AC_SUBST(TK_XINCLUDES)
AC_SUBST(TCL_INCLUDE_SPEC)
AC_SUBST(TK_INCLUDE_SPEC)

# Generate list of all .tcl files that need to be installed
generated='startup\|ps_header'
ICAL_TCLLIB_SCRIPTS=`echo $srcdir/tcl/tcllib/*.tcl|sed 's|/tcl/|/|g'`
ICAL_SCRIPTS=`echo $srcdir/tcl/*.tcl|sed -e "s/[[^ ]]*\($generated\)[[^ ]]*//g"|sed 's|/tcl/|/|g'`
ICAL_CONTRIB=`echo $srcdir/tcl/contrib/*|sed 's|/tcl/|/|g'`

AC_SUBST(ICAL_TCLLIB_SCRIPTS)
AC_SUBST(ICAL_SCRIPTS)
AC_SUBST(ICAL_CONTRIB)

AC_CHECK_FUNCS(fsync)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

