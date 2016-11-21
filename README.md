__Gravity__ is a fast-paced multiplayer top-down space shooter.

# Usage

Compile with `make` for regular builds, then run `./Gravity`.

To enable debugging symbols and extra development options, compile with
`make DEBUG=1`, then run `./Gravity-dev`. Both builds can exist simultaneously.

The client and server are built in to the same executable. `./Gravity -h` gives
options.

## Example

Start a two-player game server with:

    ./Gravity -s -p 2

Then, each player connects like so:

    ./Gravity <server_address> -n Calvin
    ./Gravity <server_address> -n Hobbes

## Controls

W – accelerate

AD – turn the ship

Space – shoot

+/- or mousewheel – zoom in/out

C – switch camera mode
