#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

# Variables:
interval=0
# TokyoNight ColorScheme:
black=#1a1b26
green=#9ece6a
white=#a9b1d6
purple=#9d7cd8
grey=#24283b
blue=#7aa2f7
red=#f7768e
orange=#ff9e64
darkblue=$blue

audio() {
  muted=$(pamixer --get-mute 2>/dev/null)
  volume=$(pamixer --get-volume-human 2>/dev/null)

  if [ "$muted" = "true" ]; then
    printf "^c$black^ ^b$red^ "
    echo   "^c$white^ ^b$grey^ MUTED"
  else
    printf "^c$black^ ^b$purple^ "
    echo   "^c$white^ ^b$grey^ $(echo "$volume" | sed 's/%/%/')"
  fi
}

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

  printf "^c$black^ ^b$green^ CPU"
  printf "^c$white^ ^b$grey^ $cpu_val"
}

mem() {
  printf "^c$black^ ^b$red^  "
  printf "^c$white^^b$grey^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

wlan() {
  case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
  up) printf   "^c$black^ ^b$orange^ 󰤨 ^d^%s" "^c$white^^b$grey^ Connected" ;;
  down) printf "^c$black^ ^b$red^ 󰤭 ^d^%s" "^c$white^^b$grey^ Disconnected" ;;
  esac
}

clock() {
  printf "^c$black^^b$darkblue^ 󱑆 "
  printf "^c$white^^b$grey^ $(date '+%I:%M %p') "
}

while true; do
  interval=$((interval + 1))
  sleep 1
  xsetroot -name "$(audio) $(cpu) $(mem) $(wlan) $(clock)"
done
