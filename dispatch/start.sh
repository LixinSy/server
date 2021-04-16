ulimit -s 2048
ulimit -n 102400
ulimit -c 1000000

if [ ! -f start.sh ]; then
    echo 'start.sh must be run within its container folder' 1>&2
    exit 1
fi

if [ ! -f gameserver ]; then
    echo "ERROR: dispatch file not exist!" 1>&2
    exit 1
fi

`pwd`/dispatch
