#!/bin/bash

# Find the root of the building area.
___hitana_root() {
    COUNT=10
    while true; do
			if [ -e ./include/hitana -a -d ./include/hitana -a -e ./include/hitana/HitAna.hh ]; then
				echo ${PWD}
				return
			fi
			COUNT=$(expr ${COUNT} - 1)
			if [ ${COUNT} -lt 1 ]; then
				echo invalid-directory
				return
			fi
			cd ..
    done
}

export HITANA_ROOT
HITANA_ROOT=$(___hitana_root)
unset -f ___hitana_root

if [ ${HITANA_ROOT} = "invalid-directory" ]; then
    echo HITANA setup.sh must be sourced in your installation directory.
    return
fi

if [ "$(uname)" == "Darwin" ]; then
    export DYLD_LIBRARY_PATH=$HITANA_ROOT/lib:${DYLD_LIBRARY_PATH}
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    export LD_LIBRARY_PATH=$HITANA_ROOT/lib:${LD_LIBRARY_PATH}
fi
