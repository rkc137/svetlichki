# svetlichki
- [What?](#what)
- [Download](#download)
- [Config](#config)
- [Build](#Build)

## What

Desktop pets that wander around your screen on top of everything :D

![demo](https://github.com/user-attachments/assets/4f3d5579-913c-44a9-b723-ddecbf1cfa20)

To close just click the tray icon.

## Download
download last version for [windows](https://github.com/rkc137/svetlichki/releases/latest/download/svetlichki.zip)

## Config
`res/config.json`

```json
{
    "ratio": 0.04,
    "count": 7,
    "transparent": 150,
    "images": [
        {
            "texture_name": "svetlichki.png",
            "frame_size": [40, 40],
            "anim_lens": [4, 4, 4],
            "is_pixel": true
        }
    ]
}
```

- `ratio`: sprite size relative to screen size
- `count`: how many of each texture to spawn
- `transparent`: sprite alpha (0-255)
- `is_rgba_important`: `true` for real alpha-channel transparency (can cause big lags), `false` for a cheap colorkey mode (black = hole)
- `images`: list of textures

for each texture:
- `texture_name`: file name inside `res/`
- `frame_size`: size of one animation frame
- `anim_lens`: frame count per animation, rows top to bottom, animations in order (need at least 3: 2 for idle and 1 for moving)
- `is_pixel`: disables smoothing (pixel art)
- `is_top_down`: sprite is drawn topdown and rotates to face its movement direction, set to `false` if it's a side view, then it just flips horizontally instead


## Build

to build you need installed [SFML](https://sfml-dev.org) and [nlohmanns json](https://json.nlohmann.me/) or just provide USE_FETCHCONTENT on build.

```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DUSE_FETCHCONTENT=ON
cmake --build build --config Release
```

The exe and dlls land in `bin/`. A `res/` folder with the config and textures needs to sit next to it.
