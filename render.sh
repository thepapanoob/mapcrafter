#!/bin/bash

#Renders the markers and the map
/usr/local/bin/mapcrafter_markers -c /config/render.conf >> /config/mapcrafter.log 2>&1 && \
/usr/local/bin/mapcrafter -j 4 -c /config/render.conf >> /config/mapcrafter.log 2>&1 && \

#Just to make sure that title and favicon are kept after the render is finished
#Copy title and favicon to be used in index.html
cp /config/title.txt /output/static/html/title.txt && \
cp /config/favicon.png /output/static/html/favicon.png