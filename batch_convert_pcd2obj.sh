#!/bin/sh
for i in *.pcd ; do pcl_convert_pcd_ascii_binary "$i" "${i%.*}.ascii" 0; grep -v nan "${i%.*}.ascii" | sed -e '1,11d;s/^/v /' > "${i%.*}.obj" ; rm "${i%.*}.ascii" ; done
