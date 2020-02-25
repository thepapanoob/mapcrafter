# Mapcrafter
Docker container to run [Mapcrafter](https://mapcrafter.org) 
This repo was make mainly to be used in my [personal server](https://map.confusion.pt/) but I've adapted it to a template, so it could be used by more people.

[Docker build ready for 1.15](https://hub.docker.com/r/dinip/mapcrafter)

# Usage
## Minecraft 1.15
```bash
docker run -d -v /path/to/output:/output -v /path/to/config:/config -v /path/to/world:/world -e threads=4 --name mapcrafter dinip/mapcrafter:world115
```
Change `-e threads=` to how many threads you want the render to use. Don't set this value higher than the amount of cores that your cpu have. If no value is set, it will set automatically to 4. 

**dinip/mapcrafter:world115 is the only image maintained at this date (22/02/2020).**

# Volumes

## /world
Minecraft world folder

## /config
Inside this folder need to be 4 files, `render.conf`, `mapcrafter-cron`, `favicon.png` and `title.txt`. An example of this files are created in the first run if they are not found.
- `render.conf` is the configs for the mapcrafter render. [See docs here](https://docs.mapcrafter.org/builds/stable/configuration.html)
- `mapcrafter-cron` is where you define the schedule to run the render. By default it will run at 5 minutes past the hour every hour (eg.: 10:05, 11:05, 12:05) but this can be changed to fit your needs, you just need to keep the `/opt/./render.sh` part after the time expression. [more info here](https://crontab-generator.org/). To apply the changes, a restart of the container is necessary.
- `favicon.png` and `title.txt` will be used to customize your index.html that is shown on the browser. To change the title you just need to edit the text in `title.txt` file. For favicon just override the file with your new favicon file. This files can be directly edited in the output folder, they are in the config folder just for easier access.

## /output
Generated files go to this path.
