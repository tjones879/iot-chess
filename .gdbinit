set target-async on
set confirm off
set mem inaccessible-by-default off
#set debug remote 1
tar ext /dev/ttyBmpGdb
mon version
mon swdp_scan
att 1
