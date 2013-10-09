#!/bin/sh
./astyle.sh
./astyle-remove-backup.sh
./clean-tilda.sh
tar -zcvf ../../CE3D\(`date +%d%m%y`\).tar.gz ../
