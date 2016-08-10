#!/usr/bin/env bash
set -e
# set -x

rsync_out=$(mktemp)
install_host_out=$(mktemp)
cleanup()
{
    rm_if() {
        if [ -e $1 ]; then
            rm $1
        fi
    }
    rm_if $rsync_out
    rm_if $install_host_out
}
trap cleanup EXIT

TOP="$(readlink -e "$(dirname "$0")")"
REMOTE_TOP="$HOME/clone/VMGL.sync"
PASS=james
cd "$TOP"
# if ! ( make -w -j4 2>&1 ) > out.txt; then
if ! make -w -j4 2>err.txt >out.txt; then
    err=$?
    cat err.txt
    # cat out.txt
    echo "ERROR: build failed with status $err"
    exit $err
fi
echo -n "RSYNC -> guest / make install-guest"
if ! rsync -avz $TOP/ root@domu_guest:$REMOTE_TOP/ &> $rsync_out; then
    err=$?
    cat $rsync_out
    echo "ERROR: rsync failed with status $err"
    exit $err
fi
echo

echo $PASS | sudo -S make install-host &> $install_host_out &
install_host_pid=$!

ssh domu_guest "
    # set -x;
    cd $REMOTE_TOP; 
    ( set -e; 
      echo $PASS | sudo -S make install-guest &> out.txt;
      echo SUCCESS; 
    ) || \
    ( err=\$?; 
      echo \"ERROR: saw status = \$err\";
      cat out.txt;
      exit \$err; )"

if ! wait $install_host_pid; then
    err=$?
    echo "ERROR: make install-host failed with status $err:"
    cat $install_host_out
    exit $err
fi
