#!/bin/bash

if [ ! -f /config/render.conf ]; then
    echo "No config file found. Creating new one..."
    cp /defaults/render.conf /config/render.conf
fi

if [ ! -f /config/title.txt ]; then
    echo "No title file found. Creating new one..."
    cp /defaults/title.txt /config/title.txt
fi

if [ ! -f /config/mapcrafter-cron ]; then
    echo "No crontab file found. Creating new one..."
    cp /defaults/mapcrafter-cron /config/mapcrafter-cron
fi

if [ ! -f /config/favicon.png ]; then
    echo "No favicon found. Creating new one..."
    cp /defaults/favicon.png /config/favicon.png
fi

if [ ! -f /config/mapcrafter.log ]; then
    echo "No log file found. Creating new one..."
    touch /config/mapcrafter.log
fi

if [ ! -d /output/static/html/ ]
then
    echo "Directory not found found. Will be created after the first render!"
else
    #Copy title and favicon to be used in index.html
    cp /config/title.txt /output/static/html/title.txt && \
    cp /config/favicon.png /output/static/html/favicon.png
fi

#Copy crontab file from config to cron.d directory and gives permission to run
cp /config/mapcrafter-cron /etc/cron.d/mapcrafter-cron && \
chmod 0777 /etc/cron.d/mapcrafter-cron && \
crontab /etc/cron.d/mapcrafter-cron && \
cron