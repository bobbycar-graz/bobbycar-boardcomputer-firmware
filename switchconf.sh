#!/bin/bash

VALID_CONFIGS=(
    "comred"
    "peter"
    "feedc0de"
    "mick"
    "greyhash"
    "nofeatures"
    "allfeatures"
    "seatbot"
)

print_usage() {
    echo "usage: ./switchconf.sh ${VALID_CONFIGS[@]}"
}

if [[ -z "$1" ]]
then
    echo "ERROR: no buildconfig specified"
    print_usage
    exit 1
fi

if [[ ! " ${VALID_CONFIGS[@]} " =~ " $1 " ]]
then
    case "$1" in
      "--help")
        print_usage
        exit
        ;;

      "--list")
        for i in ${VALID_CONFIGS[@]}
        do
            echo "$i"
        done
        exit
        ;;

      *)
        echo "ERROR: invalid buildconfig \"$1\""
        print_usage
        exit 1
        ;;
    esac
fi

if [[ -L "build" ]]
then
    rm -v "build"
else
    if [[ -e "build" ]]
    then
        echo build is not a symlink
        mv "build" "build.bak" -v
    fi
fi

mkdir -pv "build_$1"

if [[ -L "sdkconfig" ]]
then
    rm -v "sdkconfig"
else
    if [[ -e "sdkconfig" ]]
    then
        echo sdkconfig is not a symlink
        mv "sdkconfig" "sdkconfig.bak" -v
    fi
fi

if [[ -L "config.cmake" ]]
then
    rm -v "config.cmake"
else
    if [[ -e "config.cmake" ]]
    then
        echo config.cmake is not a symlink
        mv "config.cmake" "config.cmake.bak" -v
    fi
fi

ln -vs "build_$1" "build"
ln -vs "sdkconfig_$1" "sdkconfig"
ln -vs "config_$1.cmake" "config.cmake"

echo all ok
