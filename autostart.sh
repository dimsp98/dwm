#!/bin/zsh

#Xorg management
xsetroot -cursor_name left_ptr &
xrandr --output HDMI-1 --mode 1920x1080 --rate 60 &
xset s off &
xset -dpms &

#keybindings 
pgrep -x sxhkd > /dev/null || sxhkd -c $HOME/.config/sxhkd/sxhkdrc &

#compositor
pgrep -x picom > /dev/null || picom --config ~/.config/picom/picom.conf &

#wallpaper
feh --bg-scale --no-fehbg /home/dimitris/dotfiles/Backgrounds/wallhaven-7pj8jo_2560x1440.png & 

#clipboard 
xclip &

#change language 
setxkbmap -model pc104 -layout us,gr -option 'grp:alt_shift_toggle' &

#notifications
pgrep -x dunst > /dev/null || dunst -c ~/.config/dunst/dunstrc &

exec /usr/local/bin/slstatus &
exec /usr/local/bin/dwm 
