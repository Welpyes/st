#!/bin/bash

OUTPUT_FILE="$TMPDIR/st-bg-temp.png"

# Default values
BLUR_AMOUNT=10
COLOR="#16161e"
OPACITY=0.5
IMAGE_PATH=""

show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS] IMAGE_PATH

Convert an image to a blurred background for st terminal.

OPTIONS:
    -b, --blur AMOUNT       Blur amount (default: 10)
    -c, --color COLOR       Overlay color in hex (default: #16161e)
    -o, --opacity VALUE     Overlay opacity 0.0-1.0 (default: 0.5)
    -h, --help              Show this help message

EXAMPLE:
    $(basename "$0") -b 15 -c "#1a1b26" -o 0.6 ~/Pictures/wallpaper.jpg
EOF
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--blur)
            BLUR_AMOUNT="$2"
            shift 2
            ;;
        -c|--color)
            COLOR="$2"
            shift 2
            ;;
        -o|--opacity)
            OPACITY="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            IMAGE_PATH="$1"
            shift
            ;;
    esac
done

# Validate image path
if [[ -z "$IMAGE_PATH" ]]; then
    echo "Error: No image path provided"
    show_help
    exit 1
fi

if [[ ! -f "$IMAGE_PATH" ]]; then
    echo "Error: Image file not found: $IMAGE_PATH"
    exit 1
fi

# Expand tilde if present
IMAGE_PATH="${IMAGE_PATH/#\~/$HOME}"

# Get screen resolution
SCREEN_RES=$(xrandr | grep '*' | awk '{print $1}' | head -1)
SCREEN_WIDTH=$(echo $SCREEN_RES | cut -dx -f1)
SCREEN_HEIGHT=$(echo $SCREEN_RES | cut -dx -f2)

# Process with higher bit depth and better blur
ffmpeg -i "$IMAGE_PATH" -filter_complex "\
[0:v]scale=$SCREEN_WIDTH:$SCREEN_HEIGHT:force_original_aspect_ratio=increase,\
crop=$SCREEN_WIDTH:$SCREEN_HEIGHT,\
format=rgb48le,\
gblur=sigma=$BLUR_AMOUNT,\
format=rgba[blurred];\
color=c=$COLOR:s=${SCREEN_WIDTH}x${SCREEN_HEIGHT}:duration=1[overlay];\
[blurred][overlay]blend=all_mode=normal:all_opacity=$OPACITY" \
-pix_fmt rgb24 -y "$OUTPUT_FILE" 2>/dev/null

# Convert to farbfeld format
png2ff < "$OUTPUT_FILE" > "$HOME/.cache/st_wallpaper.ff"

# Clean up temp file
rm "$OUTPUT_FILE"
killall -USR1 st
