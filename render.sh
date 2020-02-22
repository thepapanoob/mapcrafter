#!/bin/bash
/usr/local/bin/mapcrafter_markers -c /config/render.conf >> /config/mapcrafter.log 2>&1 && /usr/local/bin/mapcrafter -j 4 -c /config/render.conf >> /config/mapcrafter.log 2>&1
