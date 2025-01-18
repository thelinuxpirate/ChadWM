#!/bin/sh

xset r rate 200 60 &
xset s off &
xset s noblank &
xrdb merge ~/.Xresources
dash ~/.config/chadwm/scripts/bar.sh &
feh --bg-fill ~/Pictures/Wallpapers/landscape/paper-city-art.jpg &
picom &
flameshot &
dunst &

while type chadwm >/dev/null; do chadwm && continue || break; done
