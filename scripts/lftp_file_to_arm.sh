#! /bin/bash

lftp -u user1,q6coHjd7P @arm
put ./src/compile/basic.s
bye
EOF