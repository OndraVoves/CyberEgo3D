#!/bin/sh
find ../ -regex '.*\.\(c\|cpp\|h\)$' -print0 | xargs -0 astyle --options=astylerc
