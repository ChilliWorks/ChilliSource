#!/bin/bash
# Script to update Revision.h based on svn revision numbers on moFlo and current app
# Allows application to know what revision is has been built from

# Set our path to include stuff for OS X
export PATH="/opt/subversion/bin:/usr/bin:$PATH"

# Which version of svn are we using
svn --version

WORKSPACE=$1
APP_REV=$(svn info "$WORKSPACE" | grep "Revision" | cut -f2 -d' ')
MOFLOW_REV=$(svn info "$WORKSPACE/ChilliSource/Source/ChilliSource" | grep "Revision" | cut -f2 -d' ')

if test $(grep -E "$APP_REV|$MOFLOW_REV" "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h" | wc -l) -eq 2;
    then echo "Revision.h up to date"
else
    echo "Rebuilding Revision.h"
    echo "#ifndef _MO_FLO_REVISION_H" > "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h"
    echo "#define _MO_FLO_REVISION_H" >> "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h"
    echo "#define APP_REVISION_NUMBER \"$APP_REV\"" >> "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h"
    echo "#define MOFLOW_REVISION_NUMBER \"$MOFLOW_REV\"" >> "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h"
    echo "#endif" >> "$WORKSPACE/ChilliSource/Source/ChilliSource/Revision.h"
fi