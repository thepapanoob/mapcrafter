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

if [ ! -f /config/mapcrafter-cron ]; then
    echo "No favicon found. Creating new one..."
    cp /defaults/favicon.png /config/favicon.png
fi

#Copy crontab file from config to cron.d directory and gives permission to run
cp /config/mapcrafter-cron /etc/cron.d/mapcrafter-cron && \
chmod 0777 /etc/cron.d/mapcrafter-cron && \
#Renders the markers and the map
/usr/local/bin/mapcrafter_markers -c /config/render.conf >> /config/mapcrafter.log 2>&1 && \
/usr/local/bin/mapcrafter -j 4 -c /config/render.conf >> /config/mapcrafter.log 2>&1 && \
#Copy title and favicon to be used in index.html
cp /config/title.txt /output/static/html/title.txt && \
cp /config/favicon.png /output/static/html/favicon.png