#!/bin/bash
while true
do
sleep 1
leaks $(ps | grep minishell | grep -v grep | head -n 1 | awk '{ print $1 }')
done
