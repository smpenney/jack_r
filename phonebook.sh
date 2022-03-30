#!/bin/bash

PHONEBOOK=/tmp/phonebook.db

case $1 in
"new" )
    if [ -z "$2" ] || [  -z "$3" ]; then
        echo "Not enough arguments given: ${0} new NAME NUMBER"
        exit
    fi
    echo "${2},${3}" >> ${PHONEBOOK}
    exit;;
"remove" )
    while read entry; do
        echo $entry | awk -F, -v name="${2}" '{ 
            if( $1 != name ) {
                print $1 "," $2;
            }
        }' > ${PHONEBOOK}.tmp
    done < ${PHONEBOOK}
    mv ${PHONEBOOK}.tmp ${PHONEBOOK}
    exit;;
"list" )
    if [ -f ${PHONEBOOK} ] && [ -s ${PHONEBOOK} ]; then
        awk -F, '{print "Name: " $1 ", Phone: " $2'} ${PHONEBOOK}
    else
        echo "Phonebook is empty"
    fi
    exit;;
"clear" )
    rm ${PHONEBOOK}
    exit;;
* )
    while read entry; do
        echo $entry | awk -F, -v name="${1}" '{ 
            if( $1 == name ) {
                print "Name: " $1 ", Phone: " $2;
            }
        }'
    done < ${PHONEBOOK}
    exit;;
esac

