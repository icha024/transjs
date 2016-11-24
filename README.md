# TransJS

Transforms log4j2 JSON layout log files into a flat pattern layout.

## Build
`gcc -o transjs main.c`

## Install
`sudo cp transjs /usr/local/bin/`

## Usage
`cat myLogFile.json | transjs`

(or just tail and pipe into it)
