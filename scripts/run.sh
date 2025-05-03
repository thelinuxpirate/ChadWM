#!/bin/sh

xset r rate 200 60 &
xset s off &
xset s noblank &
xset -dpms &
xrdb merge ~/.Xresources &
emacs --daemon &
feh --bg-fill ~/Pictures/Wallpapers/default.png &
dash ~/.config/chadwm/scripts/bar.sh &
picom &
flameshot &
dunst &
# playmectl -t ~/.local/audio/SMG2-CosmicCoveGalaxy.mp3

while type chadwm >/dev/null; do chadwm && continue || break; done
