#!/bin/bash
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# */

action=debug # release debug test

if [ -f "/project/public/base/lib/libjemalloc.so" ]; then
   export LD_PRELOAD="/project/public/base/lib/libjemalloc.so /project/public/base/lib/libjson-c.so"
fi

function loop_notify()
{
    while true
    do
        echo "$1"
        read reply leftover
        case $reply in
            boot | reboot)
                return
                ;;
        esac
    done
}

function jupyter_booting()
{
    if command -v jupyter > /dev/null 2>&1; then
        jupyter notebook --allow-root &
    fi
}

function goto_debug()
{
    jupyter_booting
    loop_notify "container on $action mode!"
}

hasflagnum=$(grep -c "FLAG_FOR_UPDATE" ${0##*/})
if [ $hasflagnum == 2 ]; then
    goto_debug
fi

if [ "$action" = "release" ];then
goto_debug #___FLAG_FOR_UPDATE.SH___
else
    goto_debug
fi