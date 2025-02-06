#!/bin/sh

xset r rate 200 60 &
xset s off &
xset s noblank &
xset -dpms &
xrdb merge ~/.Xresources &
feh --bg-fill ~/Pictures/Wallpapers/landscape/pink_clouds.png &
dash ~/.config/chadwm/scripts/bar.sh &
picom &
flameshot &
dunst &
playme -t ~/.local/audio/SMG2-CosmicCoveGalaxy.mp3 -d 1 &

while type chadwm >/dev/null; do chadwm && continue || break; done
