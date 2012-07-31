#!/bin/tcsh
 
# This file was generated by the script "mk_setenv.csh"
#
# Generation date: Tue Jul 31 13:21:02 EDT 2012
# User: gluex
# Host: roentgen.jlab.org
# Platform: Linux roentgen.jlab.org 2.6.32-279.1.1.el6.x86_64 #1 SMP Wed Jun 20 11:41:22 EDT 2012 x86_64 x86_64 x86_64 GNU/Linux
# BMS_OSNAME: Linux_RHEL6-x86_64-gcc4.4.6
 
if ( ! $?BMS_OSNAME ) then
   setenv BMS_OSNAME `/group/halld/Software/scripts/osrelease.pl`
endif
 
if ( -e /group/halld/Software/builds/sim-recon/sim-recon-2012-07-26/setenv_${BMS_OSNAME}.csh ) then
    # Try prepending path of cwd used in generating this file
    source /group/halld/Software/builds/sim-recon/sim-recon-2012-07-26/setenv_${BMS_OSNAME}.csh
else if ( -e setenv_${BMS_OSNAME}.csh ) then
    source setenv_${BMS_OSNAME}.csh
endif 
 
